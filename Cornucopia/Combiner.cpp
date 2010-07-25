/*--
    Combiner.cpp  

    This file is part of the Cornucopia curve sketching library.
    Copyright (C) 2010 Ilya Baran (ibaran@mit.edu)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Combiner.h"
#include "GraphConstructor.h"
#include "PrimitiveFitter.h"
#include "PathFinder.h"
#include "Preprocessing.h"
#include "Resampler.h"
#include "Polyline.h"
#include "Solver.h"
#include "ErrorComputer.h"
#include "Fitter.h"

#include <iterator>

#include <Eigen/LU>
#include <Eigen/Cholesky>

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

class MulticurveDenseEvalData : public LSEvalData
{
public:
    //overrides
    double error() const { return /*_err.squaredNorm() + 1e5 */ _con.squaredNorm(); }

    void solveForDelta(double damping, Eigen::VectorXd &out, std::set<LSBoxConstraint> &constraints)
    {
        int vars = (int)_errDer.cols();

        int size = vars + (int)_con.size() + constraints.size();
        MatrixXd lhs = MatrixXd::Zero(size, size);
        VectorXd rhs = VectorXd::Zero(size);

        lhs.block(0, 0, vars, vars) = _errDer.transpose() * _errDer;
        rhs.segment(0, vars) = -_errDer.transpose() * _err;

        lhs.block(vars, 0, _conDer.rows(), _conDer.cols()) = _conDer;
        lhs.block(0, vars, _conDer.cols(), _conDer.rows()) = _conDer.transpose();
        rhs.segment(vars, _con.size()) = -_con;

        int cnt = 0;
        for(set<LSBoxConstraint>::const_iterator it = constraints.begin(); it != constraints.end(); ++it, ++cnt)
            lhs(vars + _conDer.rows() + cnt, it->index) = lhs(it->index, vars + _conDer.rows() + cnt) = 1.;

        //lhs += damping * MatrixXd::Identity(size, size);
        lhs.block(0, 0, vars, vars) += damping * MatrixXd::Identity(vars, vars);
        VectorXd result = lhs.lu().solve(rhs);

        out = result.segment(0, vars);

        printf("Solve err = %lf\n", (lhs * result - rhs).norm());
        if(_conDer.size() > 0)
            printf("Con Solve err = %lf\n", (_conDer * out + _con).norm());

        //check which constraints we don't need
        cnt = 0;
        for(set<LSBoxConstraint>::iterator it = constraints.begin(); it != constraints.end(); ++cnt)
        {
            set<LSBoxConstraint>::iterator next = it;
            ++next;
            if(result(vars + _conDer.rows() + cnt) * it->sign > 0)
            {
                printf("Unsetting constraint on variable at index %d\n", it->index);
                constraints.erase(it);
            }
            it = next;
        }
    }

    //for derivative verification, combine error and constraints
    VectorXd errVec() const
    {
        VectorXd out(_err.size() + _con.size());
        out << _err, _con;
        return out;
    }

    MatrixXd errVecDer() const
    {
        MatrixXd out(_err.size() + _con.size(), _errDer.cols());
        out << _errDer, _conDer;
        return out;
    }

    VectorXd &errVectorRef() { return _err; }
    MatrixXd &errDerRef() { return _errDer; }

    VectorXd &conVectorRef() { return _con; }
    MatrixXd &conDerRef() { return _conDer; }

private:
    Eigen::VectorXd _err;
    Eigen::MatrixXd _errDer;
    Eigen::VectorXd _con;
    Eigen::MatrixXd _conDer;
};

class MulticurveProblem : public LSProblem
{
public:
    MulticurveProblem(const Fitter &fitter)
        : _primitives(fitter.output<PRIMITIVE_FITTING>()->primitives), _iter(0)
    {
        smart_ptr<const AlgorithmOutput<GRAPH_CONSTRUCTION> > graph = fitter.output<GRAPH_CONSTRUCTION>();
        const vector<int> &path = fitter.output<PATH_FINDING>()->path;
        _errorComputer = fitter.output<ERROR_COMPUTER>()->errorComputer;
        _closed = fitter.output<CURVE_CLOSING>()->closed;

        for(int i = 0; i < (int)path.size(); ++i)
        {
            _primIdcs.push_back(graph->edges[path[i]].startVtx);
            _continuities.push_back(graph->edges[path[i]].continuity);
        }
        if(!_closed)
            _primIdcs.push_back(graph->edges[path.back()].endVtx);
        
        _curves = VectorC<CurvePrimitivePtr>(_primIdcs.size(), _closed ? CIRCULAR : NOT_CIRCULAR);
        for(int i = 0; i < (int)_primIdcs.size(); ++i)
        {
            //TODO: make _curveRanges a VectorC and get rid of ni below
            _curveRanges.push_back(make_pair(_primitives[_primIdcs[i]].startIdx, _primitives[_primIdcs[i]].endIdx));
            _curves[i] = _primitives[_primIdcs[i]].curve->clone();
        }

        //trim curves and ranges
        const int sampledPts = fitter.output<RESAMPLING>()->output->pts().size();
        for(int i = 0; i < (int)_continuities.size(); ++i)
        {
            if(_continuities[i] == 0)
                continue;
            int ni = (i + 1) % _primIdcs.size();

            //trim
            Vector2d trimPt = 0.5 * (_curves[i]->endPos() + _curves[ni]->startPos());
            _curves[i]->trim(0, _curves[i]->project(trimPt));
            _curves[ni]->trim(_curves[ni]->project(trimPt), _curves[ni]->length());

            //the ranges over which error is computed should not overlap
            if(_continuities[i] == 1)
                swap(_curveRanges[i].second, _curveRanges[ni].first); //they overlap by 1 originally
            if(_continuities[i] == 2) //they overlap by 2
            {
                _curveRanges[ni].first = (_curveRanges[ni].first + 2) % sampledPts;
                _curveRanges[i].second = (_curveRanges[i].second + sampledPts - 2) % sampledPts;
            }
        }
    }

    vector<LSBoxConstraint> getConstraints() const
    {
        vector<LSBoxConstraint> out;

        int curVar = 0;
        for(int i = 0; i < _curves.size(); ++i)
        {
            //length must be at least half initial
            out.push_back(LSBoxConstraint(curVar + CurvePrimitive::LENGTH, _curves[i]->length() * 0.5, 1));

            //TODO: inflections

            curVar += _curves[i]->numParams();
        }

        return out;
    }

    int _iter;
    LSEvalData *createEvalData() { return new MulticurveDenseEvalData(); }
    void eval(const Eigen::VectorXd &x, LSEvalData *data)
    {
        setParams(x);
        MulticurveDenseEvalData *evalData = static_cast<MulticurveDenseEvalData *>(data);
        _evalError(evalData);
        _evalConstraints(evalData);
        printf("Err: obj = %lf con = %lf\n", evalData->errVectorRef().norm(), evalData->conVectorRef().norm()); 
    }

    void setParams(const Eigen::VectorXd &x)
    {
        int curIdx = 0;
        for(int i = 0; i < (int)_curves.size(); ++i)
        {
            if(_curves[i]->getType() != CurvePrimitive::CLOTHOID)
            {
                _curves[i]->setParams(x.segment(curIdx, _curves[i]->numParams()));
            }
            else
            {
                VectorXd xm = x.segment(curIdx, _curves[i]->numParams());
                xm(CurvePrimitive::DCURVATURE) = (xm(CurvePrimitive::DCURVATURE) - xm(CurvePrimitive::CURVATURE)) / xm(CurvePrimitive::LENGTH);
                _curves[i]->setParams(xm);
            }
            curIdx += _curves[i]->numParams();
        }

        char name[100];
        sprintf(name, "Out%d", _iter);
        for(int i = 0; i < _curves.size(); ++i)
            Debugging::get()->drawPrimitive(_curves[i], name, i, 2.);
        ++_iter;
    }

    Eigen::VectorXd params()
    {
        int totParams = 0;
        for(int i = 0; i < (int)_curves.size(); ++i)
            totParams += _curves[i]->numParams();

        Eigen::VectorXd out(totParams);

        int curIdx = 0;
        for(int i = 0; i < (int)_curves.size(); ++i)
        {
            if(_curves[i]->getType() != CurvePrimitive::CLOTHOID)
            {
                out.segment(curIdx, _curves[i]->numParams()) = _curves[i]->params();
            }
            else
            {
                VectorXd xm = _curves[i]->params(); 
                xm(CurvePrimitive::DCURVATURE) = xm(CurvePrimitive::CURVATURE) + xm(CurvePrimitive::DCURVATURE) * xm(CurvePrimitive::LENGTH);
                out.segment(curIdx, _curves[i]->numParams()) = xm;
            }
            curIdx += _curves[i]->numParams();
        }

        return out;
    }

    VectorC<CurvePrimitiveConstPtr> curves() const
    {
        VectorC<CurvePrimitiveConstPtr> out;
        copy(_curves.begin(), _curves.end(), back_inserter(out));
        return out;
    }

private:
    void _evalError(MulticurveDenseEvalData *evalData)
    {
        VectorXd &outErr = evalData->errVectorRef();
        MatrixXd &outErrDer = evalData->errDerRef();

        vector<VectorXd> errVecs(_curves.size());
        vector<MatrixXd> errVecDers(_curves.size());

        int numErr = 0, numVar = 0;
        for(int i = 0; i < (int)_curves.size(); ++i)
        {
            int csz = (int)_continuities.size();
            bool firstCorner = (!_closed && i == 0) || (_continuities[(i + csz - 1) % csz] == 0);
            bool lastCorner = (!_closed && i + 1 == (int)_curves.size()) || (_continuities[i] == 0);

            _errorComputer->computeErrorVector(_curves[i], _curveRanges[i].first, _curveRanges[i].second,
                errVecs[i], &(errVecDers[i]), firstCorner, lastCorner);
            
            _curves[i]->toEndCurvatureDerivative(errVecDers[i]);

            numErr += errVecs[i].size();
            numVar += _curves[i]->numParams();
        }

        outErr = VectorXd::Zero(numErr);
        outErrDer = MatrixXd::Zero(numErr, numVar);

        int curErr = 0, curVar = 0;
        for(int i = 0; i < (int)_curves.size(); ++i)
        {
            int nErr = errVecs[i].size();
            int nVar = errVecDers[i].cols();
            outErr.segment(curErr, nErr) = errVecs[i];
            outErrDer.block(curErr, curVar, nErr, nVar) = errVecDers[i];
            curErr += nErr;
            curVar += nVar;
        }
    }

    void _evalConstraints(MulticurveDenseEvalData *evalData)
    {
        VectorXd &outCon = evalData->conVectorRef();
        MatrixXd &outConDer = evalData->conDerRef();

        vector<VectorXd> conVecs(_continuities.size());
        vector<MatrixXd> conVecDers(_continuities.size());

        CurvePrimitive::EndDer endDer;

        int numCon = 0, numVar = 0;
        for(int i = 0; i < (int)_continuities.size(); ++i)
        {
            conVecs[i].resize(2 + _continuities[i]);
            conVecs[i].head<2>() = _curves[i]->endPos() - _curves[i + 1]->startPos();
            if(_continuities[i] >= 1)
                conVecs[i][2] = AngleUtils::toRange(_curves[i]->endAngle() - _curves[i + 1]->startAngle(), -PI);
            if(_continuities[i] == 2)
                conVecs[i][3] = _curves[i]->endCurvature() - _curves[i + 1]->startCurvature();

            _curves[i]->derivativeAtEnd(_continuities[i], endDer);
            conVecDers[i] = endDer;
            _curves[i]->toEndCurvatureDerivative(conVecDers[i]);

            numCon += conVecs[i].size();
            numVar += _curves[i]->numParams();
        }

        if(!_closed)
            numVar += _curves.back()->numParams();

        outCon = VectorXd::Zero(numCon);
        outConDer = MatrixXd::Zero(numCon, numVar);

        int curCon = 0, curVar = 0;
        for(int i = 0; i < (int)_continuities.size(); ++i)
        {
            int nCon = conVecs[i].size();
            int nVar = conVecDers[i].cols();
            outCon.segment(curCon, nCon) = conVecs[i];
            outConDer.block(curCon, curVar, nCon, nVar) = conVecDers[i];

            //now the derivatives for the second curve
            outConDer(curCon + 0, (curVar + nVar + CurvePrimitive::X) % numVar) = -1.;
            outConDer(curCon + 1, (curVar + nVar + CurvePrimitive::Y) % numVar) = -1.;
            if(nCon > 2)
                outConDer(curCon + 2, (curVar + nVar + CurvePrimitive::ANGLE) % numVar) = -1.;
            if(nCon > 3 && _curves[i + 1]->getType() != CurvePrimitive::LINE)
                outConDer(curCon + 3, (curVar + nVar + CurvePrimitive::CURVATURE) % numVar) = -1.;

            curCon += nCon;
            curVar += nVar;
        }
    }

    VectorC<CurvePrimitivePtr> _curves;
    const vector<FitPrimitive> &_primitives;
    vector<int> _primIdcs;
    vector<int> _continuities;
    vector<pair<int, int> > _curveRanges;
    bool _closed;
    ErrorComputerConstPtr _errorComputer;
};

class DefaultCombiner : public Algorithm<COMBINING>
{
public:
    string name() const { return "Default"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<COMBINING> &out)
    {
        smart_ptr<const AlgorithmOutput<GRAPH_CONSTRUCTION> > graph = fitter.output<GRAPH_CONSTRUCTION>();
        const vector<FitPrimitive> &primitives = fitter.output<PRIMITIVE_FITTING>()->primitives;
        const vector<int> &path = fitter.output<PATH_FINDING>()->path;
        bool closed = fitter.output<CURVE_CLOSING>()->closed;

        if(path.empty())
            return; //no path

        //if a single primitive
        if(graph->edges[path[0]].continuity == -1)
        {
            out.output = VectorC<CurvePrimitiveConstPtr>(1, NOT_CIRCULAR);
            out.output[0] = primitives[graph->edges[path[0]].startVtx].curve;

            Debugging::get()->drawPrimitive(out.output[0], "Final Result", 0, 2.);

            return; //no combining necessary
        }

        MulticurveProblem problem(fitter);
        vector<LSBoxConstraint> constraints = problem.getConstraints();
        LSSolver solver(&problem, constraints);
        solver.setDefaultDamping(1.);
        solver.setMaxIter(100);

        //solver.verifyDerivatives(problem.params());
        VectorXd result = solver.solve(problem.params());
        problem.setParams(result);

        out.output = problem.curves();
    }
};

void Algorithm<COMBINING>::_initialize()
{
    new DefaultCombiner();
}

END_NAMESPACE_Cornu



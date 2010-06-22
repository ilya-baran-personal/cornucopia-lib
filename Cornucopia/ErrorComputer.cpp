/*--
    ErrorComputer.cpp  

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

#include "ErrorComputer.h"
#include "Resampler.h"
#include "Fitter.h"
#include "Polyline.h"
#include "CurvePrimitive.h"

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

class DefaultErrorComputer : public ErrorComputer
{
public:
    DefaultErrorComputer(const Fitter &fitter)
        : _pts(fitter.output<RESAMPLING>()->output->pts())
    {
        PolylineConstPtr poly = fitter.output<RESAMPLING>()->output;
        CircularType circular = _pts.circular();
        _weightsLeft = VectorC<double>(_pts.size(), circular);
        _weightsRight = VectorC<double>(_pts.size(), circular);
        _weightLeftRoots = VectorC<double>(_pts.size(), circular);
        _weightRightRoots = VectorC<double>(_pts.size(), circular);
        _weightRoots = VectorC<double>(_pts.size(), circular);

        for(int i = 0; i < _pts.size(); ++i)
        {
            VectorC<Vector2d>::Circulator prev = --_pts.circulator(i);
            VectorC<Vector2d>::Circulator next = ++_pts.circulator(i);
            _weightsLeft[i] = prev.done() ? 0 : poly->lengthFromTo(prev.index(), i);
            _weightsRight[i] = next.done() ? 0 : poly->lengthFromTo(i, next.index());
            _weightLeftRoots[i] = sqrt(_weightsLeft[i]);
            _weightRightRoots[i] = sqrt(_weightsRight[i]);
            _weightRoots[i] = sqrt(_weightsLeft[i] + _weightsRight[i]);
        }
    }

    double computeError(CurvePrimitiveConstPtr curve, int from, int to) const
    {
        double error = 0;

        bool first = true;
        for(VectorC<Vector2d>::Circulator circ = _pts.circulator(from); ; ++circ)
        {
            int idx = circ.index();
            bool last = (idx == to);

            const Vector2d &pt = _pts.flatAt(idx);
            double distSq = (curve->pos(curve->project(pt)) - pt).squaredNorm();
            double weight = 0;
            if(!first)
                weight += _weightsLeft.flatAt(idx);
            if(!last)
                weight += _weightsRight.flatAt(idx);

            error += weight * distSq;

            first = false;
            if(last)
                break;
        }

        return error;
    }

    void computeErrorVector(CurvePrimitiveConstPtr curve, int from, int to, VectorXd &outError, MatrixXd *outErrorDer) const
    {
        int numParams = curve->params().size();
        int numOutputs = 2 * (_pts.numElems(from, to) + 1); //to is inclusive
        outError.resize(numOutputs); 
        if(outErrorDer)
            outErrorDer->resize(numOutputs, numParams);
         
        CurvePrimitive::ParamDer der, tanDer;
        bool first = true;
        int vecIdx = 0;
        for(VectorC<Vector2d>::Circulator circ = _pts.circulator(from); ; ++circ, vecIdx += 2)
        {
            int idx = circ.index();
            bool last = (idx == to);

            const Vector2d &pt = _pts.flatAt(idx);
            double weightRoot = 0;
            if(first)
                weightRoot = _weightRightRoots.flatAt(idx);
            else if(last)
                weightRoot = _weightLeftRoots.flatAt(idx);
            else
                weightRoot = _weightRoots.flatAt(idx);

            double s = curve->project(pt);
            Vector2d err = curve->pos(s) - pt;
            outError.segment<2>(vecIdx) = err * weightRoot;

            if(outErrorDer)
            {
                curve->derivativeAt(s, der, tanDer);
                Vector2d tangent = curve->der(s);
                RowVectorXd ds = RowVectorXd::Zero(numParams); 

                const double tol = 1e-10;

                if(s + tol >= curve->length())
                    ds(CurvePrimitive::LENGTH) = 1.;
                else if(s > tol)
                {
                    double dfds = 1. + curve->der2(s).dot(err);
                    if(fabs(dfds) < tol)
                        dfds = (dfds < 0. ? -tol : tol);
                    ds = -(err.transpose() * tanDer + tangent.transpose() * der) / dfds;
                }

                outErrorDer->block(vecIdx, 0, 2, der.cols()) = (der + tangent * ds) * weightRoot;
            }

            first = false;
            if(last)
                break;
        }
    }

private:
    const VectorC<Vector2d> &_pts;
    VectorC<double> _weightsLeft, _weightsRight, _weightLeftRoots, _weightRightRoots, _weightRoots;
};

class DefaultErrorComputerCreator : public Algorithm<ERROR_COMPUTER>
{
public:
    string name() const { return "Default"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<ERROR_COMPUTER> &out)
    {
        out.errorComputer = new DefaultErrorComputer(fitter);
    }
};

void Algorithm<ERROR_COMPUTER>::_initialize()
{
    new DefaultErrorComputerCreator();
}


END_NAMESPACE_Cornu



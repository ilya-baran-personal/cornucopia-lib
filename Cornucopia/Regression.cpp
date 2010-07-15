/*--
    Regression.cpp  

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

#include "Regression.h"
#include "Solver.h"
#include <fstream>
#include <iostream> //TMP
#include <sstream> //TMP
#include <Eigen/Cholesky>
#include <Eigen/LU>

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

int IndependentValue::discreteId() const
{
    return (continuity << 12) +
           (type[0] << 10) +
           (type[1] << 8) +
           ((startCurvSign[0] + 1) << 6) +
           ((startCurvSign[1] + 1) << 4) +
           ((endCurvSign[0] + 1) << 2) +
           (endCurvSign[1] + 1);
}

IndependentValue::FeatureVector IndependentValue::featureVector() const
{
    FeatureVector out(4 + continuity);

    int idx = 0;
    out[idx++] = 1.;
    out[idx++] = length[0];
    out[idx++] = length[1];
    out[idx++] = diffs[0];
    if(continuity >= 1)
        out[idx++] = diffs[1] * length[0];
    if(continuity >= 2)
        out[idx++] = diffs[2] * length[0] * length[0];

    return out;
}

void Dataset::save(const string &filename) const
{
    ofstream os(filename.c_str(), ofstream::binary);
    if(!os.is_open())
        return;

    int count = (int)_data.size();
    os.write((char *)&count, sizeof(int));
    os.write((char *)&(_data[0]), count * sizeof(_data[0]));
}

void Dataset::load(const string &filename) //appends to existing data
{
    ifstream is(filename.c_str(), ifstream::binary);
    if(!is.is_open())
        return;

    int origSize = (int)_data.size();
    int count = 0;
    is.read((char *)&count, sizeof(int));
    _data.resize(origSize + count);

    if(is.eof())
        return;

    is.read((char *)&(_data[origSize]), count * sizeof(_data[0]));
}

DataModel::DataModel(DatasetConstPtr data)
{
    vector<pair<IndependentValue, double> > pts = data->data();
    if(pts.empty())
        return;

    double totalErr = 0, totalLen = 0;
    for(int i = 0; i < (int)pts.size(); ++i)
    {
        totalErr += pts[i].second;
        totalLen += pts[i].first.length[0];
    }
    cout << "avg err before filter = " << totalErr / totalLen << "\n";

    //filter
    vector<int> toRemove;
    for(int i = 0; i < (int)pts.size(); ++i)
    {
        double err = pts[i].second;
        double len = pts[i].first.length[0];
        
        if(fabs(err) > len * 3.) //average squared error more than 3 pixels
            toRemove.push_back(i);
    }
    int offs = 0;
    for(int i = 0; i < (int)pts.size(); ++i)
    {
        if(offs < (int)toRemove.size() && toRemove[offs] == i)
            ++offs;
        else
            pts[i - offs] = pts[i];
    }
    pts.resize((int)pts.size() - offs);

    //sort
    std::sort(pts.begin(), pts.end());

    //dump
    if(false)
    {
        ofstream os("lendat.dat");
        for(int i = 0; i < (int)pts.size(); ++i)
        {
            if(pts[i].first.continuity == 2)
                os << pts[i].first.discreteId() << " " << pts[i].second << " " << pts[i].first.featureVector().transpose() << "\n";
        }
    }

    totalErr = 0, totalLen = 0;
    for(int i = 0; i < (int)pts.size(); ++i)
    {
        totalErr += pts[i].second;
        totalLen += pts[i].first.length[0];
    }
    cout << "avg err after filter = " << totalErr / totalLen << "\n";


    _coefs.resize(1 + pts.back().first.discreteId());

    for(int i = 0; i < (int)pts.size(); ++i)
    {
        int id = pts[i].first.discreteId();

        int cnt = 0;
        int start = i;
        for(; i < (int)pts.size(); ++i)
        {
            ++cnt;
            if(i + 1 < (int)pts.size() && id != pts[i + 1].first.discreteId())
                break;
        }

        _computeCoefs(pts, start, cnt);
    }
}

//Just a regular linear least squares problem
class CoefsProblem : public LSProblem
{
public:
    CoefsProblem(const MatrixXd &lhs, const VectorXd &rhs)
        : _lhs(lhs), _rhs(rhs)  {}

    //overrides
    double error(const VectorXd &x)
    {
        return (_lhs * x - _rhs).squaredNorm();
    }

    LSEvalData *createEvalData()
    {
        return new LSDenseEvalData();
    }
    void eval(const VectorXd &x, LSEvalData *data)
    {
        LSDenseEvalData *denseData = static_cast<LSDenseEvalData *>(data);
        denseData->errVectorRef() = _lhs * x - _rhs;
        denseData->errDerRef() = _lhs;
    }

private:
    MatrixXd _lhs;
    VectorXd _rhs;
};


void DataModel::_computeCoefs(const vector<pair<IndependentValue, double> > &pts, int start, int cnt)
{
    int dim = (int)pts[start].first.featureVector().size();
    int id = pts[start].first.discreteId();

    MatrixXd lhs = MatrixXd::Identity(dim,  dim) * 0.01;
    VectorXd rhs = VectorXd::Zero(dim);
    const IndependentValue &cur = pts[start].first;

    for(int i = start; i < start + cnt; ++i)
    {
        VectorXd vec = pts[i].first.featureVector();
        lhs += (vec * vec.transpose());
        rhs += vec * pts[i].second;
    }

    //cout << "lhs = " << lhs << " rhs = " << rhs << "\n\n";

#if 0 //unconstrained
    LDLT<MatrixXd> ldlt(lhs);
    if(!ldlt.isPositive())
    {
        Debugging::get()->printf("Id = %d, num = %d, not positive definite", id, cnt);
        return;
    }

    _coefs[id] = ldlt.solve(rhs);
#else //constrained to positive
    LLT<MatrixXd> llt(lhs);

    CoefsProblem problem(llt.matrixL().transpose(), llt.matrixL().solve(rhs));
    vector<LSBoxConstraint> constraints;

    for(int i = 0; i < rhs.size(); ++i)
        constraints.push_back(LSBoxConstraint(i, 0., 1));

    LSSolver solver(&problem, constraints);
    solver.setMaxIter(10);
    _coefs[id] = solver.solve(VectorXd::Zero(rhs.size()));
#endif

    double err = 0;
    double targetSqr = 0;
    double totVal = 0;
    for(int i = start; i < start + cnt; ++i)
        totVal += pts[i].second;
    double mean = totVal / double(cnt);

    for(int i = start; i < start + cnt; ++i)
    {
        VectorXd vec = pts[i].first.featureVector();
        err += SQR(vec.dot(_coefs[id]) - pts[i].second);
        targetSqr += SQR(pts[i].second);
    }

    stringstream ss;
    ss.precision(3);
    ss.width(-5);
    ss << _coefs[id].transpose();

    printf("%-75s\t", ss.str().c_str());

    printf("Code = %d%d%d %d %d %d %d, num = %d, err = %lf, ratio = %lf\n", cur.continuity, cur.type[0], cur.type[1],
        cur.startCurvSign[0], cur.startCurvSign[1], cur.endCurvSign[0], cur.endCurvSign[1],
        cnt, err / double(cnt), err / targetSqr);
}

double DataModel::get(const IndependentValue &indep) const
{
    int id = indep.discreteId();
    if(id >= (int)_coefs.size() || _coefs[id].size() == 0)
        return 0.; //no data

    return _coefs[id].dot(indep.featureVector());
}

END_NAMESPACE_Cornu



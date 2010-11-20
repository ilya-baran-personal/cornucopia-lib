/*--
    Regression.h  

    This file is part of the Cornucopia curve sketching library.
    Copyright (C) 2010 Ilya Baran (baran37@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CORNUCOPIA_REGRESSION_H_INCLUDED
#define CORNUCOPIA_REGRESSION_H_INCLUDED

#include "defs.h"
#include "CurvePrimitive.h"
#include <vector>
#include <Eigen/StdVector>

NAMESPACE_Cornu

//represents the data we know about two curves that will have to be combined
struct IndependentValue
{
    //discrete part
    char continuity;
    char type[2];
    char startCurvSign[2];
    char endCurvSign[2];

    int discreteId() const;
    //sort them by their discrete id's
    bool operator<(const IndependentValue &other) const { return discreteId() < other.discreteId(); }

    //continuous part
    double length[2];
    Eigen::Vector3d diffs; //position, angle, curvature

    typedef Eigen::Matrix<double, Eigen::Dynamic, 1, 0, 6, 1> FeatureVector; //avoids dynamic memory allocation
    FeatureVector featureVector() const;
};

class Dataset : public smart_base
{
public:
    void addPoint(const IndependentValue &indep, double dep)
    {
        _data.push_back(std::make_pair(indep, dep));
    }

    void save(const std::string &filename) const;
    void load(const std::string &filename); //apends to existing data

    const std::vector<std::pair<IndependentValue, double> > &data() const { return _data; }

private:
    std::vector<std::pair<IndependentValue, double> > _data;
};
CORNU_SMART_TYPEDEFS(Dataset);

class DataModel : public smart_base
{
public:
    DataModel();
    DataModel(DatasetConstPtr data);

    double get(const IndependentValue &indep) const;

    const std::vector<Eigen::VectorXd> &getCoefs() const { return _coefs; }
private:
    void _computeCoefs(const std::vector<std::pair<IndependentValue, double> > &pts, int start, int cnt);
    std::vector<Eigen::VectorXd> _coefs; //maps the discrete id to the set of coefficients
};
CORNU_SMART_TYPEDEFS(DataModel);

END_NAMESPACE_Cornu

#endif //CORNUCOPIA_REGRESSION_H_INCLUDED

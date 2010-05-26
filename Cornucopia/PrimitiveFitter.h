/*--
    PrimitiveFitter.h  

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

#ifndef PRIMITIVEFITTER_H_INCLUDED
#define PRIMITIVEFITTER_H_INCLUDED

#include "defs.h"
#include "smart_ptr.h"
#include <vector>
#include <Eigen/Core>
#include <Eigen/StdVector>

NAMESPACE_Cornu

SMART_FORW_DECL(Line);
SMART_FORW_DECL(Arc);
SMART_FORW_DECL(Clothoid);

class LineFitter
{
public:
    typedef Eigen::Vector2d Vec;

    LineFitter() : _numPts(0), _totWeight(0.), _crossSum(0.),
        _firstPoint(Vec::Zero()), _lastPoint(Vec::Zero()), _sum(Vec::Zero()), _squaredSum(Vec::Zero()) {}

    void addPoint(const Eigen::Vector2d &pt, double weight = 1.);
    LinePtr getCurve() const;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
    Vec _firstPoint, _lastPoint;
    Vec _sum;
    Vec _squaredSum;
    int _numPts;
    double _totWeight;
    double _crossSum;
};

class ArcFitter
{
public:
    ArcFitter() : _totWeight(0.), _squaredSum(_squaredSum.Zero()), _sum(_sum.Zero()) {}

    void addPoint(const Eigen::Vector2d &pt, double weight = 1.);
    ArcPtr getCurve() const;

private:
    Eigen::Matrix3d _squaredSum;
    Eigen::Vector3d _sum;
    std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d> > _pts;
    double _totWeight;
};

//This fits a clothoid by fitting a cubic polynomial to the integral of the angle function,
//using its derivative as the angle function of the clothoid,
//and making the clothoid center of mass align with that of the input
class ClothoidFitter
{
public:
    ClothoidFitter() : _totalLength(0), _prevAngle(0), _angleIntegral(0),
                       _centerOfMass(_centerOfMass.Zero()), _rhs(_rhs.Zero()) {}

    void addPoint(const Eigen::Vector2d &pt);
    ClothoidPtr getCurve() const;

private:
    static Eigen::Matrix4d _getLhs(double x);
    static Eigen::Vector4d _getRhs(double x, double y, double z);

    Eigen::Vector2d _centerOfMass;
    Eigen::Vector4d _rhs;
    std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d> > _pts;
    double _prevAngle;
    double _angleIntegral;

    double _totalLength;
};

END_NAMESPACE_Cornu

#endif //PRIMITIVEFITTER_H_INCLUDED

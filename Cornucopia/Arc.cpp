/*--
    Arc.cpp  

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

#include "Arc.h"

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

Arc::Arc(const Vec &start, double startAngle, double length, double curvature)
{
    _params.resize(numParams());

    _params.head<2>() = start;
    _params[ANGLE] = AngleUtils::toRange(startAngle);
    _params[LENGTH] = length;
    _params[CURVATURE] = curvature;

    _paramsChanged();
}

void Arc::_paramsChanged()
{
    _tangent = Vec(cos(_startAngle()), sin(_startAngle()));

    _angleDiff = _length() * _params[CURVATURE];
    _flat = fabs(_params[CURVATURE]) < 1e-6;

    if(!_flat)
    {
        Vec toCenter(-_tangent[1], _tangent[0]);
        _radius = 1. / _params[CURVATURE];
        _center = _startPos() + _radius * toCenter;
    }
}

bool Arc::isValid() const
{
    if(_params[LENGTH] < 0.)
        return false;

    if(fabs(_angleDiff) > TWOPI)
        return false;

    return true;
}

void Arc::eval(double s, Vec *pos, Vec *der, Vec *der2) const
{
    double angle = _startAngle() + s * _params[CURVATURE];
    double cosa, sina;
    if(!_flat || der || der2)
    {
        cosa = cos(angle);
        sina = sin(angle);
    }

    if(_flat && pos)
        (*pos) = _startPos() + _tangent * s;
    else if(pos)
        (*pos) = _center + _radius * Vec(sina, -cosa);

    if(der)
        (*der) = Vec(cosa, sina);
    if(der2)
        (*der2) = Vec(-sina, cosa) * _params[CURVATURE];
}

double Arc::project(const Vec &point) const
{
    double t;
    if(_flat)
    {
        t = (point - _startPos()).dot(_tangent);
    }
    else
    {
        double projAngle = atan2(point[1] - _center[1], point[0] - _center[0]);
        //To compute the projection, get the angle difference into the range centered on the midpoint of
        //the arc.
        if(_params[CURVATURE] > 0.)
        {
            double projAngleDiff = HALFPI + projAngle - _startAngle();
            t = AngleUtils::toRange(projAngleDiff, 0.5 * _angleDiff - PI) / _params[CURVATURE];
        }
        else
        {
            double projAngleDiff = HALFPI - projAngle + _startAngle();
            t = AngleUtils::toRange(projAngleDiff, -0.5 * _angleDiff - PI) / -_params[CURVATURE];
        }
    }

    return max(0., min(_length(), t));
}

void Arc::trim(double sFrom, double sTo)
{
    Vec newStart = pos(sFrom);
    _params[ANGLE] = angle(sFrom);
    _params[LENGTH] = sTo - sFrom;
    _params[X] = newStart[0];
    _params[Y] = newStart[1];

    _paramsChanged();
}

void Arc::flip()
{
    Vec newStart = endPos();
    _params[ANGLE] = -endAngle();
    _params[CURVATURE] = -_params[CURVATURE];
    _params[X] = newStart[0];
    _params[Y] = newStart[1];

    _paramsChanged();
}

void Arc::derivativeAt(double s, ParamDer &out)
{
    out = ParamDer::Zero(5, 2);
    out(X, 0) = 1;
    out(Y, 1) = 1;

    Vec diff = pos(s) - _startPos();
    out(ANGLE, 0) = -diff[1];
    out(ANGLE, 1) = diff[0];

    if(_flat)
    {
        out.row(CURVATURE) = (0.5 * s * s) * Vec(-_tangent[1], _tangent[0]);
    }
    else
    {
        double angle = _startAngle() + s * _params[CURVATURE];

        double cosa, sina;
        cosa = cos(angle);
        sina = sin(angle);

        out(CURVATURE, 0) = (s * cosa + (_tangent[1] - sina) * _radius) * _radius;
        out(CURVATURE, 1) = (s * sina - (_tangent[0] - cosa) * _radius) * _radius;
    }
}

END_NAMESPACE_Cornu

/*--
    Line.cpp  

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

#include "Line.h"

using namespace std;
using namespace Eigen;

NAMESPACE_Cornu

Line::Line(const Vec &p1, const Vec &p2)
{
    _params.resize(numParams());

    _params.head<2>() = p1;
    Vec dir = p2 - p1;
    double len = dir.norm();

    if(fabs(len) < NumTraits<double>::dummy_precision()) //fail gracefully on zero-length line
    {
        _params[LENGTH] = 0;
        _params[ANGLE] = 0;
        _der = Vec(1., 0.);
    }
    else
    {
        _params[LENGTH] = len;
        _params[ANGLE] = AngleUtils::angle(dir);
        _der = dir * (1. / len);
    }
}

bool Line::isValidImpl() const
{
    if(_params[LENGTH] < 0.)
        return false;
    return true;
}

double Line::project(const Vec &point) const
{
    return min(_length(), max(0., _der.dot(point - _startPos())));
}

void Line::eval(double s, Vec *pos, Vec *der, Vec *der2) const
{
    if(pos)
        *pos = _startPos() + s * _der;
    if(der)
        *der = _der;
    if(der2)
        *der2 = Vec::Zero();
}

void Line::trim(double sFrom, double sTo)
{
    Vec newStart = _startPos() + sFrom * _der;
    _params[LENGTH] = sTo - sFrom;
    _params[X] = newStart[0];
    _params[Y] = newStart[1];
}

void Line::flip()
{
    Vec newStart = endPos();
    _params[ANGLE] = -_params[ANGLE];
    _der = -_der;
    _params[X] = newStart[0];
    _params[Y] = newStart[1];
}

void Line::derivativeAt(double s, ParamDer &out)
{
    out = ParamDer::Zero(4, 2);
    out(X, 0) = 1;
    out(Y, 1) = 1;
    out(ANGLE, 0) = -s * _der(1);
    out(ANGLE, 1) = s * _der(0);
}

END_NAMESPACE_Cornu



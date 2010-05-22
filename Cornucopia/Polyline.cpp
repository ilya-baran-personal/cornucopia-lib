/*--
    Polyline.cpp  

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

#include "Polyline.h"

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

Polyline::Polyline(const VectorC<Eigen::Vector2d> &pts) : _pts(pts)
{
    assert(_pts.size() > 1);
    _lengths.reserve(pts.size() + 1);
    _lengths.push_back(0);
    for(int i = 0; i < pts.endIdx(1); ++i)
        _lengths.push_back(_lengths.back() + (pts[i] - pts[i + 1]).norm());
}

int Polyline::paramToIdx(double param, double *outParam) const
{
    int idx = min(std::upper_bound(_lengths.begin(), _lengths.end(), param) - _lengths.begin(), (ptrdiff_t)_lengths.size() - 1) - 1;
    if(outParam)
        *outParam = param - _lengths[idx];
    return idx;
}

void Polyline::eval(double s, Vec *pos, Vec *der, Vec *der2) const
{
    if(_pts.circular())
        s = fmod(s + 10. * _lengths.back(), _lengths.back());
    double cParam;
    int idx = paramToIdx(s, &cParam);
    int nidx = (idx + 1) % _pts.size();
    double invLength = (1. / (_lengths[idx + 1] - _lengths[idx]));
    if(pos)
        (*pos) = _pts.flatAt(idx) + (cParam * invLength) * (_pts.flatAt(nidx) - _pts.flatAt(idx));
    if(der)
        (*der) = (_pts.flatAt(nidx) - _pts.flatAt(idx)) * invLength;
    if(der2)
        (*der2) = Vec();
}

double Polyline::project(const Vector2d &point) const
{
    double bestS = 0.;
    double minDistSq = (point - _pts[0]).squaredNorm();

    for(int i = 0; i < _pts.endIdx(1); ++i)
    {
        double len = (_lengths[i + 1] - _lengths[i]);
        double invLen = 1. / len;
        Vector2d der = (_pts[i + 1] - _pts[i]) * invLen;
        double dot = der.dot(point - _pts[i]);        
        if(dot < 0.)
            dot = 0.;
        if(dot > len)
            dot = len;

        Vector2d ptOnLine = _pts[i] + (_pts[i + 1] - _pts[i]) * (dot * invLen);
        double distSq = (ptOnLine - point).squaredNorm();
        if(distSq < minDistSq)
        {
            minDistSq = distSq;
            bestS = _lengths[i] + dot;
        }
    }

    return bestS;
}

END_NAMESPACE_Cornu



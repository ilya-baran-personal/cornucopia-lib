/*--
    PrimitiveSequence.cpp  

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

#include "PrimitiveSequence.h"

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

PrimitiveSequence::PrimitiveSequence(const VectorC<CurvePrimitiveConstPtr> &primitives) : _primitives(primitives)
{
    assert(_primitives.size() > 0);
    _lengths.resize(_primitives.size() + 1, 0);
    for(int i = 0; i < (int)_primitives.size(); ++i)
        _lengths[i + 1] = _lengths[i] + _primitives[i]->length();
}

int PrimitiveSequence::paramToIdx(double param, double *outParam) const
{
    int idx = (int)min(std::upper_bound(_lengths.begin(), _lengths.end(), param) - _lengths.begin(), (ptrdiff_t)_lengths.size() - 1) - 1;
    if(outParam)
        *outParam = param - _lengths[idx];
    return idx;
}

void PrimitiveSequence::eval(double s, Vec *pos, Vec *der, Vec *der2) const
{
    if(_primitives.circular())
    {
        s = fmod(s, _lengths.back());
        if(s < 0.)
            s += _lengths.back();
    }
    double cParam;
    int idx = paramToIdx(s, &cParam);
    _primitives[idx]->eval(cParam, pos, der, der2);
}

double PrimitiveSequence::project(const Vector2d &point) const
{
    double bestS = 0.;
    double minDistSq = 1e50;

    for(int i = 0; i < _primitives.size(); ++i)
    {
        double localS = _primitives[i]->project(point);
        Vector2d pt = _primitives[i]->pos(localS);
        double distSq = (pt - point).squaredNorm();
        if(distSq < minDistSq)
        {
            minDistSq = distSq;
            bestS = _lengths[i] + localS;
        }
    }

    return bestS;
}

END_NAMESPACE_Cornu



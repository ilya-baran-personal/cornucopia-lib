/*--
    ClothoidProjector.cpp  

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

#include "Clothoid.h"
#include "Arc.h"
#include "Fresnel.h"

#include <deque>

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

//an arc that approximates a part of a clothoid
class _ApproxArc
{
public:
    _ApproxArc(double start, double length)
        : _start(start), _length(length)
    {
        //build an arc through three points on the clothoid to approximate it
        Vector2d p[3];
        for(int i = 0; i < 3; ++i)
            fresnel(start + 0.5 * length * i, &(p[i][1]), &(p[i][0]));
        _arc = new Arc(p[0], p[1], p[2]);
    }

    _ApproxArc(double start, double length, const Vector2d &startPt, const Vector2d &endPt)
    {
        //TODO
    }

    //TODO: optimize -- no need to always call project
    bool test(const Vector2d &pt, double &minDistSq, double &minT, double from, double to) const
    {
        double t = _arc->project(pt);
        if(t + _start < from || t + _start > to)
            return false;

        double distSq = (pt - _arc->pos(t)).squaredNorm();
        if(distSq >= minDistSq)
            return false;

        minT = _start + t;
        minDistSq = distSq;
        return true;
    }

//private:
    ArcPtr _arc;
    double _start;
    double _length;
};

class Clothoid::_ClothoidProjectorImpl : public Clothoid::_ClothoidProjector
{
public:
    typedef Vector2d Vec;

    _ClothoidProjectorImpl() //initialize
        : _arcSpacing(0.1)
    {
        double t;
        for(t = 0; t * _arcSpacing < 0.9; t += _arcSpacing)
        {
            _arcs.push_front(_ApproxArc(-t - _arcSpacing, _arcSpacing));
            _arcs.push_back(_ApproxArc(t, _arcSpacing));
        }
        _maxArcParam = t;
    }

    double project(const Vec &pt, double from, double to) const
    {
        //return 0;

        double minT, minDistSq;
#if 0
        Vec minPt;
        for(int i = 0; i <= 20; ++i)
        {
            double t = from + double(i) / 20. * (to - from);
            Vec pos;
            eval(t, &pos);
            double distSq = (pos - pt).squaredNorm();
            if(i == 0 || distSq < minDistSq)
            {
                minT = t;
                minPt = pos;
                minDistSq = distSq;
            }
        }
#elif 0
        _ApproxArc arc(from, to - from);
        minT = arc.project(pt);
#else
        Vector2d start, end;
        fresnelApprox(from, &(start[1]), &(start[0]));
        fresnelApprox(to, &(end[1]), &(end[0]));

        minT = from;
        minDistSq = (pt - start).squaredNorm();
        double distSq = (pt - end).squaredNorm();
        if(distSq < minDistSq)
        {
            minDistSq = distSq;
            minT = to;
        }

        int minArcIdx = (int)floor((_maxArcParam + from) / _arcSpacing);
        if(minArcIdx < 0)
        {
            minArcIdx = 0;
            //TODO: test against new first arc
        }
        int maxArcIdx = (int)ceil((_maxArcParam + to) / _arcSpacing);
        if(maxArcIdx > (int)_arcs.size())
        {
            maxArcIdx = (int)_arcs.size();
            //TODO: test against new last arc
        }

        for(int i = minArcIdx; i < maxArcIdx; ++i)
            _arcs[i].test(pt, minDistSq, minT, from, to);

        //TODO: get rid of this debugging crap, once the method is complete and tested
        if(minT < from || minT > to)
            Debugging::get()->printf("Off: (%lf %lf) %lf", from, to, minT);

        if(minArcIdx != (int)floor((_maxArcParam + from) / _arcSpacing))
            Debugging::get()->printf("Out of bounds low!");
        if(maxArcIdx != (int)ceil((_maxArcParam + to) / _arcSpacing))
            Debugging::get()->printf("Out of bounds high!");
#endif

        minT = projectNewton(minT, pt, from, to);
        //minT = projectNewton(minT, pt, from, to);
        //minT = projectNewton(minT, pt, from, to);
        return minT;
    }

private:
    double projectNewton(double guess, const Vec &pt, double from, double to) const
    {
        Vec p, der, der2;
        eval(guess, &p, &der, &der2);

        double dot = der.dot(pt - p);
        double dotDer = der2.dot(pt - p) - der.squaredNorm();

        if(dotDer >= -1e-30) //if distance will increase
            return guess;

        return max(from, min(to, guess - dot / dotDer));
    }

    void eval(double t, Vec *pos, Vec *der = NULL, Vec *der2 = NULL) const
    {
        if(pos)
            fresnelApprox(t, &((*pos)[1]), &((*pos)[0]));
        if(der || der2)
        {
            double s = sin(HALFPI * t * t), c = cos(HALFPI * t * t);
            if(der)
                *der = Vec(c, s);
            if(der2)
                *der2 = (PI * t) * Vec(-s, c);
        }
    }

    const double _arcSpacing;
    deque<_ApproxArc> _arcs;
    double _maxArcParam;
};

smart_ptr<Clothoid::_ClothoidProjector> Clothoid::_clothoidProjector()
{
    static smart_ptr<Clothoid::_ClothoidProjector> projector = new _ClothoidProjectorImpl();
    return projector;
}

END_NAMESPACE_Cornu



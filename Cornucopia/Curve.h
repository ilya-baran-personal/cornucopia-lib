/*--
    Curve.h  

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

#ifndef CURVE_H_INCLUDED
#define CURVE_H_INCLUDED

#include "defs.h"
#include "smart_ptr.h"
#include "AngleUtils.h"

NAMESPACE_Cornu

/*
    Represents a general curve.  Curves are arclength parameterized from 0 to length().
*/
class Curve : public smart_base
{
protected:
    typedef Eigen::Vector2d Vec;

public:
    virtual double length() const = 0;

    virtual bool isClosed() const { return false; }

    //evaluates the curve with optionally the first and second derivatives (tangent and curvature)
    virtual void eval(double s, Vec *pos, Vec *der = NULL, Vec *der2 = NULL) const = 0;

    virtual double project(const Vec &point) const = 0;

    //derived evaluation functions--subclasses can implement them more efficiently
    virtual Vec pos(double s) const { Vec out; eval(s, &out); return out; }
    virtual Vec der(double s) const { Vec out; eval(s, NULL, &out); return out; }
    virtual Vec der2(double s) const { Vec out; eval(s, NULL, NULL, &out); return out; }

    virtual double angle(double s) const { return AngleUtils::angle(der(s)); }
    virtual double curvature(double s) const { Vec d, d2; eval(s, NULL, &d, &d2); return d.dot(Vec(d2[1], -d2[0])); }

    virtual Vec startPos() const { return pos(0); }
    virtual Vec endPos() const { return pos(length()); }
    virtual double startAngle() const { return angle(0); }
    virtual double endAngle() const { return angle(length()); }
    virtual double startCurvature() const { return curvature(0); }
    virtual double endCurvature() const { return curvature(length()); }
};

END_NAMESPACE_Cornu

#endif //CURVE_H_INCLUDED

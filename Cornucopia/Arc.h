/*--
    Arc.h  

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

#ifndef ARC_H_INCLUDED
#define ARC_H_INCLUDED

#include "defs.h"
#include "CurvePrimitive.h"

NAMESPACE_Cornu

SMART_FORW_DECL(Arc);

class Arc : public CurvePrimitive
{
public:
    Arc() {} //uninitialized
    Arc(const Vec &start, double startAngle, double length, double curvature);

    //overrides
    bool isValid() const;

    void eval(double s, Vec *pos, Vec *der = NULL, Vec *der2 = NULL) const;

    double project(const Vec &point) const;

    //Vec pos(double s) const;
    //Vec der(double s) const;
    //Vec der2(double s) const;

    double angle(double s) const { return _startAngle() + s * _params[CURVATURE]; }
    double evalCurvature(double s) const { return _params[CURVATURE]; }

    double endAngle() const { return _startAngle() + _angleDiff; }
    double startCurvature() const { return _params[CURVATURE]; }
    double endCurvature() const { return _params[CURVATURE]; }

    PrimitiveType getType() const { return ARC; }

    void trim(double sFrom, double sTo);
    void flip();
    CurvePrimitivePtr clone() const { ArcPtr out = new Arc(); out->setParams(_params); return out; }
    void derivativeAt(double s, ParamDer &out);

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
protected:
    //override
    void _paramsChanged();

private:
    Vec _tangent; //at start
    Vec _center; //if arc is not flat
    double _radius; //if arc is not flat, 1 / curvature
    double _angleDiff; //length * curvature
    bool _flat; //if true, arc is almost flat and we should use an approximation
};

END_NAMESPACE_Cornu

#endif //ARC_H_INCLUDED

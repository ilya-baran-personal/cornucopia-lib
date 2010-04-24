/*--
    Clothoid.h  

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

#ifndef CLOTHOID_H_INCLUDED
#define CLOTHOID_H_INCLUDED

#include "defs.h"
#include "CurvePrimitive.h"

NAMESPACE_Cornu

SMART_FORW_DECL(Clothoid);

class Clothoid : public CurvePrimitive
{
public:
    Clothoid() {} //uninitialized
    Clothoid(const Vec &start, double startAngle, double length, double curvature, double endCurvature);

    //overrides
    bool isValid() const;

    void eval(double s, Vec *pos, Vec *der = NULL, Vec *der2 = NULL) const;

    double project(const Vec &point) const;

    double angle(double s) const;
    double curvature(double s) const;

    double startCurvature() const { return _params[CURVATURE]; }
    double endCurvature() const { return _params[CURVATURE] + _params[LENGTH]* _params[DCURVATURE]; }

    PrimitiveType getType() const { return CLOTHOID; }

    void trim(double sFrom, double sTo);
    void flip();
    CurvePrimitivePtr clone() const { ClothoidPtr out = new Clothoid(); out->setParams(_params); return out; }
    void derivativeAt(double s, ParamDer &out);

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
protected:
    //override
    void _paramsChanged();

private:
    Vec _startShift; //translation component of transformation from canonical clothoid
    Eigen::Matrix2d _mat; //rotation and scale component of transformation from canonical clothoid
    double _t1; //start parameter on the canonical clothoid
    double _tdiff;
    bool _arc;
    bool _flat;
};

END_NAMESPACE_Cornu

#endif //CLOTHOID_H_INCLUDED

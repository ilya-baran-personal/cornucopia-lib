/*--
    Polyline.h  

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

#ifndef POLYLINE_H_INCLUDED
#define POLYLINE_H_INCLUDED

#include "defs.h"
#include "Curve.h"
#include "VectorC.h"

NAMESPACE_Cornu

SMART_FORW_DECL(Polyline);

//A polyline must have at least two points
class Polyline : public Curve
{
public:
    Polyline(const VectorC<Eigen::Vector2d> &pts);

    //overrides
    double length() const { return _lengths.back(); }
    bool isClosed() const { return _pts.circular() == CIRCULAR; }

    void eval(double s, Vec *pos, Vec *der = NULL, Vec *der2 = NULL) const;

    double project(const Vec &point) const;

    //utility functions
    int paramToIdx(double param, double *outParam = NULL) const;
    double idxToParam(int idx) const { return _lengths[idx]; }

    const VectorC<Eigen::Vector2d> &pts() const { return _pts; }

private:
    VectorC<Eigen::Vector2d> _pts;
    //lengths[x] = \sum_{i=1}^{i=x} ||pts[i]-pts[i-1]||, i.e., length up to point x
    //For closed curves, _lengths has a last member which is the total curve length.
    std::vector<double> _lengths; 
};

END_NAMESPACE_Cornu

#endif //POLYLINE_H_INCLUDED

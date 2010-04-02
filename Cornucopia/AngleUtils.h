/*--
    AngleUtils.h  

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

#ifndef ANGLEUTILS_H_INCLUDED
#define ANGLEUTILS_H_INCLUDED

#include "defs.h"

#include <Eigen/Core>

NAMESPACE_Cornu

/*
    This class has a bunch of static methods for working with angles
*/
class AngleUtils
{
public:
    static double angle(const Eigen::Vector2d &v) { return atan2(v[1], v[0]); }
};

END_NAMESPACE_Cornu

#endif //ANGLEUTILS_H_INCLUDED

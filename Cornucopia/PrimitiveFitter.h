/*--
    PrimitiveFitter.h  

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

#ifndef PRIMITIVEFITTER_H_INCLUDED
#define PRIMITIVEFITTER_H_INCLUDED

#include "defs.h"
#include "Algorithm.h"

NAMESPACE_Cornu

SMART_FORW_DECL(CurvePrimitive);

struct FitPrimitive
{
    CurvePrimitivePtr curve;
    int startIdx;
    int endIdx;
    double error;
    int startCurvSign;
    int endCurvSign;
};

template<>
struct AlgorithmOutput<PRIMITIVE_FITTING> : public AlgorithmOutputBase
{
    std::vector<FitPrimitive> primitives;
};

template<>
class Algorithm<PRIMITIVE_FITTING> : public AlgorithmBaseTemplate<PRIMITIVE_FITTING>
{
public:
    //override
    std::string stageName() const { return "Primitive Fitter"; }

private:
    friend class AlgorithmBase;
    static void _initialize();
};

END_NAMESPACE_Cornu

#endif //PRIMITIVEFITTER_H_INCLUDED

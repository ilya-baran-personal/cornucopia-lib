/*--
    TwoCurveCombine.cpp  

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

#include "TwoCurveCombine.h"
#include "Resampler.h"
#include "PrimitiveFitter.h"
#include "Fitter.h"
#include "Polyline.h"

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

Combination twoCurveCombine(int p1, int p2, int continuity, const Fitter &fitter)
{
    const vector<FitPrimitive> &primitives = fitter.output<PRIMITIVE_FITTING>()->primitives;
    const VectorC<Vector2d> &pts = fitter.output<RESAMPLING>()->output->pts();

    Combination out;

    return out;
}

END_NAMESPACE_Cornu



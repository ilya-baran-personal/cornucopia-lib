/*--
    Preprocessing.cpp  

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

#include "Preprocessing.h"
#include "Fitter.h"
#include "Polyline.h"
#include "Debugging.h"
#include <iostream>

#include "Eigen/Geometry"

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

class NoScaleDetector : public Algorithm<SCALE_DETECTION>
{
public:
    string name() const { return "None"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<SCALE_DETECTION> &out)
    {
        out.scale = 1.;
    }
};

class AdaptiveScaleDetector : public Algorithm<SCALE_DETECTION>
{
public:
    string name() const { return "Adaptive"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<SCALE_DETECTION> &out)
    {
        double pixel = fitter.params().get(Parameters::PIXEL_SIZE);

        //compute the diagonal length of the bounding box of the curve
        const VectorC<Vector2d> &pts = fitter.originalSketch()->pts();
        AlignedBox<double, 2> boundingBox(pts[0]);
        for(int i = 1; i < (int)pts.size(); ++i)
            boundingBox.extend(pts[i]);
        double diag = boundingBox.diagonal().norm();

        const double smallCurvePixels = fitter.params().get(Parameters::SMALL_CURVE_PIXELS);
        const double largeCurvePixels = fitter.params().get(Parameters::LARGE_CURVE_PIXELS);
        const double maxRescale = fitter.params().get(Parameters::MAX_RESCALE);

        out.scale = 1.;

        if(diag < pixel * smallCurvePixels)
            out.scale = 1. / max(1. / maxRescale, diag / (pixel * smallCurvePixels));
        if(diag > pixel * largeCurvePixels)
            out.scale = 1. / min(maxRescale, diag / (pixel * largeCurvePixels));
    }
};

void Algorithm<SCALE_DETECTION>::_initialize()
{
    new AdaptiveScaleDetector();
    new NoScaleDetector();
};

class DefaultCurveCloser : public Algorithm<CURVE_CLOSING>
{
public:
    string name() const { return "Default"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<CURVE_CLOSING> &out)
    {
        out.output = fitter.originalSketch();
        //Debugging::get()->d
    }
};

void Algorithm<CURVE_CLOSING>::_initialize()
{
    new DefaultCurveCloser();
}

END_NAMESPACE_Cornu



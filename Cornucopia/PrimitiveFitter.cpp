/*--
    PrimitiveFitter.cpp  

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

#include "PrimitiveFitter.h"
#include "Resampler.h"
#include "Fitter.h"
#include "Polyline.h"
#include "PrimitiveFitUtils.h"

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

class DefaultPrimitiveFitter : public Algorithm<PRIMITIVE_FITTING>
{
public:
    string name() const { return "Default"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<PRIMITIVE_FITTING> &out)
    {
        const VectorC<bool> &corners = fitter.output<RESAMPLING>()->corners;
        PolylineConstPtr poly = fitter.output<RESAMPLING>()->output;
        const VectorC<Vector2d> &pts = poly->pts();

        const double errorThreshold = fitter.scaledParameter(Parameters::ERROR_THRESHOLD);

        for(int i = 0; i < pts.size(); ++i) //iterate over start points
        {
            FitterBasePtr fitters[3];
            fitters[0] = new LineFitter();
            fitters[1] = new ArcFitter();
            fitters[2] = new ClothoidFitter();
            std::string typeNames[3] = { "Lines", "Arcs", "Clothoids" };

            for(int type = 0; type <= 2; ++type)
            {
                int fitSoFar = 0;

                bool needType = fitter.params().get(Parameters::ParameterType(Parameters::LINE_COST + type)) < Parameters::infinity;

                for(VectorC<Vector2d>::Circulator circ = pts.circulator(i); !circ.done(); ++circ)
                {
                    ++fitSoFar;

                    if(!needType && (type == 2 || fitSoFar >= 3 + type)) //if we don't need primitives of this type
                        break;

                    fitters[type]->addPoint(*circ);
                    if(fitSoFar >= 2 + type) //at least two points per line, etc.
                    {
                        CurvePrimitivePtr curve = fitters[type]->getPrimitive();
                        Vector3d color(0, 0, 0);
                        color[type] = 1;
                        Debugging::get()->drawCurve(curve, color, typeNames[type]);

                        FitPrimitive fit;
                        fit.curve = curve;
                        fit.startIdx = i;
                        fit.endIdx = circ.index();
                        fit.error = computeError(fitter, curve, i, fit.endIdx);

                        double length = poly->lengthFromTo(i, fit.endIdx);
                        if(fit.error / length > errorThreshold * errorThreshold)
                            break;

                        out.primitives.push_back(fit);

                        //TODO: different start and end curvatures
                    }
                    if(fitSoFar > 1 && corners[circ.index()])
                        break;
                }
            }
        }
    }

    double computeError(const Fitter &fitter, CurvePrimitiveConstPtr curve, int from, int to)
    {
        PolylineConstPtr poly = fitter.output<RESAMPLING>()->output;
        const VectorC<Vector2d> &pts = poly->pts();

        double error = 0;

        bool first = true;
        for(VectorC<Vector2d>::Circulator circ = pts.circulator(from); ; ++circ)
        {
            bool last = circ.index() == to;

            double dist = (curve->pos(curve->project(*circ)) - *circ).squaredNorm();

            VectorC<Vector2d>::Circulator prev = circ;
            if(!first)
                --prev;

            VectorC<Vector2d>::Circulator next = circ;
            if(!last)
                ++next;

            double weight = poly->lengthFromTo(prev.index(), next.index()) * 0.5;

            error += weight * dist;

            first = false;
            if(last)
                break;
        }

        return error;
    }
};

void Algorithm<PRIMITIVE_FITTING>::_initialize()
{
    new DefaultPrimitiveFitter();
}

END_NAMESPACE_Cornu



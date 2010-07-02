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
#include "ErrorComputer.h"

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
        ErrorComputerConstPtr errorComputer = fitter.output<ERROR_COMPUTER>()->errorComputer;

        const VectorC<Vector2d> &pts = poly->pts();

        const double errorThreshold = fitter.scaledParameter(Parameters::ERROR_THRESHOLD);
        std::string typeNames[3] = { "Lines", "Arcs", "Clothoids" };
        bool inflectionAccounting = fitter.params().get(Parameters::INFLECTION_COST) > 0.;

        for(int i = 0; i < pts.size(); ++i) //iterate over start points
        {
            FitterBasePtr fitters[3];
            fitters[0] = new LineFitter();
            fitters[1] = new ArcFitter();
            fitters[2] = new ClothoidFitter();

            for(int type = 0; type <= 2; ++type) //iterate over lines, arcs, clothoids
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

                        FitPrimitive fit;
                        fit.curve = curve;
                        fit.startIdx = i;
                        fit.endIdx = circ.index();
                        fit.numPts = fitSoFar;
                        fit.error = errorComputer->computeError(curve, i, fit.endIdx);
                        fit.startCurvSign = (curve->startCurvature() >= 0) ? 1 : -1;
                        fit.endCurvSign = (curve->endCurvature() >= 0) ? 1 : -1;

                        double length = poly->lengthFromTo(i, fit.endIdx);
                        if(fit.error / length > errorThreshold * errorThreshold)
                            break;

                        Debugging::get()->drawCurve(curve, color, typeNames[type]);
                        out.primitives.push_back(fit);

                        if(type == 0 && inflectionAccounting) //line with "opposite" curvature
                        {
                            fit.startCurvSign = -fit.startCurvSign;
                            fit.endCurvSign = -fit.endCurvSign;
                            out.primitives.push_back(fit);
                        }

                        //if different start and end curvatures
                        if(fit.startCurvSign != fit.endCurvSign && inflectionAccounting)
                        {
                            double start = poly->idxToParam(i);
                            double end = poly->idxToParam(fit.endIdx);
                            CurvePrimitivePtr startNoCurv = static_pointer_cast<ClothoidFitter>(fitters[2])->getCurveWithZeroCurvature(0);
                            CurvePrimitivePtr endNoCurv = static_pointer_cast<ClothoidFitter>(fitters[2])->getCurveWithZeroCurvature(end - start);

                            fit.curve = startNoCurv;
                            fit.startCurvSign = fit.endCurvSign = (startNoCurv->endCurvature() > 0. ? 1 : -1);
                            fit.error = errorComputer->computeError(fit.curve, i, fit.endIdx);

                            if(fit.error / length < errorThreshold * errorThreshold)
                            {
                                out.primitives.push_back(fit);
                                Debugging::get()->drawCurve(fit.curve, color, typeNames[type]);
                            }

                            fit.curve = endNoCurv;
                            fit.startCurvSign = fit.endCurvSign = (endNoCurv->startCurvature() > 0. ? 1 : -1);
                            fit.error = errorComputer->computeError(fit.curve, i, fit.endIdx);

                            if(fit.error / length < errorThreshold * errorThreshold)
                            {
                                out.primitives.push_back(fit);
                                Debugging::get()->drawCurve(fit.curve, color, typeNames[type]);
                            }
                        }
                    }
                    if(fitSoFar > 1 && corners[circ.index()])
                        break;
                }
            }
        }
    }
};

void Algorithm<PRIMITIVE_FITTING>::_initialize()
{
    new DefaultPrimitiveFitter();
}

END_NAMESPACE_Cornu



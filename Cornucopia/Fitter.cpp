/*--
    Fitter.cpp  

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

#include "Fitter.h"
#include "Preprocessing.h"
#include "Polyline.h"
#include "Resampler.h"

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

void Fitter::run()
{
    Debugging::get()->clear();
    Debugging::get()->printf("Starting...");
    Debugging::get()->drawCurve(_originalSketch, Vector3d(0, 0, 0), "Original Sketch", 2., Debugging::DOTTED);
    Debugging::get()->startTiming("Total");

    for(int i = 0; i < NUM_ALGORITHM_STAGES; ++i)
    {
        if(!(_outputs[i]))
        {
            std::string stageName = AlgorithmBase::get((AlgorithmStage)i, 0)->stageName();
            Debugging::get()->startTiming(stageName);
            _runStage((AlgorithmStage)i);
            if(Debugging::get()->getTimeElapsed(stageName) > 0.001) //only print significant times
                Debugging::get()->elapsedTime(stageName);
        }
    }
    Debugging::get()->elapsedTime("Total");
}

void Fitter::_runStage(AlgorithmStage stage)
{
    _outputs[stage] = AlgorithmBase::get(stage, _params.getAlgorithm(stage))->run(*this);
}

void Fitter::_clearBefore(AlgorithmStage stage)
{
    for(int i = stage; i < NUM_ALGORITHM_STAGES; ++i)
        _outputs[i] = AlgorithmOutputBasePtr();
}

double Fitter::scaledParameter(Parameters::ParameterType param) const
{
    return _params.get(param) * output<SCALE_DETECTION>()->scale * _params.get(Parameters::PIXEL_SIZE);
}

CurveConstPtr Fitter::finalOutput() const
{
    return output<RESAMPLING>()->output; //TODO
}

END_NAMESPACE_Cornu



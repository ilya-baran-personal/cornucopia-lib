/*--
    Fitter.h  

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

#ifndef FITTER_H_INCLUDED
#define FITTER_H_INCLUDED

#include "defs.h"
#include "Parameters.h"
#include "Algorithm.h"

NAMESPACE_Cornu

SMART_FORW_DECL(Polyline);

class Fitter
{
public:
    Fitter() : _outputs(NUM_ALGORITHM_STAGES) {}

    const Parameters &params() const { return _params; }
    void setParams(const Parameters &params) { _params = params; _clearBefore(SCALE_DETECTION); }

    PolylineConstPtr originalSketch() const { return _originalSketch; }
    void setOriginalSketch(PolylineConstPtr originalSketch) { _originalSketch = originalSketch; _clearBefore(SCALE_DETECTION); }

    template<int AlgStage>
    smart_ptr<const AlgorithmOutput<AlgStage> > output() const
    {
        return static_pointer_cast<const AlgorithmOutput<AlgStage> >(_outputs[AlgStage]);
    }

    void run();

    double scaledParameter(Parameters::ParameterType param) const;

private:
    void _runStage(AlgorithmStage stage);
    void _clearBefore(AlgorithmStage stage);

    PolylineConstPtr _originalSketch;
    Parameters _params;

    std::vector<AlgorithmOutputBasePtr> _outputs;
};

END_NAMESPACE_Cornu

#endif //FITTER_H_INCLUDED

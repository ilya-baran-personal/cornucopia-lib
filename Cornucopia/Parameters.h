/*--
    Parameters.h  

    This file is part of the Cornucopia curve sketching library.
    Copyright (C) 2010 Ilya Baran (baran37@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PARAMETERS_H_INCLUDED
#define PARAMETERS_H_INCLUDED

#include "defs.h"

#include <vector>
#include <string>

NAMESPACE_Cornu

class Parameters
{
public:
    enum ParameterType
    {
        LINE_COST,
        ARC_COST,
        CLOTHOID_COST,
        G0_COST,
        G1_COST,
        G2_COST,
        ERROR_COST,
        SHORTNESS_COST,
        INFLECTION_COST,
        //This is not a real parameter--below are parameters the user should not control
        INTERNAL_PARAMETERS_MARKER, 
        PIXEL_SIZE,
        SMALL_CURVE_PIXELS,
        LARGE_CURVE_PIXELS,
        MAX_RESCALE,
        MIN_PRELIM_LENGTH,
        DP_CUTOFF,
        CLOSEDNESS_THRESHOLD,
        MINIMUM_CORNER_SPACING,
        CORNER_NEIGHBORHOOD,
        DENSE_SAMPLING_STEP,
        CORNER_SCALES,
        CORNER_THRESHOLD,
        MAX_SAMPLING_INTERVAL,
        CURVATURE_ESTIMATE_REGION,
        POINTS_PER_CIRCLE,
        MAX_SAMPLE_RATE_SLOPE,
        ERROR_THRESHOLD,
        SHORTNESS_THRESHOLD,
        TWO_CURVE_CURVATURE_ADJUST,
        CURVE_ADJUST_DAMPING,
        REDUCE_GRAPH_EVERY,
        COMBINE_DAMPING
    };

    Parameters(const std::string &name = std::string());
    Parameters(const Parameters &parameters) : _name(parameters._name), _values(parameters._values), _algorithms(parameters._algorithms) {}

    void set(ParameterType param, double val) { _values[param] = val; }
    double get(ParameterType param) const { return _values[param]; }
    void setAlgorithm(int stage, int algorithm) { _algorithms[stage] = algorithm; }
    int getAlgorithm(int stage) const { return _algorithms[stage]; }

    const std::string &name() const { return _name; } 
    void setName(const std::string &name) { _name = name; }

    bool operator==(const Parameters &other) const { return _values == other._values && _algorithms == other._algorithms; }
    bool operator!=(const Parameters &other) const { return !((*this) == other); }

private:
    std::string _name;
    std::vector<double> _values;
    std::vector<int> _algorithms;

    //===== static description data =====
public:
    struct Parameter
    {
        Parameter(ParameterType inType, const std::string &inName, double inMin, double inMax, double inDefault, bool inInfinityAllowed = true)
            : type(inType), typeName(inName), min(inMin), max(inMax), defaultVal(inDefault), infinityAllowed(inInfinityAllowed) {}
        Parameter(ParameterType inType, const std::string &inName, double inValue) //for internal parameters
            : type(inType), typeName(inName), min(inValue), max(inValue), defaultVal(inValue), infinityAllowed(false) {}

        ParameterType type;
        std::string typeName;
        double min, max, defaultVal;
        bool infinityAllowed;
    };

    static const double infinity;

    static const std::vector<Parameter> &parameters() { _initializeParameters(); return _parameters; }
    static const std::vector<Parameters> &presets() { _initializePresets(); return _presets; }

private:
    static void _initializeParameters(); 
    static void _initializePresets(); 
    static std::vector<Parameter> _parameters;
    static std::vector<Parameters> _presets;
};

END_NAMESPACE_Cornu

#endif //PARAMETERS_H_INCLUDED

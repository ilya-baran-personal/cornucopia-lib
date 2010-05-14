/*--
    Parameters.h  

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
        INFLECTION_COST,
        INTERNAL_PARAMETERS_MARKER, //This is not a real parameter--below are parameters the user should not control
        PIXEL_SIZE,
        CLOSEDNESS_THRESHOLD
    };

    Parameters(const std::string &name = std::string());
    Parameters(const Parameters &parameters) : _name(parameters._name), _values(parameters._values) {}

    void set(ParameterType param, double val) { _values[param] = val; }
    double get(ParameterType param) const { return _values[param]; }
    const std::string &name() const { return _name; } 

    bool operator==(const Parameters &other) const { return _values == other._values; }
    bool operator!=(const Parameters &other) const { return !((*this) == other); }

private:
    std::string _name;
    std::vector<double> _values;

    //===== static description data =====
public:
    struct Parameter
    {
        Parameter(ParameterType inType, const std::string &inName, double inMin, double inMax, double inDefault)
            : type(inType), typeName(inName), min(inMin), max(inMax), defaultVal(inDefault) {}
        Parameter(ParameterType inType, double inValue) //for internal parameters
            : type(inType), min(inValue), max(inValue), defaultVal(inValue) {}

        ParameterType type;
        std::string typeName;
        double min, max, defaultVal;
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

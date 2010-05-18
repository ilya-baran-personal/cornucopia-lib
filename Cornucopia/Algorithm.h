/*--
    Algorithm.h  

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

#ifndef ALGORITHM_H_INCLUDED
#define ALGORITHM_H_INCLUDED

#include "defs.h"
#include "smart_ptr.h"
#include <vector>

NAMESPACE_Cornu

//This header file defines the stages and base classes for algorithms in the fitting pipeline.
//Fitting proceeds in stages, each one having access to the outputs of all the previous stages
//through the Fitter class.

enum AlgorithmStage
{
    SCALE_DETECTION,
    CURVE_CLOSING,
    CORNER_DETECTION,
    //RESAMPLING,
    NUM_ALGORITHM_TYPES
};

struct AlgorithmOutputBase : public smart_base
{
};

SMART_TYPEDEFS(AlgorithmOutputBase);

template<int AlgStage>
struct AlgorithmOutput : public AlgorithmOutputBase
{
};

class Fitter;

template<int AlgStage>
class Algorithm
{
};

class AlgorithmBase
{
public:
    virtual std::string name() const { return "Default"; }
    virtual std::string stageName() const = 0;
    virtual AlgorithmOutputBasePtr run(const Fitter &) = 0;

    static int numAlgorithmsOfType(AlgorithmStage stage) { return _algorithms[stage].size(); }
    static AlgorithmBase *get(AlgorithmStage stage, int algorithm) { return _algorithms[stage][algorithm]; }

protected:
    static std::vector<std::vector<AlgorithmBase *> > _algorithms;
};

template<int AlgStage>
class AlgorithmBaseTemplate : public AlgorithmBase
{
public:

    //override
    AlgorithmOutputBasePtr run(const Fitter &fitter)
    {
        smart_ptr<AlgorithmOutput<AlgStage> > out = new AlgorithmOutput<AlgStage>();
        _run(fitter, *out);
        return out;
    }

    static Algorithm<AlgStage> *get(int algorithm)
    {
        Algorithm<AlgStage>::_initialize();
        return static_cast<Algorithm<AlgStage> *>(_algorithms[AlgStage][algorithm]);
    }

    static std::vector<std::string> names()
    {
        _initialize();

        std::vector<std::string> out;
        for(int i = 0; i < (int)_algorithms.size(); ++i)
            out.push_back(_algorithms[AlgStage][i]->name());

        return out;
    }

protected:
    virtual void _run(const Fitter &fitter, AlgorithmOutput<AlgStage> &out) = 0;

    //should only be called by _initializePrivate()
    static void _addAlgorithm(Algorithm<AlgStage> *algorithm) { _algorithms[AlgStage].push_back(algorithm); }

private:
    static void _initialize()
    {
        static bool initialized = false;
        if(!initialized)
        {
            initialized = true;
            Derived::_initializePrivate(); //derived classes should implement this
        }
    }
};

END_NAMESPACE_Cornu

#endif //ALGORITHM_H_INCLUDED

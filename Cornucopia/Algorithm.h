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
#include <vector>

NAMESPACE_Cornu

template<class Derived>
class Algorithm
{
public:
    virtual std::string name() = 0;

    static Derived *get(int algorithm)
    {
        _initialize();
        return _algorithms[algorithm];
    }

    static std::vector<std::string> names()
    {
        _initialize();

        std::vector<std::string> out;
        for(int i = 0; i < (int)_algorithms.size(); ++i)
            out.push_back(_algorithms[i]->name());

        return out;
    }

protected:
    static void _addAlgorithm(Derived *algorithm) { _algorithms.push_back(algorithm); }

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

    static std::vector<Derived *> _algorithms;
};

template<class Derived> std::vector<Derived *> Algorithm<Derived>::_algorithms;

END_NAMESPACE_Cornu

#endif //ALGORITHM_H_INCLUDED

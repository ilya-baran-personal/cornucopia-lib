/*--
    VectorC.h  

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

#ifndef VECTORC_H_INCLUDED
#define VECTORC_H_INCLUDED

#include "defs.h"
#include <vector>
#include "Eigen/StdVector"

NAMESPACE_Cornu

enum CircularType
{
    NOT_CIRCULAR,
    CIRCULAR
};

//VectorC represents a vector with possibly circular access.
//Warning: inherits class with non-virtual destructor--do not use polymorphically!
template<typename T>
class VectorC : public std::vector<T>
{
public:
    typedef std::vector<T> Base;

    using Base::size;

    VectorC(CircularType circular) : _circular(circular) {}
    VectorC(int size, CircularType circular) : Base(size), _circular(circular) {}
    VectorC(const Base &base, CircularType circular) : Base(base), _circular(circular) {}
    VectorC(const VectorC &other) : Base(other), _circular(other._circular) {}

    T &operator[](int idx) { return Base::operator[](toLinearIdx(idx)); }
    const T &operator[](int idx) const { return Base::operator[](toLinearIdx(idx)); }

    //noncircular
    T &flatAt(int idx) { return Base::operator[](idx); }
    const T &flatAt(int idx) const { return Base::operator[](idx); }

    CircularType circular() const { return _circular; }

    //returns the size for iteration where at each iteration we access elements i, i+1, ..., i+offset
    int endIdx(int offset) const { return _circular ? (int)Base::size() : std::max(0, (int)size() - offset); }

private:
    int toLinearIdx(int idx) const
    {
        if(!_circular)
            return idx;
        int out = idx % size();
        if(out >= 0)
            return out;
        return out + size();
    }

    CircularType _circular;
};

//unfortunately, we have to specialize for Eigen's types...
template<>
class VectorC<Eigen::Vector2d> : public std::vector<EIGEN_WORKAROUND_MSVC_STL_SUPPORT(Eigen::Vector2d),
                Eigen::aligned_allocator_indirection<EIGEN_WORKAROUND_MSVC_STL_SUPPORT(Eigen::Vector2d) > >
                //Normally it should just inherit std::vector<T>, but MSVC sucks.
{
public:
    typedef Eigen::Vector2d T;
    typedef std::vector<EIGEN_WORKAROUND_MSVC_STL_SUPPORT(T),
                Eigen::aligned_allocator_indirection<EIGEN_WORKAROUND_MSVC_STL_SUPPORT(T) > > Base;

    using Base::size;

    VectorC(bool circular) : _circular(circular) {}
    VectorC(int size, bool circular) : Base(size), _circular(circular) {}
    VectorC(const Base &base, bool circular) : Base(base), _circular(circular) {}
    VectorC(const VectorC &other) : Base(other), _circular(other._circular) {}

    T &operator[](int idx) { return Base::operator[](toLinearIdx(idx)); }
    const T &operator[](int idx) const { return Base::operator[](toLinearIdx(idx)); }

    //noncircular
    T &flatAt(int idx) { return Base::operator[](idx); }
    const T &flatAt(int idx) const { return Base::operator[](idx); }

    bool circular() const { return _circular; }

    //returns the size for iteration where at each iteration we access elements i, i+1, ..., i+offset
    int endIdx(int offset) const { return _circular ? (int)Base::size() : std::max(0, (int)size() - offset); }

private:
    int toLinearIdx(int idx) const
    {
        if(!_circular)
            return idx;
        int out = idx % size();
        if(out >= 0)
            return out;
        return out + size();
    }

    bool _circular;
};

END_NAMESPACE_Cornu

#endif //VECTORC_H_INCLUDED

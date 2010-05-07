/*--
    TestUtils.h  

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

#ifndef TESTUTILS_H_INCLUDED
#define TESTUTILS_H_INCLUDED

#include <string>
#include <sstream>
#include <cassert>
#include <cstdlib>

namespace Cornu {}
namespace Eigen {}

#define STRINGIZE(EXPR) ((Stringize() << EXPR).str())

class Stringize
{
public:
    Stringize()
    {
        _out.precision(10);
    }

    template <typename T> const Stringize& operator<<(const T& item) const
    {
        _out << item;
        return *this;
    }

    std::string str() const
    {
        return _out.str();
    }

private:
    mutable std::ostringstream _out;
};

struct Assertion {};

#define CORNU_ASSERT_MSG(EXPR, MSG) \
    if(!(EXPR)) { \
        Debugging::get()->printf("Assert in %s:%d: " #EXPR, __FILE__, __LINE__); \
        Debugging::get()->printf("%s", STRINGIZE(MSG).c_str()); \
        assert(false); \
        throw Assertion(); \
    }

#define CORNU_ASSERT(EXPR) CORNU_ASSERT_MSG(EXPR, "")

#define CORNU_ASSERT_LT_MSG(EXPR, VAL, MSG) \
    CORNU_ASSERT_MSG((EXPR) < (VAL), (EXPR) << " > " << (VAL) << " " << MSG)

inline double drand(double from, double to) { return from + (to - from) * double(rand() % RAND_MAX) / double(RAND_MAX); }

#endif //TESTUTILS_H_INCLUDED

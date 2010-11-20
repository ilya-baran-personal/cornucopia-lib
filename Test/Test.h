/*--
    Test.h  

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

#ifndef CORNUCOPIA_TEST_H_INCLUDED
#define CORNUCOPIA_TEST_H_INCLUDED

#include <vector>
#include <string>

#include "TestUtils.h"

#include <sstream>

namespace Cornu {}
namespace Eigen {}

class TestCase
{
public:
    TestCase()
    {
        allTests().push_back(this);
    }

    virtual ~TestCase()
    {
        //should not be called
    }

    virtual void run() = 0; //throws on failure
    virtual std::string name() { return "Unnamed"; }

    static std::vector<TestCase *> &allTests(); //Meyer's singleton
};

#endif //CORNUCOPIA_TEST_H_INCLUDED

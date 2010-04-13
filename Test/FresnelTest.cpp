/*--
    FresnelTest.cpp  

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

#include "Test.h"
#include "Fresnel.h"

#include <iostream>

using namespace std;
using namespace Eigen;
using namespace Cornu;

class FresnelTest : public TestCase
{
public:
    //override
    std::string name() { return "FresnelTest"; }

    //override
    bool run()
    {
        const int num = 5000000;

        VectorXd t(num), s1, c1, s2, c2;

        for(int i = 0; i < num; ++i) {
            t[i] = f(i, num);
        }

        Debugging::get()->startTiming("Fresnel double");
        fresnel(t, &s1, &c1);
        Debugging::get()->elapsedTime("Fresnel double");

        Debugging::get()->startTiming("Fresnel approx");
        fresnelApprox(t, &s2, &c2);
        Debugging::get()->elapsedTime("Fresnel approx");

        ArrayXd diffV = (s1 - s2).array().square() + (c1 - c2).array().square();
        ArrayXd errVec = (diffV / (s1.array().square() + c1.array().square())).sqrt();
        double relErr = errVec.maxCoeff();
        Debugging::get()->printf("Done, max relative error = %.10lf", relErr);

        return relErr < 1e-6;
    }

    double f(int i, int num)
    {
        return (double(i) / double(num)) * 10. - 5.;
    }
};

static FresnelTest test;

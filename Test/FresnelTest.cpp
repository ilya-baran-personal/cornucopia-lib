/*--
    FresnelTest.cpp  

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

#include "Test.h"
#include "Fresnel.h"

using namespace std;
using namespace Eigen;
using namespace Cornu;

class FresnelTest : public TestCase
{
public:
    //override
    std::string name() { return "FresnelTest"; }

    //override
    void run()
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

        CORNU_ASSERT(relErr < 1e-6);
    }

    double f(int i, int num)
    {
        return (double(i) / double(num)) * 10. - 5.;
    }
};

static FresnelTest test;

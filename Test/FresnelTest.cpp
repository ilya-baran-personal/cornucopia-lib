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
        const int num = 1000000;

        VectorXd t(num), s(num), c(num);

        for(int i = 0; i < num; ++i) {
            t[i] = f(i, num);
        }

        Debugging::get()->startTiming("Fresnel double");
        fresnel(t, &s, &c);
        Debugging::get()->elapsedTime("Fresnel double");

        double sum = (s.cwiseAbs() + c.cwiseAbs()).sum();
        Debugging::get()->printf("Done, sum = %lf", sum);

        Debugging::get()->startTiming("Fresnel approx");
        fresnelApprox(t, &s, &c);
        Debugging::get()->elapsedTime("Fresnel approx");

        double sum2 = (s.cwiseAbs() + c.cwiseAbs()).sum();
        Debugging::get()->printf("Done, sum = %lf", sum2);

        return fabs(sum - sum2) / (fabs(sum) + fabs(sum2)) < 1e-2;
    }

    double f(int i, int num)
    {
        return double(rand() % 100) / 10. - 5.;
        //return double(rand() % 1000) / 1000. - .5;
    }
};

static FresnelTest test;

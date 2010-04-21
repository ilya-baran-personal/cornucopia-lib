/*--
    CurveDerivativesTest.cpp  

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
#include "Line.h"
#include "Arc.h"
#include "Clothoid.h"

#include <iostream>

using namespace std;
using namespace Eigen;
using namespace Cornu;

class CurveDerivativesTest : public TestCase
{
public:
    //override
    std::string name() { return "CurveDerivativesTest"; }

    //override
    void run()
    {
        testLine();
        testArc();
    }

    void testLine()
    {
        LinePtr line = new Line(Vector2d(1., 3.), Vector2d(3., 4.));

        CORNU_ASSERT(line->isValid());

        CurvePrimitive::ParamDer der;
        double s = 1.5;
        CurvePrimitive::ParamVec params = line->params();

        line->derivativeAt(s, der);

        double delta = 1e-6;
        for(int i = 0; i < (int)params.size(); ++i)
        {
            CurvePrimitive::ParamVec newParams = params;
            newParams[i] += delta;
            line->setParams(newParams);
            Vector2d p1 = line->pos(s);
            newParams[i] = params[i] - delta;
            line->setParams(newParams);
            Vector2d p0 = line->pos(s);
            Vector2d derV = (p1 - p0) / (2. * delta);

            CORNU_ASSERT_LT_MSG((derV.transpose() - der.row(i)).norm(), delta, "-- Param = " << i \
                << " numeric = " << derV.transpose() << " analytic = " << der.row(i));
        }        
    }

    void testArc()
    {
        ArcPtr arc = new Arc(Vector2d(1., 3.), 0.5, 3, 0.1);

        CORNU_ASSERT(arc->isValid());

        CurvePrimitive::ParamDer der;
        double s = 1.5;
        for(double c = -.1; c < .1001; c += .01)
        {
            CurvePrimitive::ParamVec params = arc->params();
            params[CurvePrimitive::CURVATURE] = c;
            arc->setParams(params);

            arc->derivativeAt(s, der);

            double delta = 5e-6;
            for(int i = 0; i < (int)params.size(); ++i)
            {
                CurvePrimitive::ParamVec newParams = params;
                newParams[i] += delta;
                arc->setParams(newParams);
                Vector2d p1 = arc->pos(s);
                newParams[i] = params[i] - delta;
                arc->setParams(newParams);
                Vector2d p0 = arc->pos(s);
                Vector2d derV = (p1 - p0) / (2. * delta);

                CORNU_ASSERT_LT_MSG((derV.transpose() - der.row(i)).norm(), delta, "-- Param = " << i \
                    << " numeric = " << derV.transpose() << " analytic = " << der.row(i));
            }
        }
    }
};

static CurveDerivativesTest test;

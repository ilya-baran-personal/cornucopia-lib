/*--
    CurveDerivativesTest.cpp  

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
        testClothoid();
    }

    void testLine()
    {
        LinePtr line = new Line(Vector2d(1., 3.), Vector2d(3., 4.));

        CORNU_ASSERT(line->isValid());

        CurvePrimitive::ParamDer der, tanDer;
        double s = 1.5;
        CurvePrimitive::ParamVec params = line->params();

        line->derivativeAt(s, der, tanDer);

        double delta = 1e-6;
        for(int i = 0; i < (int)params.size(); ++i)
        {
            CurvePrimitive::ParamVec newParams = params;
            newParams[i] += delta;
            line->setParams(newParams);
            Vector2d p1 = line->pos(s);
            Vector2d d1 = line->der(s);
            newParams[i] = params[i] - delta;
            line->setParams(newParams);
            Vector2d p0 = line->pos(s);
            Vector2d d0 = line->der(s);
            Vector2d derV = (p1 - p0) / (2. * delta);
            Vector2d tanDerV = (d1 - d0) / (2. * delta);

            CORNU_ASSERT_LT_MSG((derV - der.col(i)).norm(), delta, "-- Param = " << i \
                << " numeric = " << derV.transpose() << " analytic = " << der.col(i).transpose());
            CORNU_ASSERT_LT_MSG((tanDerV - tanDer.col(i)).norm(), delta, "-- Param = " << i \
                << " numeric = " << tanDerV.transpose() << " analytic = " << tanDer.col(i).transpose());
        }        
    }

    void testArc()
    {
        ArcPtr arc = new Arc(Vector2d(1., 3.), 0.5, 3, 0.1);

        CORNU_ASSERT(arc->isValid());

        CurvePrimitive::ParamDer der, tanDer;
        double s = 1.5;
        for(double c = -.1; c < .1001; c += .01)
        {
            CurvePrimitive::ParamVec params = arc->params();
            params[CurvePrimitive::CURVATURE] = c;
            arc->setParams(params);

            arc->derivativeAt(s, der, tanDer);

            double delta = 5e-6;
            for(int i = 0; i < (int)params.size(); ++i)
            {
                CurvePrimitive::ParamVec newParams = params;
                newParams[i] += delta;
                arc->setParams(newParams);
                Vector2d p1 = arc->pos(s);
                Vector2d d1 = arc->der(s);
                newParams[i] = params[i] - delta;
                arc->setParams(newParams);
                Vector2d p0 = arc->pos(s);
                Vector2d d0 = arc->der(s);
                Vector2d derV = (p1 - p0) / (2. * delta);
                Vector2d tanDerV = (d1 - d0) / (2. * delta);

                CORNU_ASSERT_LT_MSG((derV - der.col(i)).norm(), delta, "-- Param = " << i \
                    << " numeric = " << derV.transpose() << " analytic = " << der.col(i).transpose());
                CORNU_ASSERT_LT_MSG((tanDerV - tanDer.col(i)).norm(), delta, "-- Param = " << i \
                    << " numeric = " << tanDerV.transpose() << " analytic = " << tanDer.col(i).transpose());
            }
        }
    }

    void testClothoid()
    {
        ClothoidPtr clothoid = new Clothoid(Vector2d(1., 3.), 0.5, 3, 0.0, 0.0);

        CORNU_ASSERT(clothoid->isValid());

        CurvePrimitive::ParamDer der, tanDer;
        for(double s = 0; s < 3.; s += 0.1)
        {
            for(double sc = -.1; sc < .1001; sc += .01)
            {
                for(double ec = -.1; ec < .1001; ec += .01)
                {
                    CurvePrimitive::ParamVec params = clothoid->params();
                    params[CurvePrimitive::CURVATURE] = sc;
                    params[CurvePrimitive::DCURVATURE] = (ec - sc) / params[CurvePrimitive::LENGTH];
                    clothoid->setParams(params);

                    clothoid->derivativeAt(s, der, tanDer);

                    double delta = 5e-6;
                    for(int i = 0; i < (int)params.size(); ++i)
                    {
                        CurvePrimitive::ParamVec newParams = params;
                        newParams[i] += delta;
                        clothoid->setParams(newParams);
                        Vector2d p1 = clothoid->pos(s);
                        Vector2d d1 = clothoid->der(s);
                        newParams[i] = params[i] - delta;
                        clothoid->setParams(newParams);
                        Vector2d p0 = clothoid->pos(s);
                        Vector2d d0 = clothoid->der(s);
                        Vector2d derV = (p1 - p0) / (2. * delta);
                        Vector2d tanDerV = (d1 - d0) / (2. * delta);

                        CORNU_ASSERT_LT_MSG((derV - der.col(i)).norm(), delta, "-- Param = " << i \
                            << " numeric = " << derV.transpose() << " analytic = " << der.col(i).transpose());
                        CORNU_ASSERT_LT_MSG((tanDerV - tanDer.col(i)).norm(), delta, "-- Param = " << i \
                            << " numeric = " << tanDerV.transpose() << " analytic = " << tanDer.col(i).transpose());
                    }
                }
            }
        }
    }
};

static CurveDerivativesTest test;

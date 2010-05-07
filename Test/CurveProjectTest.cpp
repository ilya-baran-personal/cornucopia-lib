/*--
    CurveProjectTest.cpp  

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

using namespace std;
using namespace Eigen;
using namespace Cornu;

class CurveProjectTest : public TestCase
{
public:
    //override
    std::string name() { return "CurveProjectTest"; }

    //override
    void run()
    {
        maxDot = 0;

        LinePtr line = new Line(Vector2d(1., 3.), Vector2d(4., 2.));
        testProject(line);

        ArcPtr arc = new Arc(Vector2d(1., 3.), 0.5, 3, 0.1);
        testProject(arc);

        arc = new Arc(Vector2d(1., 3.), 0.5, 3, -0.1);
        testProject(arc);

        arc = new Arc(Vector2d(-1., 3.), -0.5, 39, -0.1);
        testProject(arc);

        for(int i = 0; i < 20; ++i)
        {
            //ClothoidPtr clothoid = new Clothoid(Vector2d(0.5, 1), 0.3, drand(0.01, 3.), drand(-0.1, 0.1), drand(-0.1, 0.1));
            double from = drand(-2., 2.);
            double to = drand(-2., 2.);
            if(from > to)
                swap(from, to);
            ClothoidPtr clothoid = new Clothoid(Vector2d(0.0, 0), 0.0, (to - from) / PI, from, to);
            testProject(clothoid);
        }

        Debugging::get()->printf("Projection max dot product = %.10lf", maxDot);
    }

    void testProject(CurvePrimitivePtr curve)
    {
        CORNU_ASSERT(curve->isValid());

        const double tol = 1e-5;
        for(int i = 0; i < 1000; ++i)
        {
            Vector2d pt = Vector2d(drand(-10, 10), drand(-10, 10));

            double s = curve->project(pt);
            Vector2d curvePt = curve->pos(s);
            double projDist = (pt - curvePt).norm();
            //if(curve->getType() == CurvePrimitive::CLOTHOID)
            //    Debugging::get()->printf("Proj dist = %.10lf", projDist);
            
            double dot = (pt - curvePt).normalized().dot(curve->der(s));
            if(s > tol && s < curve->length() - tol)
                maxDot = max(maxDot, fabs(dot));

            double newS = curve->project(curvePt);
            //CORNU_ASSERT_LT_MSG(fabs(s - newS), tol, "Projecting again should yield the same result");

            double len = curve->length();
            for(double newS = 0; newS < len + 1e-8; newS += (len / 30.))
            {
                Vector2d newPt = curve->pos(newS);
                //CORNU_ASSERT_LT_MSG(projDist, (pt - newPt).norm() + tol * (pt - newPt).squaredNorm(), "Projection should be closest point");
            }
        }
        for(int i = 0; i < 105000; ++i)
        {
            Vector2d pt = Vector2d(drand(-10, 10), drand(-10, 10));
            double s = curve->project(pt);
        }
    }

    double maxDot;
};

static CurveProjectTest test;

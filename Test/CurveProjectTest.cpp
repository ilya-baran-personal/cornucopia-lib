/*--
    CurveProjectTest.cpp  

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
#include "Polyline.h"

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
        LinePtr line = new Line(Vector2d(1., 3.), Vector2d(4., 2.));
        testProject(line);

        ArcPtr arc = new Arc(Vector2d(1., 3.), 0.5, 3, 0.1);
        testProject(arc);

        arc = new Arc(Vector2d(1., 3.), 0.5, 3, -0.1);
        testProject(arc);

        arc = new Arc(Vector2d(-1., 3.), -0.5, 39, -0.1);
        testProject(arc);

        for(int i = 0; i < 2; ++i)
        {
            VectorC<Vector2d> pts(5, i == 1 ? CIRCULAR : NOT_CIRCULAR);
            pts[0] = Vector2d(1, 1);
            pts[1] = Vector2d(2, 3);
            pts[2] = Vector2d(4, 4);
            pts[3] = Vector2d(1.5, 1);
            pts[4] = Vector2d(-1, -2);

            testProject(new Polyline(pts));
        }

        maxDot = 0;

        for(int i = 0; i < 200; ++i)
        {
            ClothoidPtr clothoid = new Clothoid(Vector2d(0.5, 1), 0.3, drand(0.01, 3.), drand(-5.1, 5.1), drand(-5.1, 5.1));
            testProject(clothoid);
        }

        Debugging::get()->printf("Projection max dot product = %.10lf", maxDot);
    }

    void testProject(CurvePtr curve)
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
            if(!curve->isClosed() || fabs(s - newS) + tol < curve->length())
                CORNU_ASSERT_LT_MSG(fabs(s - newS), tol, "Projecting again should yield the same result");

            double len = curve->length();
            for(double newS = 0; newS < len + 1e-8; newS += (len / 30.))
            {
                Vector2d newPt = curve->pos(newS);
                CORNU_ASSERT_LT_MSG(projDist, (pt - newPt).norm() + tol * (pt - newPt).squaredNorm(), "Projection should be closest point");
            }
        }
        if(false) for(int i = 0; i < 105000; ++i)
        {
            Vector2d pt = Vector2d(drand(-10, 10), drand(-10, 10));
            double s = curve->project(pt);
        }
    }

    double maxDot;
};

static CurveProjectTest test;

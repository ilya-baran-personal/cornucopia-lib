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
        LinePtr line = new Line(Vector2d(1., 3.), Vector2d(4., 2.));
        testProject(line);

        ArcPtr arc = new Arc(Vector2d(1., 3.), 0.5, 3, 0.1);
        testProject(arc);

        arc = new Arc(Vector2d(1., 3.), 0.5, 3, -0.1);
        testProject(arc);

        arc = new Arc(Vector2d(-1., 3.), -0.5, 39, -0.1);
        testProject(arc);
    }

    void testProject(CurvePrimitivePtr curve)
    {
        CORNU_ASSERT(curve->isValid());

        const double tol = 1e-8;
        for(int i = 0; i < 1000; ++i)
        {
            Vector2d pt = Vector2d(drand(-10, 10), drand(-10, 10));

            double s = curve->project(pt);
            Vector2d curvePt = curve->pos(s);
            
            double newS = curve->project(curvePt);
            CORNU_ASSERT_LT_MSG(fabs(s - newS), tol, "Projecting again should yield the same result");

            double len = curve->length();
            for(double newS = 0; newS < len + tol; newS += (len / 30.))
            {
                Vector2d newPt = curve->pos(newS);
                CORNU_ASSERT_LT_MSG((pt - curvePt).norm(), (pt - newPt).norm() + tol, "Projection should be closest point");
            }
        }
    }
};

static CurveProjectTest test;

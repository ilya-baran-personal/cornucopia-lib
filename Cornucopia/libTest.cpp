/*--
    libTest.cpp  

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

#include "libTest.h"
#include "Line.h"
#include "Arc.h"
#include "Clothoid.h"

using namespace std;
using namespace Eigen;

NAMESPACE_Cornu

int f(int z)
{
    Debugging::get()->printf("Working on f: %d\n", z);

    Debugging::get()->drawCurve(new Line(Vector2d(10, 10), Vector2d(100, 200)), Vector3d(0, 1, 1), "f Line", 3);
    ArcPtr arc = new Arc(Vector2d(100, 100), 0.5, 200, 0.01);
    Debugging::get()->drawCurve(arc, Vector3d(1, 0, 1), "f Arc", 3);

    for(int x = 0; x < 350; x += 30) for(int y = 0; y < 350; y += 30)
    {
        Vector2d p1 = Vector2d(double(x), double(y));
        Vector2d p2 = arc->pos(arc->project(p1));
        Debugging::get()->drawLine(p1, p2, Vector3d(0, 0, 1), "proj", 1);
    }

    {
        double stc = 0.005, enc = 0.0;
        for(enc = -0.01; enc < 0.01; enc += 0.001)
        {
            ClothoidPtr c1 = new Clothoid(Vector2d(500, 500), .5, 600, stc, enc);
            ArcPtr a1 = new Arc(Vector2d(500, 500), .5, 600, stc);
            ArcPtr a2 = new Arc(c1->endPos(), PI + c1->endAngle(), 600, -enc);

            Debugging::get()->drawCurve(c1, Vector3d(1, 0, 0), "Clothoid", 3);
            Debugging::get()->drawCurve(a1, Vector3d(0, 1, 0), "Arc", 3);
            Debugging::get()->drawCurve(a2, Vector3d(0, 1, 0), "Arc", 3);

            Vector2d pt = c1->pos(300);
            Debugging::get()->drawLine(pt, pt + c1->der(300) * 100, Vector3d(0, 0, 1), "Der1", 3);
            if(c1->der2(300).squaredNorm() > 1e-8)
                Debugging::get()->drawLine(pt, pt + c1->der2(300) * 200 / fabs(stc + enc), Vector3d(0, 0, 1), "Der2", 3);
        }
    }

    {
        Vector2d p1(500, 500), p2(150, 250), p3(300, 100), p4(100, 400);
        arc = new Arc(p1, p2, p3);
        Debugging::get()->drawCurve(arc, Vector3d(1, 0, 1), "Arc Pt 1", 3);
        arc = new Arc(p1, p2, p4);
        Debugging::get()->drawCurve(arc, Vector3d(1, 0, 1), "Arc Pt 2", 3);
        Debugging::get()->drawPoint(p1, Vector3d(1, 0, 0), "Arc Pts");
        Debugging::get()->drawPoint(p2, Vector3d(0, 1, 0), "Arc Pts");
        Debugging::get()->drawPoint(p3, Vector3d(0, 0, 1), "Arc Pts");
        Debugging::get()->drawPoint(p4, Vector3d(0, 1, 1), "Arc Pts");
    }

    return z + 4;
}

int g(int x)
{
    return x * 2 + 5;
}

END_NAMESPACE_Cornu

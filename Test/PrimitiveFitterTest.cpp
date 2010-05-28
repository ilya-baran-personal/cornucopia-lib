/*--
    PrimitiveFitterTest.cpp  

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

#include "PrimitiveFitter.h"
#include "Line.h"
#include "Arc.h"
#include "Clothoid.h"

#include "Eigen/StdVector"

using namespace std;
using namespace Eigen;
using namespace Cornu;

class PrimitiveFitterTest : public TestCase
{
public:
    //override
    std::string name() { return "PrimitiveFitterTest"; }

    //override
    void run()
    {
        for(int i = 0; i < 1000; ++i)
            testLine();
        for(int i = 0; i < 1000; ++i)
            testArc();
        for(int i = 0; i < 1000; ++i)
            testClothoid();
    }

    void testLine()
    {
        LinePtr orig = new Line(Vector2d(drand(-1, 1), drand(-1, 1)), Vector2d(drand(-1, 1), drand(-1, 1)));
        vector<Vector2d, Eigen::aligned_allocator<Vector2d> > pts;
        for(int i = 0; i < 100; ++i)
            pts.push_back(orig->pos(double(i) * orig->length() / 99.));

        const double jiggle = 0.1;

        for(int i = 0; i < (int)pts.size(); ++i)
            pts[i] += Vector2d(drand(-jiggle, jiggle), drand(-jiggle, jiggle));

        LineFitter fitter;
        for(int i = 0; i < (int)pts.size(); ++i)
            fitter.addPoint(pts[i]);

        LinePtr fit = fitter.getCurve();

        CORNU_ASSERT_LT_MSG(fabs(fit->length() - orig->length()), jiggle * sqrt(8.), "Length too different");

        for(int i = 0; i < (int)pts.size(); ++i)
        {
            double dist = (pts[i] - fit->pos(fit->project(pts[i]))).norm();
            CORNU_ASSERT_LT_MSG(dist, jiggle * sqrt(8.), "Point " << i << " too far");
        }
    }

    void testArc()
    {
        ArcPtr orig = new Arc(Vector2d(drand(-1, 1), drand(-1, 1)), Vector2d(drand(-1, 1), drand(-1, 1)), Vector2d(drand(-1, 1), drand(-1, 1)));
        if(orig->length() == 0.)
            return;

        vector<Vector2d, Eigen::aligned_allocator<Vector2d> > pts;
        for(int i = 0; i < 100; ++i)
            pts.push_back(orig->pos(double(i) * orig->length() / 99.));

        const double jiggle = 0.2;

        for(int i = 0; i < (int)pts.size(); ++i)
            pts[i] += Vector2d(drand(-jiggle, jiggle), drand(-jiggle, jiggle));

        ArcFitter fitter;
        for(int i = 0; i < (int)pts.size(); ++i)
            fitter.addPoint(pts[i]);

        ArcPtr fit = fitter.getCurve();

        CORNU_ASSERT_LT_MSG(fabs(fit->length() - orig->length()), jiggle * sqrt(8.) * 2., "Length too different");

        for(int i = 0; i < (int)pts.size(); ++i)
        {
            double dist = (pts[i] - fit->pos(fit->project(pts[i]))).norm();
            CORNU_ASSERT_LT_MSG(dist, jiggle * sqrt(8.) * 2., "Point " << i << " too far");
        }
    }

    void testClothoid()
    {
        ClothoidPtr orig = new Clothoid(Vector2d(drand(-1, 1), drand(-1, 1)), drand(-PI, PI), drand(0.1, 2.), drand(-0.2, 0.2), drand(-0.2, 0.2));

        vector<Vector2d, Eigen::aligned_allocator<Vector2d> > pts;
        const int cnt = 20;
        for(int i = 0; i < cnt; ++i)
            pts.push_back(orig->pos(double(i) * orig->length() / double(cnt - 1)));

        const double jiggle = orig->length() / double(cnt * 10.);

        for(int i = 0; i < (int)pts.size(); ++i)
            pts[i] += Vector2d(drand(-jiggle, jiggle), drand(-jiggle, jiggle));

        ClothoidFitter fitter;
        for(int i = 0; i < (int)pts.size(); ++i)
            fitter.addPoint(pts[i]);

        ClothoidPtr fit = fitter.getCurve();

        CORNU_ASSERT_LT_MSG(fabs(fit->length() - orig->length()), jiggle * sqrt(8.) * cnt, "Length too different");

        for(int i = 0; i < (int)pts.size(); ++i)
        {
            double dist = (pts[i] - fit->pos(fit->project(pts[i]))).norm();
            CORNU_ASSERT_LT_MSG(dist, jiggle * sqrt(8.) * 2., "Point " << i << " too far");
        }
    }
};

static PrimitiveFitterTest test;

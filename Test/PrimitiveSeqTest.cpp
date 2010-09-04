/*--
    PrimitiveSeqTest.cpp  

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

#include "PrimitiveSequence.h"
#include "Line.h"

using namespace std;
using namespace Eigen;
using namespace Cornu;

class PrimitiveSeqTest : public TestCase
{
public:
    //override
    std::string name() { return "PrimitiveSeqTest"; }

    //override
    void run()
    {
        Vector2d pts1[3];
        VectorC<CurvePrimitiveConstPtr> prims1(2, NOT_CIRCULAR);
        pts1[0] = Vector2d(1, 1);
        pts1[1] = Vector2d(2, 3);
        pts1[2] = Vector2d(4, 4);

        for(int i = 0; i < 2; ++i)
            prims1[i] = new Line(pts1[i], pts1[i + 1]);

        testPrimitiveSequence(PrimitiveSequence(prims1));

        Vector2d pts2[3];
        VectorC<CurvePrimitiveConstPtr> prims2(3, CIRCULAR);
        pts2[0] = Vector2d(3, 1);
        pts2[1] = Vector2d(1, 3);
        pts2[2] = Vector2d(5, 4);

        for(int i = 0; i < 3; ++i)
            prims2[i] = new Line(pts2[i], pts2[(i + 1) % 3]);
        
        testPrimitiveSequence(PrimitiveSequence(prims2));
    }

    void testPrimitiveSequence(const PrimitiveSequence &p)
    {
        //test trimming
        double startFrom = 0, endFrom = p.length();
        if(p.isClosed())
        {
            startFrom = -p.length();
            endFrom = p.length() * 2;
        }
        const double step = 0.1;
        for(double from = startFrom; from < endFrom; from += step)
        {
            for(double to = from + step; to - from < p.length(); to += step)
            {
                PrimitiveSequencePtr trim = p.trimmed(from, to);

                for(double x = 0; x < trim->length(); x += step * 0.5)
                {
                    Vector2d diff = trim->pos(x) - p.pos(from + x);
                    CORNU_ASSERT_LT_MSG(diff.norm(), 1e-8, "Incorrect trim");
                }
            }
        }
    }
};

static PrimitiveSeqTest test;

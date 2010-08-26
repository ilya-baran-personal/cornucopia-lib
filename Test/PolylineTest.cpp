/*--
    PolylineTest.cpp  

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

#include "Polyline.h"

using namespace std;
using namespace Eigen;
using namespace Cornu;

class PolylineTest : public TestCase
{
public:
    //override
    std::string name() { return "PolylineTest"; }

    //override
    void run()
    {
        VectorC<Vector2d> pts1(3, NOT_CIRCULAR);
        pts1[0] = Vector2d(1, 1);
        pts1[1] = Vector2d(2, 3);
        pts1[2] = Vector2d(4, 4);
        
        testPolyline(Polyline(pts1));

        VectorC<Vector2d> pts2(3, CIRCULAR);
        pts2[0] = Vector2d(3, 1);
        pts2[1] = Vector2d(1, 3);
        pts2[2] = Vector2d(5, 4);
        
        testPolyline(Polyline(pts2));
    }

    void testPolyline(const Polyline &p)
    {
        vector<double> params;
        vector<int> indices;

        const int num = 20;
        for(int i = 0; i < num; ++i)
        {
            double param = p.length() * double(i) / double(num - 1);
            params.push_back(param);
            int idx = p.paramToIdx(param);
            CORNU_ASSERT(idx >= 0);
            CORNU_ASSERT(idx <= (int)p.pts().size());
            indices.push_back(idx);
        }

        for(int i = 1; i < (int)indices.size(); ++i)
        {
            CORNU_ASSERT(indices[i] >= indices[i - 1]);

            Vector2d prev = p.pos(params[i - 1]);
            Vector2d cur = p.pos(params[i]);

            CORNU_ASSERT_LT_MSG((prev - cur).norm(), (params[i] - params[i - 1]) + 1e-12, "Samples on polyline too far apart");
            if(indices[i] == indices[i - 1])
            {
                Vector2d diff = (cur - prev).normalized() - p.der(0.5 * (params[i - 1] + params[i]));
                CORNU_ASSERT_LT_MSG(diff.norm(), 1e-8, "Incorrect derivative at indices: " << indices[i] << " " << indices[i - 1]);
            }
        }

        //test trimming
        double startFrom = 0, endFrom = p.length();
        if(p.isClosed())
        {
            startFrom = -p.length();
            endFrom = p.length() * 2;
        }
        for(double from = startFrom; from < endFrom; ++from)
        {
            const double step = 0.1;
            for(double to = from + step; to - from < p.length(); to += step)
            {
                PolylinePtr trim = p.trimmed(from, to);

                for(double x = 0; x < trim->length(); x += step * 0.5)
                {
                    Vector2d diff = trim->pos(x) - p.pos(from + x);
                    CORNU_ASSERT_LT_MSG(diff.norm(), 1e-8, "Incorrect trim");
                }
            }
        }
    }
};

static PolylineTest test;

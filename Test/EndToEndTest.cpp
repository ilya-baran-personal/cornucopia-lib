/*--
    EndToEndTest.cpp  

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

#include <cstdio>
#include "Test.h"
#include "SimpleAPI.h" //just the simple API
#include "Cornucopia.h" //includes everything necessary to use the library

class EndToEndTest : public TestCase
{
public:
    //override
    std::string name() { return "EndToEndTest"; }

    //override
    void run()
    {
        simpleAPITest();
        fullAPITest();
    }

    void simpleAPITest()
    {
        Cornu::Parameters params; //default values
        std::vector<Cornu::Point> pts;

        pts.push_back(Cornu::Point(100, 100));
        pts.push_back(Cornu::Point(120, 130));
        pts.push_back(Cornu::Point(140, 140));
        pts.push_back(Cornu::Point(300, 140));

        //pass it to the fitter and process it
        std::vector<Cornu::BasicPrimitive> result = Cornu::fit(pts, params);

        int nPrims[3] = { 0, 0, 0 };
        for(int i = 0; i < (int)result.size(); ++i)
        {
            nPrims[result[i].type]++;
        }

        Cornu::Debugging::get()->printf("SimpleAPI Finished, #lines = %d, #arcs = %d, #clothoids = %d\n", nPrims[0], nPrims[1], nPrims[2]);

        std::vector<Cornu::BasicBezier> bezier = Cornu::toBezierSpline(result, 1.);

        Cornu::Debugging::get()->printf("Conversion to Bezier results in %d segments\n", bezier.size());
    }

    void fullAPITest()
    {
        //initialize the fitter
        Cornu::Fitter fitter;
        Cornu::Parameters params; //default values
        fitter.setParams(params);

        //initialize the curve to a sequence of points (there will usually be more and they'll come from a mouse or tablet)
        Cornu::VectorC<Eigen::Vector2d> pts(4, Cornu::NOT_CIRCULAR);

        pts[0] = Eigen::Vector2d(100, 100);
        pts[1] = Eigen::Vector2d(120, 130);
        pts[2] = Eigen::Vector2d(140, 140);
        pts[3] = Eigen::Vector2d(300, 140);

        //pass it to the fitter and process it
        fitter.setOriginalSketch(new Cornu::Polyline(pts));
        fitter.run(); //to see why this prints debugging output, look at DebuggingTestImpl in Test.cpp

        //process the output -- count the number of primitives of each type
        Cornu::PrimitiveSequenceConstPtr output = fitter.finalOutput();

        int nPrims[3] = { 0, 0, 0 };
        for(int i = 0; i < output->primitives().size(); ++i)
        {
            nPrims[output->primitives()[i]->getType()]++;
        }

        Cornu::Debugging::get()->printf("FullAPI Finished, #lines = %d, #arcs = %d, #clothoids = %d\n", nPrims[0], nPrims[1], nPrims[2]);

        //now try again with a reduced pixel size
        double scale = 0.01;

        params.set(Cornu::Parameters::PIXEL_SIZE, scale);

        for(int i = 0; i < pts.size(); ++i)
            pts[i] *= scale;

        fitter.setParams(params);
        fitter.setOriginalSketch(new Cornu::Polyline(pts));
        fitter.run();

        nPrims[0] = nPrims[1] = nPrims[2] = 0;
        for(int i = 0; i < output->primitives().size(); ++i)
        {
            nPrims[output->primitives()[i]->getType()]++;
        }

        Cornu::Debugging::get()->printf("Small Pixels Finished, #lines = %d, #arcs = %d, #clothoids = %d\n", nPrims[0], nPrims[1], nPrims[2]);

        //output is destroyed with the destruction of the smart pointer and the fitter
    }
};

static EndToEndTest test;

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

#include "Test.h"
#include "Cornucopia.h" //includes everything necessary to use the library

class EndToEndTest : public TestCase
{
public:
    //override
    std::string name() { return "EndToEndTest"; }

    //override
    void run()
    {
        //initialize the fitter
        Cornu::Fitter fitter;
        fitter.setParams(Cornu::Parameters::presets()[0]); //0 is the default parameters

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

        printf("Finished, #lines = %d, #arcs = %d, #clothoids = %d\n", nPrims[0], nPrims[1], nPrims[2]);

        //output is destroyed with the destruction of the smart pointer and the fitter
    }
};

static EndToEndTest test;

/*--
    CornerDetector.cpp  

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

#include "CornerDetector.h"
#include "Fitter.h"
#include "Preprocessing.h"
#include "Polyline.h"
#include "PrimitiveFitter.h"

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

class BaseCornerDetector : public Algorithm<CORNER_DETECTION>
{
public:
    string name() const { return "Default"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<CORNER_DETECTION> &out)
    {
        const VectorC<Vector2d> &pts = fitter.output<CURVE_CLOSING>()->output->pts();

        out.corners.resize(pts.size(), false);
        out.corners.setCircular(pts.circular());

        double threshold = 5; //TODO

        VectorC<double> probabilities = cornerProbabilities(fitter);

        for(int i = 0; i < (int)pts.size(); ++i)
        {
            bool valid = true;

            for(int sign = -1; sign < 2; sign += 2) //walk in both directions
            {
                VectorC<Vector2d>::Circulator cur = pts.circulator(i);
                double dist = 0;
                for(cur += sign; !cur.done(); cur += sign)
                {
                    dist += ((*cur) - (*(cur - sign))).norm();
                    if(dist > threshold) //if we walked too far along the line, break
                        break;

                    //check if we have a point near i with a higher corner probability
                    if(cur.index() != i && probabilities[cur.index()] >= probabilities[i])
                    {
                        valid = false; //if so, i is not a corner
                        break;
                    }
                }
            }

            out.corners[i] = valid && probabilities[i] > 0.;
        }
    }

    virtual VectorC<double> cornerProbabilities(const Fitter &fitter) = 0;
};

class DefaultCornerDetector : public BaseCornerDetector
{
protected:
    virtual VectorC<double> cornerProbabilities(const Fitter &fitter)
    {
        const VectorC<Vector2d> &pts = fitter.output<CURVE_CLOSING>()->output->pts();
        return VectorC<double>(pts.size(), pts.circular()); //TODO
    }
};

void Algorithm<CORNER_DETECTION>::_initialize()
{
    new DefaultCornerDetector();
}

END_NAMESPACE_Cornu



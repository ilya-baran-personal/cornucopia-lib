/*--
    Preprocessing.cpp  

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

#include "Preprocessing.h"
#include "Fitter.h"
#include "Polyline.h"
#include "Debugging.h"
#include "Line.h"
#include <iostream>

#include <Eigen/Geometry>

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

class NoScaleDetector : public Algorithm<SCALE_DETECTION>
{
public:
    string name() const { return "None"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<SCALE_DETECTION> &out)
    {
        out.scale = 1.;
    }
};

class AdaptiveScaleDetector : public Algorithm<SCALE_DETECTION>
{
public:
    string name() const { return "Adaptive"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<SCALE_DETECTION> &out)
    {
        double pixel = fitter.params().get(Parameters::PIXEL_SIZE);

        //compute the diagonal length of the bounding box of the curve
        const VectorC<Vector2d> &pts = fitter.originalSketch()->pts();
        AlignedBox<double, 2> boundingBox(pts[0]);
        for(int i = 1; i < (int)pts.size(); ++i)
            boundingBox.extend(pts[i]);
        double diag = boundingBox.diagonal().norm();

        const double smallCurvePixels = fitter.params().get(Parameters::SMALL_CURVE_PIXELS);
        const double largeCurvePixels = fitter.params().get(Parameters::LARGE_CURVE_PIXELS);
        const double maxRescale = fitter.params().get(Parameters::MAX_RESCALE);

        out.scale = 1.;

        if(diag < pixel * smallCurvePixels)
            out.scale = max(1. / maxRescale, diag / (pixel * smallCurvePixels));
        if(diag > pixel * largeCurvePixels)
            out.scale = min(maxRescale, diag / (pixel * largeCurvePixels));
    }
};

void Algorithm<SCALE_DETECTION>::_initialize()
{
    new AdaptiveScaleDetector();
    new NoScaleDetector();
};

//========================Preliminary Resampling=============================

class DefaultPrelimResampling : public Algorithm<PRELIM_RESAMPLING>
{
public:
    string name() const { return "Default"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<PRELIM_RESAMPLING> &out)
    {
        const VectorC<Vector2d> &pts = fitter.originalSketch()->pts();
        VectorC<Vector2d> outPts(0, NOT_CIRCULAR);

        const double radius = fitter.scaledParameter(Parameters::MIN_PRELIM_LENGTH);

        vector<bool> keep = _markDouglasPeucker(pts, fitter.scaledParameter(Parameters::DP_CUTOFF));

        outPts.push_back(pts[0]);
        for(int idx = 1; idx < pts.size(); ++idx)
        {
            Vector2d curResampled = outPts.back();
            Vector2d curPt = pts[idx];

            double distSqToCur = (curPt - curResampled).squaredNorm();

            if(idx + 1 == pts.size() || keep[idx])
            {
                if(distSqToCur > 1e-16)
                    outPts.push_back(curPt);
                continue;
            }

            if(distSqToCur < SQR(radius))
                continue;

            Vector2d prevPt = pts[idx - 1];
            
            ParametrizedLine<double, 2> line = ParametrizedLine<double, 2>::Through(prevPt, curPt);

            Vector2d closestOnLine = line.projection(curResampled);
            double distSqToLine = (curResampled - closestOnLine).squaredNorm();
            if(distSqToLine < 1e-16)
            {
                outPts.push_back(curPt);
                continue;
            }
            double y = sqrt(max(0., radius * radius - distSqToLine));
            Vector2d newPt = closestOnLine + y * line.direction(); //y is positive, so this will find the correct point

            outPts.push_back(newPt);
            --idx;
        }

        out.output = new Polyline(outPts);
        for(int i = 0; i < (int)outPts.size(); ++i)
            Debugging::get()->drawPoint(outPts[i], Vector3d(0, 0, 1), "Prelim resampled");
        Debugging::get()->drawCurve(out.output, Vector3d(0, 0, 1), "Prelim resampled curve");
    }

private:
    vector<bool> _markDouglasPeucker(const VectorC<Vector2d> &pts, double cutoff)
    {
        vector<bool> out(pts.size(), false);
        out.front() = out.back() = true;

        _dpHelper(pts, out, cutoff, 0, pts.size());
        return out;
    }

    void _dpHelper(const VectorC<Vector2d> &pts, vector<bool> &out, double cutoff, int start, int end)
    {
        Line cur(pts[start], pts[end - 1]);
        double maxDist = cutoff;
        int mid = -1;
        for(int i = start; i < end; ++i)
        {
            double dist = (pts[i] - cur.pos(cur.project(pts[i]))).norm();
            if(dist > maxDist)
            {
                maxDist = dist;
                mid = i;
            }
        }
        if(mid < 0)
            return;
        out[mid] = true;
        _dpHelper(pts, out, cutoff, start, mid + 1);
        _dpHelper(pts, out, cutoff, mid, end);
    }
};

class NoPrelimResampling : public Algorithm<PRELIM_RESAMPLING>
{
public:
    string name() const { return "None"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<PRELIM_RESAMPLING> &out)
    {
        out.output = fitter.originalSketch();
    }
};

void Algorithm<PRELIM_RESAMPLING>::_initialize()
{
    new DefaultPrelimResampling();
    new NoPrelimResampling();
}

//========================Curve Closer=============================

class OldCurveCloser : public Algorithm<CURVE_CLOSING>
{
public:
    string name() const { return "Old"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<CURVE_CLOSING> &out)
    {
        out.output = fitter.output<PRELIM_RESAMPLING>()->output;
        out.closed = false;
        
        const double tol = fitter.scaledParameter(Parameters::CLOSEDNESS_THRESHOLD);
        const double tolSq = SQR(tol);
        const VectorC<Vector2d> &pts = fitter.output<PRELIM_RESAMPLING>()->output->pts();

        if(pts.size() < 3)
            return; //definitely not closed

        Vector2d start = pts[0], end = pts.back();

        //find point farthest away from start-end line segment
        Line startEnd(start, end);

        if(startEnd.length() > tol) //start and end too far
            return;

        double maxDistSq = 0;
        int farthest = 1;
        for(int i = 1; i + 1 < (int)pts.size(); ++i) {
            double t = startEnd.project(pts[i]);
            double distSq = (pts[i] - startEnd.pos(t)).squaredNorm();
            if(distSq > maxDistSq) {
                maxDistSq = distSq;
                farthest = i;
            }
        }

        if(maxDistSq < tolSq)
            return;

        //now find the points closest to each other that are within tol of each
        //other and of the segment connecting the start and end points
        int closest0 = 0, closest1 = (int)pts.size() - 1;
        double minDistSq = tolSq;
        for(int i = 0; i < farthest; ++i) {
            double t = startEnd.project(pts[i]);
            if((pts[i] - startEnd.pos(t)).squaredNorm() > tolSq)
                break;
            for(int j = (int)pts.size() - 1; j > farthest; --j) {
                double t2 = startEnd.project(pts[j]);
                if((pts[j] - startEnd.pos(t2)).squaredNorm() > tolSq)
                    break;
                double distSq = (pts[i] - pts[j]).squaredNorm();
                if(distSq > minDistSq)
                    continue;
                minDistSq = distSq;
                closest0 = i;
                closest1 = j;
            }
        }

        if(minDistSq == tolSq)
            return;
        //if we are here, curve is closed
        out.closed = true;

        //close and adjust curve
        VectorC<Vector2d>::Base newPts = pts;

        if(closest1 + 1 < (int)newPts.size())
            newPts.erase(newPts.begin() + closest1 + 1, newPts.end());
        if(closest0 > 0)
            newPts.erase(newPts.begin(), newPts.begin() + closest0 - 1);

        PolylinePtr polyptr = new Polyline(VectorC<Vector2d>(newPts, NOT_CIRCULAR));
        Vector2d adjust = newPts[0] - newPts.back();
        const double adjustPower = 3.;
        for(int i = 0; i < (int)newPts.size(); ++i) {
            double param = polyptr->idxToParam(i) / polyptr->length();
            double adjustFactor = 0.5 * pow(fabs(param - 0.5) * 2., adjustPower) * (param > 0.5 ? 1. : -1.);
            newPts[i] += adjust * adjustFactor;
        }
        newPts.pop_back(); //the last point is duplicate

        out.output = new Polyline(VectorC<Vector2d>(newPts, CIRCULAR));

        Debugging::get()->drawCurve(out.output, Debugging::Color(0., 0., 0.), "Closed", 2., Debugging::DOTTED);
    }
};

void Algorithm<CURVE_CLOSING>::_initialize()
{
    new OldCurveCloser();
}

END_NAMESPACE_Cornu



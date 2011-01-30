/*--
    Resampler.cpp  

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

#include "Resampler.h"
#include "Fitter.h"
#include "Oversketcher.h"
#include "Polyline.h"
#include "CornerDetector.h"
#include "PrimitiveFitUtils.h"
#include "Arc.h"

#include <iterator>
#include <map>
#include <cstdio>

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

#define RESAMPLING_DEBUG 0

class BaseResampler : public Algorithm<RESAMPLING>
{
protected:
    //split the input into polylines based on corners and resample them individually
    void _run(const Fitter &fitter, AlgorithmOutput<RESAMPLING> &out)
    {
        VectorC<bool> corners = fitter.output<CORNER_DETECTION>()->corners;
        smart_ptr<const AlgorithmOutput<OVERSKETCHING> > osOutput = fitter.output<OVERSKETCHING>();
        PolylineConstPtr poly = osOutput->output;
        const VectorC<Vector2d> &pts = poly->pts();

        if(!poly->isClosed()) //if curve is not closed, start and end points are effectively corners
            corners[0] = corners.back() = true;

        bool anyCorners = false;
        for(int i = 0; i < corners.size(); ++i)
            anyCorners = anyCorners || corners[i];

        if(!anyCorners) //also implies curve is closed
        {
            out.output = _resample(fitter, poly);
            out.corners = VectorC<bool>(vector<bool>(out.output->pts().size(), false), CIRCULAR);
            displayOutput(out);
            return;
        }

        out.corners = VectorC<bool>(0, pts.circular());
        VectorC<Vector2d> outputPts(0, pts.circular());

        for(int idx = 0; idx < pts.endIdx(1); ++idx) //go through all possible segment starting points
        {
            if(!corners[idx])
                continue;

            bool denseNearStart = (idx == 0) && osOutput->startCurve;

            VectorC<Vector2d> segment(0, NOT_CIRCULAR);
            for(VectorC<Vector2d>::Circulator circ = pts.circulator(idx); !circ.done(); ++circ, ++idx)
            {
                segment.push_back(*circ);
                if(segment.size() > 1 && corners[circ.index()])
                    break;
            }

            bool denseNearEnd = (idx + 1 == pts.size()) && osOutput->endCurve;

            --idx; //we should start the next segment from the last point of the current one

            PolylineConstPtr resampled = _resample(fitter, new Polyline(segment), denseNearStart, denseNearEnd);
            int startIdx = outputPts.empty() ? 0 : 1;
            copy(resampled->pts().begin() + startIdx, resampled->pts().end(), back_inserter(outputPts));
            out.corners.insert(out.corners.end(), resampled->pts().end() - (resampled->pts().begin() + startIdx), false);
            out.corners[0] = out.corners.back() = true;
        }

        if(poly->isClosed())
        {
            outputPts.pop_back();
            out.corners.pop_back();
        }

        out.output = new Polyline(outputPts);
        displayOutput(out);
    }

    virtual PolylineConstPtr _resample(const Fitter &fitter, PolylineConstPtr poly, bool denseNearStart = false, bool denseNearEnd = false) = 0;

private:
    void displayOutput(AlgorithmOutput<RESAMPLING> &out)
    {
        for(int i = 0; i < out.output->pts().size(); ++i)
        {
            Debugging::get()->drawPoint(out.output->pts()[i], Vector3d(0, 0, 0), "Resampled");
            if(out.corners[i])
                Debugging::get()->drawPoint(out.output->pts()[i], Vector3d(0, 0, 0), "Resampled Corners");
        }
        Debugging::get()->printf("Num samples = %d", out.output->pts().size());
    }
};

//Represents the piecewise-linear function over the parametric domain of the input curve
//whose value is an upper bound on the distance between samples.
class SampleSpacingFunction
{
public:
    SampleSpacingFunction(PolylineConstPtr poly)
        : _values(vector<double>(poly->pts().size(), 0.), poly->pts().circular()), _maxSlope(1e10), _basePoly(poly)
    {
        _lengths.resize(1 + poly->pts().endIdx(1), 0.);
        for(int i = 0; i < (int)_lengths.size(); ++i)
            _lengths[i] = poly->idxToParam(i);
    }

    VectorC<double> &values() { return _values; }

    bool isClosed() const { return _values.circular() == CIRCULAR; }
    
    //Returns the distance from the point at idx to the next point.
    //note the boundary condition for closed curves
    double distToNext(int idx) { int flatIdx = _values.toLinearIdx(idx); return _lengths[flatIdx + 1] - _lengths[flatIdx]; }

    void enforceMaxSlope(double maxSlope)
    {
        _maxSlope = min(_maxSlope, maxSlope);
        //Make sure the sample spacing doesn't vary more than allowed.
        //Do this by walking forward and backward and enforcing this condition on adjacent
        //points--note that we need to loop around twice for closed curves (well, technically, 1.5 times).
        int maxIdx = isClosed() ? (_values.size() * 2) : (_values.size() - 1);
        for(int i = 0; i < maxIdx; ++i) //forward
            _values[i + 1] = min(_values[i + 1], _values[i] + distToNext(i) * maxSlope);
        for(int i = maxIdx - 1; i >= 0; --i) //backward
            _values[i] = min(_values[i], _values[i + 1] + distToNext(i) * maxSlope);
    }

    //Returns the value of the function at s
    double eval(double s) const
    {
        if(_values.circular())
        {
            s = fmod(s, _lengths.back());
            if(s < 0.)
                s += _lengths.back();
        }
        double cParam;
        int idx = paramToIdx(s, &cParam);
        int nidx = (idx + 1) % _values.size();
        double invLength = (1. / (_lengths[idx + 1] - _lengths[idx]));
        return _values.flatAt(idx) + (cParam * invLength) * (_values.flatAt(nidx) - _values.flatAt(idx));
    }

    //Returns the maximum step we can take starting at s.  It's the side length of the largest square
    //we can inscribe under the function plot with a corner at s.
    double evalStep(double s)
    {
        double maxStep = eval(s);
        SampleSpacingFunction tr = trimmed(s, s + maxStep);

        double out = 0;
        double minSoFar = maxStep;
        for(int i = 0; i + 1 < tr._values.size(); ++i)
        {
            //consider the segment from i to i + 1
            double len = tr._lengths[i + 1] - tr._lengths[i];
            double r1 = tr._values[i], r2 = tr._values[i + 1];

            //check if we get this entire segment
            if(min(minSoFar, r2) >= out + len)
            {
                minSoFar = min(minSoFar, r2);
                out += len;
                continue;
            }

            double maxFromBefore = minSoFar;
            double curSlope = (r2 - r1) / (len + 1e-16);
            double curYIntercept = r1 - curSlope * tr._lengths[i];
            double maxFromCurrent = curYIntercept / (1. - curSlope);

            return min(maxFromBefore, maxFromCurrent);
        }

        return minSoFar;
    }

    int paramToIdx(double param, double *outParam) const
    {
        int idx = (int)min(std::upper_bound(_lengths.begin(), _lengths.end(), param) - _lengths.begin(), (ptrdiff_t)_lengths.size() - 1) - 1;
        if(outParam)
            *outParam = param - _lengths[idx];
        return idx;
    }

    SampleSpacingFunction trimmed(double from, double to)
    {
        //Trim the base polyline and evaluate the function on it.  Not the fastest way to do it, but it works
        PolylineConstPtr trimmedPoly = _basePoly->trimmed(from, to);
        SampleSpacingFunction out(trimmedPoly);

        for(int i = 0; i < trimmedPoly->pts().size(); ++i)
            out.values()[i] = eval(from + trimmedPoly->idxToParam(i));

        return out;
    }

    void draw()
    {
        char name[100];
        static int cnt = 0;
        ++cnt;
        sprintf(name, "Func %d", cnt);
        Vector2d offs(10, 20 + 50 * cnt);
        Debugging::get()->drawLine(offs, offs + Vector2d(_lengths.back(), 0), Vector3d(0, 0, 0), name);
        for(int i = 0; i < _values.endIdx(1); ++i)
            Debugging::get()->drawLine(offs + Vector2d(_lengths[i], -_values[i]), offs + Vector2d(_lengths[i + 1], -_values[i + 1]), Vector3d(1, 0, 0), name);

        //draw the inscribed squares
        double param = 0;
        while(param < _lengths.back())
        {
            double step = evalStep(param);
            Vector2d corner = offs + Vector2d(param, 0);
            Debugging::get()->drawLine(corner, corner + Vector2d(0, -step), Vector3d(0, 1, 0), name);
            Debugging::get()->drawLine(corner + Vector2d(0, -step), corner + Vector2d(step, -step), Vector3d(0, 1, 0), name);
            Debugging::get()->drawLine(corner + Vector2d(step, -step), corner + Vector2d(step, 0), Vector3d(0, 1, 0), name);
            param += step;
        }
    }

    void scale(double sc)
    {
        _maxSlope *= sc;
        for(int i = 0; i < _values.size(); ++i)
            _values[i] *= sc;
    }

    bool selfTest()
    {
        //test slope
        for(int times = 0; times < 10000; ++times)
        {
            double param1 = fmod(double(rand()), _lengths.back());
            double param2 = fmod(double(rand()), _lengths.back());
            double diff = fabs(param2 - param1);
            if(_values.circular())
                diff = min(diff, _lengths.back() - diff);
            double val1 = eval(param1);
            double val2 = eval(param2);

            if(fabs(val1 - val2) > 1e-8 + _maxSlope * diff)
                return false;
        }

        //test uniformity
        for(int times = 0; times < 10000; ++times)
        {
            double param = fmod(double(rand()), _lengths.back() * 0.5);
            double offs = fmod(double(rand()) * 0.01, param * 0.5);

            double stepEnd1 = param + evalStep(param);
            double stepEnd0 = (param - offs) + evalStep(param - offs);
            double stepEnd2 = (param + offs) + evalStep(param + offs);

            if(stepEnd0 > stepEnd1 + 1e-8)
                return false;
            if(stepEnd1 > stepEnd2 + 1e-8)
                return false;
        }

        return true;
    }

private: 
    double _maxSlope;
    //organized like Polyline
    VectorC<double> _values;
    vector<double> _lengths;
    PolylineConstPtr _basePoly;
};

//Approximates a monotone function piecewise-linearly.  Supports adding points and solving for x given y.
//Used for solving for the adjustment to the sampling rate that leads to an integer number of samples.
class PiecewiseLinearMonotone
{
public:
    void add(double x, double y)
    {
        _map[x] = y;
    }

    //Find the x that maps to y.
    double invert(double y) const
    {
        const double tol = 1e-8;
        for(map<double, double>::const_iterator it = _map.begin(); it != _map.end(); ++it)
        {
            map<double, double>::const_iterator it2 = it;
            ++it2;
            if(it2 == _map.end())
                break;

            double x1 = it->first, y1 = it->second;
            double x2 = it2->first, y2 = it2->second;
            if(fabs(y1 - y2) < tol)
                continue;

            if(y1 < y2)
                Debugging::get()->printf("ERROR: non-monotone sampling");

            double w = (y - y1) / (y2 - y1);
            double x = x1 * (1. - w) + x2 * w;
            if(x + tol < x1 && it != _map.begin())
                continue;
            if(x - tol > x2 && (++it2 != _map.end())) //at this point, we don't need it2 anymore
                continue;

            return x;
        }

        return -1; //not found
    }

private:
    map<double, double> _map;
};

class DefaultResampler : public BaseResampler
{
public:
    string name() const { return "Default"; }

protected:
    PolylineConstPtr _resample(const Fitter &fitter, PolylineConstPtr poly, bool denseNearStart = false, bool denseNearEnd = false)
    {
        const VectorC<Vector2d> pts = poly->pts();
        SampleSpacingFunction spacing(poly);

        //estimate the sample spacing from the curvatures
        double step = fitter.scaledParameter(Parameters::DENSE_SAMPLING_STEP);
        double regionSize = fitter.scaledParameter(Parameters::CURVATURE_ESTIMATE_REGION);
        double maxInterval = fitter.scaledParameter(Parameters::MAX_SAMPLING_INTERVAL);
        double pointsPerCircle = fitter.params().get(Parameters::POINTS_PER_CIRCLE);
        double arcFitterScale = 1. / fitter.scale(); //our ArcFitter is approximate and not scale-invariant, so we scale its input and output

        for(int i = 0; i < pts.size(); ++i)
        {
            ArcFitter arcFitter;
            //step forward
            double startParam = poly->idxToParam(i);
            for(double param = startParam; poly->isParamValid(param) && param < startParam + regionSize; param += step)
                arcFitter.addPoint(poly->pos(param) * arcFitterScale);
            //step backward
            for(double param = startParam; poly->isParamValid(param) && param > startParam - regionSize; param -= step)
                arcFitter.addPoint(poly->pos(param) * arcFitterScale);

            double curvature = fabs(arcFitter.getCurve()->startCurvature()) * arcFitterScale;
            spacing.values()[i] = TWOPI / max(pointsPerCircle * curvature, TWOPI / maxInterval);
            if((i == 0 && denseNearStart) || ((i + 1) == pts.size() && denseNearEnd))
                spacing.values()[i] = 1;
        }

        //Make sure the sample spacing doesn't vary more than allowed
        double maxSlope = fitter.params().get(Parameters::MAX_SAMPLE_RATE_SLOPE);
        spacing.enforceMaxSlope(maxSlope);

#if RESAMPLING_DEBUG
        if(!spacing.selfTest())
            Debugging::get()->printf("Error: spacing self-test failed");
#endif

        //We need for the last sample to end precisely at the end of the curve.  This won't happen naturally,
        //so we scale the sampling rate until it does.
        //Start by figuring out how many samples we'll have and what the "remainder" is and an initial guess at
        //how much we need to scale.
        int numPts = 0;
        double param = 0;
        while(true)
        {
            double step = spacing.evalStep(param);
            param += step;
            ++numPts;
            if(param + step * 0.5 > poly->length())
                break;
        }

        //We maintain the sampling spacing scales and the ratio of the last parameter to the curve length
        //as a piecewise linear function.
        //It is guaranteed to be monotone--reducing the sampling spacing must reduce the last parameter.
        PiecewiseLinearMonotone pl;

        double scale = poly->length() / param;
        pl.add(1., scale);
#if RESAMPLING_DEBUG
        Debugging::get()->printf("Log scale error at 1 = %lf", log(scale));
#endif

        for(int iters = 0; iters < 3; ++iters) //three iterations have been sufficient for convergence
        {
            SampleSpacingFunction scaledSpacing = spacing;
            scaledSpacing.scale(scale);

            param = 0;
            for(int i = 0; i < numPts; ++i)
                param += scaledSpacing.evalStep(param);

            double y = poly->length() / param;
            pl.add(scale, y);
#if RESAMPLING_DEBUG
            Debugging::get()->printf("Log scale error at %lf = %lf", scale, log(y));
#endif

            scale = pl.invert(1.);
            if(scale < 0)
            {
                scale = 1;
                break;
            }
        }

        spacing.scale(scale);
        VectorC<Vector2d> out(0, pts.circular());

        param = 0;
        for(int i = 0; i < numPts; ++i)
        {
            out.push_back(poly->pos(param));
            param += spacing.evalStep(param);
        }

#if RESAMPLING_DEBUG
        //DBG:
        Debugging::get()->printf("Log scale error Final = %lf", log(poly->length() / param));
        spacing.draw();
#endif

        if(!poly->isClosed())
            out.push_back(poly->endPos());        

        return new Polyline(out);
    }
};

class LengthResampler : public BaseResampler
{
public:
    string name() const { return "Length"; }

protected:
    PolylineConstPtr _resample(const Fitter &fitter, PolylineConstPtr poly, bool denseNearStart = false, bool denseNearEnd = false)
    {
        int numPts = 2 + int(poly->length() / fitter.scaledParameter(Parameters::MAX_SAMPLING_INTERVAL));
        VectorC<Vector2d> out(numPts, poly->pts().circular());

        for(int i = 0; i < numPts; ++i)
        {
            double param = double(i) / (poly->isClosed() ? numPts : numPts - 1);
            out[i] = poly->pos(param * poly->length());
        }

        return new Polyline(out);
    }
};

void Algorithm<RESAMPLING>::_initialize()
{
    new DefaultResampler();
    new LengthResampler();
}

END_NAMESPACE_Cornu



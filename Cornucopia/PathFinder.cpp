/*--
    PathFinder.cpp  

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

#include "PathFinder.h"
#include "GraphConstructor.h"
#include "PrimitiveFitter.h"
#include "CurvePrimitive.h"
#include "Preprocessing.h"
#include "Fitter.h"

#include <queue>

using namespace std;
using namespace Eigen;
NAMESPACE_Cornu

struct PathFindingVertexData
{
    PathFindingVertexData()
        : distance(0.), finished(false), prevEdge(-1), source(false), target(false), numIncoming(0), numOutgoing(0)
    {
    }

    double distance;
    bool finished;
    int prevEdge;
    bool source;
    bool target;
    int numIncoming, numOutgoing;
};

class PathFindingEdgeData
{
public:
    PathFindingEdgeData(const Edge *edge)
        : _edge(edge), _validated(false), _cost(_edge->cost), _reducedCost(0)
    {
        _ignore = _cost >= Parameters::infinity;
    }

    bool validate(const Fitter &fitter)
    {
        if(_validated)
            return true;
        _validated = true;
        double newCost = _edge->validatedCost(fitter);
        if(newCost > _cost)
        {
            //Debugging::get()->printf("Inv");
            _reducedCost += newCost - _cost;
            _cost = newCost;
            return false;
        }
        //Debugging::get()->printf("Val");
        return true;
    }

    bool ignore() const { return _ignore; }
    double cost() const { return _cost; }
    double reducedCost() const { return _reducedCost; }
    void reduce(double by) { _reducedCost = _cost - by; }

private:
    const Edge *_edge; //the actual edge in the graph
    bool _validated;
    bool _ignore;
    double _cost;
    double _reducedCost;
};

class PathFindingGraph
{
public:
    PathFindingGraph(const vector<Vertex> &vertices, const vector<Edge> &edges)
        : _vertices(vertices), _edges(edges)
    {
        _vData.resize(vertices.size());
        _eData.reserve(edges.size());
        for(size_t i = 0; i < edges.size(); ++i)
            _eData.push_back(PathFindingEdgeData(&(edges[i])));

        for(size_t i = 0; i < vertices.size(); ++i)
            _vData[i].numOutgoing = vertices[i].edges.size();
        for(size_t i = 0; i < edges.size(); ++i)
            _vData[edges[i].endVtx].numIncoming++;
    }

    vector<int> shortestPath(const Fitter &fitter)
    {
        vector<int> sources;
        for(int i = 0; i < (int)_vertices.size(); ++i)
        {
            if(_vertices[i].source)
                sources.push_back(i);
            _vData[i].source = _vertices[i].source;
            _vData[i].target = _vertices[i].target;
        }

        int reduceEvery = max(1, (int)fitter.params().get(Parameters::REDUCE_GRAPH_EVERY));
        vector<int> sp;

        for(int i = 0; i < _maxIter; ++i)
        {
            if(i % reduceEvery == 0)
                _reduceForPath(sources);

            sp = _shortestPath(sources);

            bool done = true;
            for(int j = 0; j < (int)sp.size(); ++j)
                done = _eData[sp[j]].validate(fitter) && done;

            if(done)
                break;
        }

        //debugging output
        double total = 0;
        for(int j = 0; j < (int)sp.size(); ++j)
            total += _eData[sp[j]].cost();
        Debugging::get()->printf("Found path, len = %d, cost = %lf", sp.size(), total);

        return sp;
    }

    vector<int> shortestCycle(const Fitter &fitter)
    {
        //start with the vertex that has an edge both cheap and with very connected vertices
        double minEdgeCost = Parameters::infinity;
        int bestEdge = 0;

        for(size_t i = 0; i < _eData.size(); ++i)
        {
            double cost = _eData[i].cost() - double(_vData[_edges[i].startVtx].numIncoming) * _vData[_edges[i].endVtx].numOutgoing;
            if(cost < minEdgeCost)
            {
                minEdgeCost = cost;
                bestEdge = i;
            }
        }

        vector<int> sources(1, _edges[bestEdge].startVtx);

        int reduceEvery = max(1, (int)fitter.params().get(Parameters::REDUCE_GRAPH_EVERY));

        vector<int> sp;

        for(int iter = 0; iter < 2; ++iter)
        {
            _vData[sources[0]].source = _vData[sources[0]].target = true;

            for(int i = 0; i < _maxIter; ++i)
            {
                if(i % reduceEvery == 0)
                    _reduceForCycle(sources[0]);

                sp = _shortestPath(sources);

                if(sp.empty()) //should not happen
                    return sp;

                bool done = true;
                for(int j = 0; j < (int)sp.size(); ++j)
                    done = _eData[sp[j]].validate(fitter) && done;

                if(done)
                    break;
            }

            _vData[sources[0]].source = _vData[sources[0]].target = false;

            sources[0] = _edges[sp[sp.size() / 2]].endVtx; //the new source is the middlemost vertex

            //debugging output
            double total = 0;
            for(int j = 0; j < (int)sp.size(); ++j)
                total += _eData[sp[j]].cost();
            Debugging::get()->printf("Found cycle, len = %d, cost = %lf", sp.size(), total);
        }

        return sp;
    }

private:
    static const int _maxIter = 10000;

    void _reduceForPath(const vector<int> &sourceVertices)
    {
        //compute distances
        for(int i = 0; i < (int)_vertices.size(); ++i)
            _vData[i].distance = _vData[i].target ? 0. : Parameters::infinity;

        for(int i = (int)_edges.size() - 1; i >= 0; --i)
        {
            if(_eData[i].ignore())
                continue;
            int src = _edges[i].startVtx;
            int tgt = _edges[i].endVtx;

            _vData[src].distance = min(_vData[src].distance, _eData[i].cost() + _vData[tgt].distance);
        }

        //reduce
        const double reductionTol = 1e-8;
        double minDist = Parameters::infinity;
        for(int i = 0; i < (int)sourceVertices.size(); ++i)
            minDist = min(minDist, _vData[sourceVertices[i]].distance);

        for(int i = 0; i < (int)_edges.size(); ++i) {
            if(_eData[i].ignore())
                continue;

            int src = _edges[i].startVtx;
            int tgt = _edges[i].endVtx;
            
            if(_vData[src].source)
                _eData[i].reduce(minDist - _vData[tgt].distance - reductionTol);
            else
                _eData[i].reduce(_vData[src].distance - _vData[tgt].distance - reductionTol);

            if(_eData[i].reducedCost() < 0.)
                Debugging::get()->printf("Reducing error!");
        }
    }

    void _reduceForCycle(int vertex)
    {
        //compute distances
        for(int i = 0; i < (int)_vertices.size(); ++i)
            _vData[i].distance = Parameters::infinity;
        _vData[vertex].distance = 0.;

        int startEdge = _vertices[vertex].edges[0] - 1;
        int lastSourceEdge = _vertices[vertex].edges.back();

        size_t count = 0;

        for(int i = startEdge; count < _edges.size(); --i, ++count)
        {
            if(i < 0)
                i = (int)_edges.size() - 1;

            if(i == lastSourceEdge)
                _vData[vertex].distance = Parameters::infinity;

            if(_eData[i].ignore())
                continue;

            int src = _edges[i].startVtx;
            int tgt = _edges[i].endVtx;

            _vData[src].distance = min(_vData[src].distance, _eData[i].cost() + _vData[tgt].distance);
        }

        //reduce
        const double reductionTol = 1e-8;

        for(int i = 0; i < (int)_edges.size(); ++i) {
            if(_eData[i].ignore())
                continue;

            int src = _edges[i].startVtx;
            int tgt = _edges[i].endVtx;
            
            if(_vData[tgt].target)
            {
                _eData[i].reduce(_vData[src].distance - reductionTol);
                continue;
            }
            else
            {
                bool crossesSource;
                if(src < tgt)
                    crossesSource = src < vertex && vertex < tgt;
                else
                    crossesSource = src < vertex || vertex < tgt;

                if(!crossesSource && _vData[src].distance < Parameters::infinity) //if the edge does not cross the starting vertex
                    _eData[i].reduce(_vData[src].distance - _vData[tgt].distance - reductionTol);
                else
                    _eData[i].reduce(0);
            }

            if(_eData[i].reducedCost() < 0.)
                Debugging::get()->printf("Reducing error!");
        }
    }

    vector<int> _shortestPath(const vector<int> &sourceVertices)
    {
        for(size_t i = 0; i < _vertices.size(); ++i)
        {
            _vData[i].prevEdge = -1;
            _vData[i].distance = Parameters::infinity;
            _vData[i].finished = false;
        }

        priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > todo;

        //initialize
        for(int i = 0; i < (int)sourceVertices.size(); ++i)
            todo.push(make_pair(0., sourceVertices[i]));

        //run
        while(!todo.empty())
        {
            double curDistance = todo.top().first;
            int v = todo.top().second;
            todo.pop();

            if(_vData[v].target && _vData[v].prevEdge >= 0) //done, now traverse the edges backwards
            {
                vector<int> out;

                int cur = v;
                do
                {
                    out.push_back(_vData[cur].prevEdge);
                    cur = _edges[out.back()].startVtx;
                } while(_vData[cur].prevEdge >= 0 && cur != v);

                reverse(out.begin(), out.end());
                return out;
            }

            if(_vData[v].finished)
                continue;
            _vData[v].finished = true;

            for(int i = 0; i < (int)_vertices[v].edges.size(); ++i)
            {
                int e = _vertices[v].edges[i];
                if(_eData[e].ignore())
                    continue;
                int tgt = _edges[e].endVtx;
                double newDist = curDistance + _eData[e].reducedCost();

                if(newDist < _vData[tgt].distance)
                {
                    _vData[tgt].distance = newDist;
                    _vData[tgt].prevEdge = e;
                    todo.push(make_pair(newDist, tgt));
                }
            }
        }

        //should not get here--no path
        return vector<int>();
    }

    const vector<Vertex> &_vertices;
    const vector<Edge> &_edges;
    vector<PathFindingEdgeData> _eData;
    vector<PathFindingVertexData> _vData;
};

class DefaultPathFinder : public Algorithm<PATH_FINDING>
{
public:
    string name() const { return "Default"; }

protected:
    void _run(const Fitter &fitter, AlgorithmOutput<PATH_FINDING> &out)
    {
        smart_ptr<const AlgorithmOutput<GRAPH_CONSTRUCTION> > graph = fitter.output<GRAPH_CONSTRUCTION>();
        const vector<FitPrimitive> &primitives = fitter.output<PRIMITIVE_FITTING>()->primitives;

        //construct the path finding graph
        PathFindingGraph pfgraph(graph->vertices, graph->edges);
        
        bool closed = fitter.output<CURVE_CLOSING>()->closed;

        vector<int> shortestPath;
        if(closed)
            shortestPath = pfgraph.shortestCycle(fitter);
        else
            shortestPath = pfgraph.shortestPath(fitter);

        //debugging output
        ostringstream ss;
        for(int i = 0; i < (int)shortestPath.size(); ++i)
        {
            char curveTypes[3] = { 'L', 'A', 'C' }; //line, arc, clothoid
            ss << curveTypes[primitives[graph->edges[shortestPath[i]].startVtx].curve->getType()];
            if(graph->edges[shortestPath[i]].continuity == -1)
                break;
            ss << "-" << graph->edges[shortestPath[i]].continuity << "-";
            if(!closed && i + 1 == (int)shortestPath.size())
                ss << curveTypes[primitives[graph->edges[shortestPath[i]].endVtx].curve->getType()];
        }
        Debugging::get()->printf("Curves = %s", ss.str().c_str());

        for(int i = 0; i < (int)shortestPath.size(); ++i)
        {
            Debugging::get()->drawPrimitive(primitives[graph->edges[shortestPath[i]].startVtx].curve, "Path", i);
        }
        if(shortestPath.size() > 0 && graph->edges[shortestPath[0]].continuity != -1)
            Debugging::get()->drawPrimitive(primitives[graph->edges[shortestPath.back()].endVtx].curve, "Path", shortestPath.size());

        out.path = shortestPath;
    }
};

void Algorithm<PATH_FINDING>::_initialize()
{
    new DefaultPathFinder();
}

END_NAMESPACE_Cornu



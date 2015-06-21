#ifndef GRAPHDRAW_H_INCLUDED
#define GRAPHDRAW_H_INCLUDED

#include <vector>
#include "spacevect.h"
#include "graph.h"

class GraphDraw
{
    public:
        GraphDraw(const Graph &_graph);
        void step();
        void render();

        bool finished() const {return f;}
        void reset() {f = false;}
        void graphNodeRemoved(int node);
        const Graph& getGraph() const {return graph;}
        const std::vector<SpaceVect>& getCoords() const {return pos;}
        void getCenterAndRadius(SpaceVect &center, double &radius);

    private:
        GraphDraw();

        const Graph &graph;
        std::vector<SpaceVect> velo;
        std::vector<SpaceVect> pos;
        bool f;
};

#endif // GRAPHDRAW_H_INCLUDED
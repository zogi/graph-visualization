#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "color.h"

class Graph
{
    friend std::istream& operator>> (std::istream &inp, Graph& graph);

    public:
        struct dfsData
        {
            std::vector<int> depth;
            std::vector<int> finish;
            std::set<std::pair<int, int> > treeEdges;
            std::set<std::pair<int, int> > forwardEdges;
            std::set<std::pair<int, int> > backwardEdges;
            std::set<std::pair<int, int> > crossEdges;
        };

        Graph() {}
        Graph(unsigned int nodeCount);

        void addEdge(int from, int to);
        void removeEdge(int from, int to);
        void addNode();
        void addNode(std::string lbl);
        void removeNode(int node);
        void clear();

        std::string getLabel(int i) const;
        void setLabel(int node, std::string lbl);

        Color getNodeColor(int node) const;
        void setNodeColor(int node, const Color& color);
        Color getEdgeColor(int from, int to) const;
        void setEdgeColor(int from, int to, const Color& color);

        bool isLinked(int node1, int node2) const;
        unsigned int size() const {return outEdges.size();}

        void dfs(dfsData &data) const;

        const std::set<int>& outEdgeList(int node) const {return outEdges[node];}

    protected:
        std::vector<std::set<int> > outEdges;
        std::map<int, std::string> label;
        std::map<int, Color> nodeColor;
        std::map<std::pair<int, int>, Color> edgeColor;

        void dfs_traverse(int node, std::vector<bool> &visited, int &time, dfsData &data) const;
};

std::istream& operator>> (std::istream &inp, Graph& graph);

#endif // GRAPH_H_INCLUDED
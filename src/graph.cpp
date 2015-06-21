#include "graph.h"

using namespace std;

typedef pair<int, int> PII;

Graph::Graph(unsigned int nodeCount): outEdges(nodeCount, set<int>())
{
}

void Graph::addEdge(int from, int to)
{
    outEdges[from].insert(to);
}

void Graph::removeEdge(int from, int to)
{
    outEdges[from].erase(to);
    edgeColor.erase(PII(from, to));
}

void Graph::addNode()
{
    outEdges.push_back(set<int>());
}

void Graph::addNode(std::string lbl)
{
    label[outEdges.size()] = lbl;
    outEdges.push_back(set<int>());
}

void Graph::removeNode(int node)
{
    if (node < 0 || node >= static_cast<int>(outEdges.size()))
        throw "invalid node index";

    set<int> nodeSet;
    map<PII, Color> newEdgeColor;

    // fix labels and node coloring
    for (int i = 0; i < static_cast<int>(outEdges.size()); ++i)
    {
        if (i > node)
        {
            label[i - 1] = label[i];
            nodeColor[i - 1] = nodeColor[i];
        }
    }
    label.erase(outEdges.size() - 1);
    nodeColor.erase(outEdges.size() - 1);

    // fix edge coloring
    PII edge;
    for (map<PII, Color>::iterator it = edgeColor.begin(); it != edgeColor.end(); ++it)
    {
        edge = it->first;
        if (edge.first != node && edge.second != node)
        {
            if (node < edge.first)
                --edge.first;

            if (node < edge.second)
                --edge.second;

            newEdgeColor.insert(pair<PII, Color>(edge, it->second));
        }
    }
    edgeColor = newEdgeColor;

    // fix edge lists
    for(int i = 0; i < static_cast<int>(outEdges.size()); ++i)
    {
        if (i != node)
        {
            nodeSet.clear();
            for (set<int>::iterator it = outEdges[i].begin(); it != outEdges[i].end(); ++it)
            {
                if (node != *it)
                {
                    if (node < *it)
                        nodeSet.insert(*it - 1);
                    else
                        nodeSet.insert(*it);
                }
            }
            if (i > node)
                outEdges[i - 1] = nodeSet;
            else
                outEdges[i] = nodeSet;
        }
    }
    outEdges.pop_back();
}

void Graph::clear()
{
    outEdges.clear();
    label.clear();
}

string Graph::getLabel(int node) const
{
    map<int, string>::const_iterator it = label.find(node);

    if (it == label.end())
        return "";
    else
        return it->second;
}

void Graph::setLabel(int node, string lbl)
{
    label[node] = lbl;
}

Color Graph::getNodeColor(int node) const
{
    map<int, Color>::const_iterator it = nodeColor.find(node);

    if (it == nodeColor.end())
        return Color::White;
    else
        return it->second;
}

void Graph::setNodeColor(int node, const Color& color)
{
    nodeColor[node] = color;
}

Color Graph::getEdgeColor(int from, int to) const
{
    map<PII, Color>::const_iterator it = edgeColor.find(PII(from, to));

    if (it == edgeColor.end())
        return Color::White;
    else
        return it->second;
}

void Graph::setEdgeColor(int from, int to, const Color& color)
{
    edgeColor[PII(from, to)] = color;
    edgeColor[PII(to, from)] = color;
}

bool Graph::isLinked(int node1, int node2) const
{
    return outEdges[node1].find(node2) != outEdges[node1].end() || outEdges[node2].find(node1) != outEdges[node2].end();
}

void Graph::dfs(dfsData &data) const
{
    int time = 1;

    data.depth.clear();
    data.finish.clear();

    data.depth.resize(outEdges.size(), 0);
    data.finish.resize(outEdges.size(), 0);

    data.treeEdges.clear();
    data.crossEdges.clear();
    data.forwardEdges.clear();
    data.backwardEdges.clear();

    vector<bool> visited(outEdges.size(), false);
    for (unsigned int node = 0; node < outEdges.size(); ++node)
    {
        if (!visited[node])
        {
            dfs_traverse(node, visited, time, data);
        }
    }
}

void Graph::dfs_traverse(int node, vector<bool> &visited, int &time, dfsData &data) const
{
    visited[node] = true;

    data.depth[node] = time++;

    for (set<int>::const_iterator it = outEdges[node].begin(); it != outEdges[node].end(); ++it)
    {
        if (data.depth[*it] == 0)
        {
            data.treeEdges.insert(PII(node, *it));
            dfs_traverse(*it, visited, time, data);
        }
        else if (data.depth[*it] <= data.depth[node] && data.finish[*it] == 0)
        {
            data.backwardEdges.insert(PII(node, *it));
        }
        else if (data.depth[*it] > data.depth[node])
        {
            data.forwardEdges.insert(PII(node, *it));
        }
        else
        {
            data.crossEdges.insert(PII(node, *it));
        }
    }

    data.finish[node] = time++;
}

istream& operator>> (istream &inp, Graph& graph)
{
    int n, m, ind, parent, child;
    string s;
    map<string, int> nodeIndex;
    map<string, int>::iterator it;

    graph.clear();

    inp >> n;

    ind = 0;
    for (int i = 0; i < n; ++i)
    {
        inp >> s;

        it = nodeIndex.find(s);
        if (it == nodeIndex.end())
        {
            parent = ind;
            nodeIndex[s] = ind;
            graph.addNode(s);
            ++ind;
        }
        else
        {
            parent = it->second;
        }

        inp >> m;
        for (int j = 0; j < m; ++j)
        {
            inp >> s;
            it = nodeIndex.find(s);
            if (it == nodeIndex.end())
            {
                child = ind;
                nodeIndex[s] = ind;
                graph.addNode(s);
                ++ind;
            }
            else
            {
                child = it->second;
            }

            graph.addEdge(parent, child);
        }
    }

    return inp;
}
#include <cmath>
#include <algorithm>

#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glut.h>

#include "graphdraw.h"

using namespace std;

const double pi = 3.14159265;
const double pi2 = 2 * pi;
const double radToDeg = 180.0 / pi;

const double nodeRadius = 0.2f;
const double coneBase = 0.1f;
const double coneHeight = 0.4f;

const double epsilon = 0.00001;
const double timestep = 0.1;
const double damping = 0.5;
const double repulsion_constant = 20.0;
const double attraction_constant = 1;

GraphDraw::GraphDraw(const Graph &_graph): graph(_graph), velo(_graph.size(), SpaceVect()), pos(_graph.size(), SpaceVect()), f(false)
{
    // The simulation requires the initial coordinates to be different
    for (unsigned int i = 0; i < pos.size(); ++i)
    {
        pos[i].x = sin(pi2 * i / pos.size()) * 1.0;
        pos[i].y = cos(pi2 * i / pos.size()) * 1.0;
        pos[i].z = i * 1.0;
    }
}

void GraphDraw::graphNodeRemoved(int node)
{
    for (int i = node; i < static_cast<int>(pos.size()) - 1; ++i)
    {
        pos[i] = pos[i + 1];
        velo[i] = velo[i + 1];
    }
    pos.pop_back();
    velo.pop_back();
}

void GraphDraw::getCenterAndRadius(SpaceVect &center, double &radius)
{
    center = SpaceVect(0.0, 0.0, 0.0);
    for (unsigned int i = 0; i < pos.size(); ++i)
        center += pos[i];

    center = (1.0 / pos.size()) * center;

    radius = 1.5;
    for  (unsigned int i = 0; i < pos.size(); ++i)
        radius = std::max(radius, (center - pos[i]).length());
}

void GraphDraw::step()
{
    if (!f)
    {
		// Force-based gragh drawing from wikipedia

        double total_kinetic_energy, dot;
        SpaceVect net_force;

        total_kinetic_energy = 0; // running sum of total kinetic energy over all particles
        for (unsigned int i = 0; i < graph.size(); ++i)
        {
            net_force = SpaceVect(0.0, 0.0, 0.0); // running sum of total force on this particular node

            for (unsigned int j = 0; j < graph.size(); ++j)
            {
                if (j != i)
                {
                    dot = dotProd(pos[i] - pos[j], pos[i] - pos[j]);
                    net_force += (repulsion_constant / (dot * sqrt(dot))) * (pos[i] - pos[j]);
                }
            }

            for (unsigned int j = 0; j < graph.size(); ++j)
            {
                if (graph.isLinked(i, j))
                    net_force += attraction_constant * (pos[j] - pos[i]);
            }

            // without damping, it moves forever
            velo[i] = damping * (velo[i] + timestep * net_force);
            pos[i] += timestep * velo[i];
            total_kinetic_energy += dotProd(velo[i], velo[i]); //0.5 * m * v^2

        }

        f = (total_kinetic_energy < epsilon); //the simulation has stopped moving
    }
}

void GraphDraw::render()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < static_cast<int>(graph.size()); ++i)
    {
        // node
        graph.getNodeColor(i).apply();
        glPushMatrix();
            glTranslatef(pos[i].x, pos[i].y, pos[i].z);
            glutSolidSphere(nodeRadius, 10, 10);
        glPopMatrix();

        // edges
        glBegin(GL_LINES);
            for (unsigned int j = i + 1; j < graph.size(); ++j)
            {
                if (graph.isLinked(i, j))
                {
                    graph.getEdgeColor(i, j).apply();
                    glVertex3f(pos[i].x, pos[i].y, pos[i].z);
                    glVertex3f(pos[j].x, pos[j].y, pos[j].z);
                }
            }
        glEnd();

        // edge arrowheads
        for (set<int>::const_iterator it = graph.outEdgeList(i).begin(); it != graph.outEdgeList(i).end(); ++it)
        {
            graph.getEdgeColor(i, *it).apply();
            glPushMatrix();
                SpaceVect axis = crossProd(ZAxis, pos[*it] - pos[i]);
                glTranslatef(pos[*it].x, pos[*it].y, pos[*it].z);
                glRotatef(vectAngle(pos[*it] - pos[i], ZAxis) * radToDeg, axis.x, axis.y, axis.z);
                glTranslatef(0.0f, 0.0f, -coneHeight - nodeRadius);
                glutSolidCone(coneBase, coneHeight, 10, 10);
            glPopMatrix();
        }
    }
}
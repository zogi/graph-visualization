#include <set>
#include <map>
#include <sstream>

#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glut.h>

#include "mainwindow.h"
#include "keyboard.h"
#include "mouse.h"
#include "matrix.h"

using namespace std;

namespace {

	void* bitmapFont = GLUT_BITMAP_TIMES_ROMAN_24;

	double camRadiusMpler = 2.0;
	const double camRadiusPlus = 1.5;
	const double camMotionConst = 0.05;

	const Color& processNodeColor = Color::Blue;
	const Color& resourceNodeColor = Color::Green;

	const char* phaseMessage[] = {
		"Press SPACE to detect cycle.",
		"Cycle detected. Press SPACE to remove node from the cycle.",
		"No cycle found. A topological ordering: ",
	};

}

MainWindow *MainWindow::instance = NULL;

MainWindow::MainWindow(string title, int width, int height, const Graph &_graph): w(width), h(height), graph(_graph), gd(graph)
{
    if (instance != NULL)
        throw "Can't create multiple instances of MainWindow";

    instance = this;

    phase = DETECTCYCLE;

    // init graph drawer
    for (unsigned int i = 0; i < graph.size(); ++i)
    {
        if (graph.getLabel(i).length() > 0 && graph.getLabel(i)[0] == 'f')
            graph.setNodeColor(i, processNodeColor);
        else
            graph.setNodeColor(i, resourceNodeColor);

        if (graph.getLabel(i).length() > 0)
            graph.setLabel(i, graph.getLabel(i).substr(1));

        for (set<int>::const_iterator it = graph.outEdgeList(i).begin(); it != graph.outEdgeList(i).end(); ++it)
            graph.setEdgeColor(i, *it, Color::Gray);
    }

    // init GL
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow(title.c_str());

	initScene();

	// input
	Keyboard::init();
	Keyboard::registerKeyUpCallback(keyboardProc);
    Mouse::init();
    Mouse::registerCallback(mouseProc);

    // callback registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

    // mainloop
	glutMainLoop();
}

void MainWindow::initScene()
{
    // light params
    const GLfloat light_ambient[]  = { 0.1f, 0.1f, 0.1f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 0.0f, 0.0f, 0.0f, 0.0f };
    const GLfloat high_shininess[] = { 100.0f };

    // Options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLineWidth(3.0f);

    // Light and material
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    // Camera
    cam.setPos(SpaceVect(0.0, 0.0, -2.0));
    cam.setDir(SpaceVect(0.0, 0.0, 1.0));
    cam.setUp(SpaceVect(0.0, 1.0, 0.0));
}

void MainWindow::changeSize(int wid, int hgt)
{
    instance->w = wid;
    instance->h = hgt;

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(hgt == 0)
        hgt = 1;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, wid, hgt);

    // Set the clipping volume
    gluPerspective(45, 1.0 * wid / hgt, 1, 1000);
    glMatrixMode(GL_MODELVIEW);

    instance->cam.update();
}

/// RENDER SCENE
void MainWindow::renderScene()
{
    Phases &phase = instance->phase;
    GraphDraw &gd = instance->gd;
    deque<string> &order = instance->order;
    Camera &cam = instance->cam;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // adjust zoom
    if (Keyboard::getState('+'))
    {
        camRadiusMpler -= 0.1;
        if (camRadiusMpler < 1.0)
            camRadiusMpler  = 1.0;
    }
    if (Keyboard::getState('-'))
    {
        camRadiusMpler += 0.1;
    }

    // draw graph
    gd.step();
    gd.render();

    // draw labels
    for (unsigned int i = 0; i < instance->graph.size(); ++i)
        instance->renderText(instance->graph.getLabel(i), instance->gd.getCoords()[i]);

    // display text
    if (phase == DETECTCYCLE)
    {
        instance->renderText(phaseMessage[0], 5, 5);
    }
    else if (phase == ERASENODE)
    {
        instance->renderText(phaseMessage[1], 5, 5);
    }
    else if (phase == ACYCLIC)
    {
        // word wrap
        bool fTooSmall;
        int sepWid = glutBitmapWidth(bitmapFont, ' ') + glutBitmapWidth(bitmapFont, ',');
        int wordWid, lineWidth;
        string sLine;
        deque<string> lines;
        deque<string>::iterator itNext;

        fTooSmall = false;
        lineWidth = 0;
        itNext = order.begin();
        for (deque<string>::iterator it = order.begin(); it != order.end(); ++it)
        {
            wordWid = 0;
            for (unsigned int i = 0; i < it->length(); ++i)
                wordWid += glutBitmapWidth(bitmapFont, (*it)[i]);

            if ((5 + lineWidth + sepWid + wordWid) > instance->w)
            {
                if (lineWidth == 0)
                {
                    fTooSmall = true;
                    break;
                }
                else
                {
                    lineWidth = 0;
                    sLine = *itNext;
                    for (++itNext; itNext != it; ++itNext)
                        sLine += ", " + *itNext;

                    lines.push_back(sLine);
                }
            }
            else
            {
                lineWidth += sepWid + wordWid;
            }
        }
		// last line
        sLine = *itNext;
        for (++itNext; itNext != order.end(); ++itNext)
            sLine += ", " + *itNext;
        lines.push_back(sLine);

        if (!fTooSmall)
        {
            instance->renderText(phaseMessage[2], 5, 5 + 25 * lines.size());
            int y = lines.size() - 1;
            for (deque<string>::iterator it = lines.begin(); it != lines.end(); ++it)
            {
                instance->renderText(*it, 5, 5 + 25 * y--);
            }
        }
    }

    // adjust camera
    SpaceVect center;
    double radius = 0;
    gd.getCenterAndRadius(center, radius);
    cam.setPos(camMotionConst * (center - (camRadiusMpler * radius + camRadiusPlus) * cam.getDir()) + (1 - camMotionConst) * cam.getPos());
    cam.update();

    glFinish();
	glutSwapBuffers();
	glutPostRedisplay();
}

void MainWindow::mouseProc()
{
    GraphDraw &gd = instance->gd;
    Camera &cam = instance->cam;

    if (Mouse::getState(GLUT_LEFT_BUTTON))
    {
        SpaceVect oldDir = cam.getDir();
        cam.rotate(Mouse::getX() - Mouse::getLastX(), Mouse::getY() - Mouse::getLastY());

        SpaceVect center;
        double radius = 0;

        gd.getCenterAndRadius(center, radius);
        cam.setPos(cam.getPos() + (camRadiusMpler * radius + camRadiusPlus) * (oldDir - cam.getDir()));
    }
}

void MainWindow::keyboardProc(unsigned char key, int x, int y)
{
    Phases &phase = instance->phase;
    Graph &graph = instance->graph;
    Graph::dfsData &data = instance->data;
    GraphDraw &gd = instance->gd;
    deque<string> &order = instance->order;

    if (key == ' ')
    {
        if (phase == DETECTCYCLE)
        {
            // depth-first search
            graph.dfs(data);

            if (data.backwardEdges.empty())
            {
                //topological order
                map<int, int> tpl;

                for (unsigned int i = 0; i < data.finish.size(); ++i)
                    tpl[data.finish[i]] = i;

                for (map<int, int>::iterator it = tpl.begin(); it != tpl.end(); ++it)
                    if (graph.getNodeColor(it->second) == processNodeColor)
                        order.push_back(graph.getLabel(it->second));

                // next phase
                phase = ACYCLIC;
            }
            else
            {
                // reverse tree edges
                map<int, int> revTreeEdges;
                for (set<pair<int, int> >::iterator it = data.treeEdges.begin(); it != data.treeEdges.end(); ++it)
                    revTreeEdges[it->second] = it->first;

                // color the cycle
                graph.setEdgeColor(data.backwardEdges.begin()->first, data.backwardEdges.begin()->second, Color::Red);
                for (int node = data.backwardEdges.begin()->first; node != data.backwardEdges.begin()->second; node = revTreeEdges[node])
                    graph.setEdgeColor(node, revTreeEdges[node], Color::Red);

                // next phase
                phase = ERASENODE;
            }
        }
        else if (phase == ERASENODE)
        {
            // remove node
            if (graph.getNodeColor(data.backwardEdges.begin()->first) == processNodeColor)
            {
                graph.removeNode(data.backwardEdges.begin()->first);
                gd.graphNodeRemoved(data.backwardEdges.begin()->first);
            }
            else
            {
                graph.removeNode(data.backwardEdges.begin()->second);
                gd.graphNodeRemoved(data.backwardEdges.begin()->second);
            }

            gd.reset();

            // restore edge coloring
            for (unsigned int i = 0; i < graph.size(); ++i)
                for (set<int>::const_iterator it = graph.outEdgeList(i).begin(); it != graph.outEdgeList(i).end(); ++it)
                    graph.setEdgeColor(i, *it, Color::Gray);


            // next phase
            phase = DETECTCYCLE;
        }
    }
    else if (key == Keyboard::KEY_ESCAPE)
    {
        glFinish();
        glutDestroyWindow(glutGetWindow());
        glFinish();
        exit(0);
    }
}

void MainWindow::renderText(const string &text, int x, int y)
{
    // switch to orthogonal projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);

    // save modelview
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // set black color
    glColor3f(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < text.size(); ++i)
    {
        glRasterPos2i(x, y);
        glutBitmapCharacter(bitmapFont, text[i]);
        x += glutBitmapWidth(bitmapFont, text[i]);
    }

    // restore matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

#include <iostream>
void MainWindow::renderText(const string &text, const SpaceVect& pos)
{
    // we need the projection matrix
    Matrix proj;
    glGetFloatv(GL_PROJECTION_MATRIX, proj.m);

    Matrix model;
    glGetFloatv(GL_MODELVIEW_MATRIX, model.m);

    Matrix mat = proj * model;

    SpaceVect v = mat.transformVect(pos);
    v.homogenize();

    renderText(text, static_cast<int>((v.x + 1) * w * 0.5), static_cast<int>((v.y + 1) * h * 0.5));
}
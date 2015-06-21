#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include <string>
#include <deque>
#include "graphdraw.h"
#include "graph.h"
#include "camera.h"
#include "spacevect.h"

class MainWindow
{
    public:
        MainWindow(std::string title, int width, int height, const Graph &_graph);

        Camera& getCamera() {return cam;}
        int getWidth() const {return w;}
        int getHeight() const {return h;}
        void renderText(const std::string &text, int x, int y);
        void renderText(const std::string &text, const SpaceVect &pos);

    private:
        enum Phases {DETECTCYCLE, ERASENODE, ACYCLIC};

        int w, h;
        Phases phase;
        Camera cam;
        Graph graph;
        GraphDraw gd;
        Graph::dfsData data;
        std::deque<std::string> order;
        void initScene();

        static MainWindow *instance;
        static void changeSize(int wid, int hgt);
        static void renderScene();
        static void mouseProc();
        static void keyboardProc(unsigned char key, int x, int y);
};

#endif // MAINWINDOW_H_INCLUDED
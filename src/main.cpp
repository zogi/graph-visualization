#include <iostream>
#include <fstream>
#include <string>

#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glut.h>

#include "mainwindow.h"
#include "graph.h"

using namespace std;

int main (int argc, char *argv[])
{
    const char *filename;

	if (argc > 1) {
		filename = argv[1];
	} else {
		filename = "input.txt";
	}

    ifstream fin(filename);
    Graph graph;
    fin >> graph;
	
	glutInit(&argc, argv);
	MainWindow window("Graph demo", 640, 480, graph);

	return 0;
}
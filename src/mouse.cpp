#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glut.h>

#include "mouse.h"

int Mouse::posX;
int Mouse::posY;
int Mouse::lastX;
int Mouse::lastY;
bool Mouse::state[8];
void (*Mouse::callbackFunc)() = NULL;

using namespace std;

void Mouse::init()
{
    glutMouseFunc(mouseFunc);
    glutMotionFunc(mouseMotionFunc);
    glutPassiveMotionFunc(mouseMotionFunc);
}

bool Mouse::getState(int button)
{
    if (button < 0 || button > 8)
        return false;
    else
        return state[button];
}

void Mouse::mouseFunc(int button, int mouseState, int x, int y)
{
    lastX = posX;
    lastY = posY;
    posX = x;
    posY = y;
    state[button] = (mouseState == GLUT_DOWN);

    if (callbackFunc != NULL)
        (*callbackFunc)();
}

void Mouse::mouseMotionFunc(int x, int y)
{
    lastX = posX;
    lastY = posY;
    posX = x;
    posY = y;

    if (callbackFunc != NULL)
        (*callbackFunc)();
}
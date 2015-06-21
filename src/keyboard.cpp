#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glut.h>

#include "keyboard.h"

bool Keyboard::state[128];
void (*Keyboard::keyDownCallback)(unsigned char key, int x, int y) = NULL;
void (*Keyboard::keyUpCallback)(unsigned char key, int x, int y) = NULL;

void Keyboard::init()
{
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
}

bool Keyboard::getState(int key)
{
    if (key < 0 || key > 127)
        return false;
    else
        return state[key];
}

void Keyboard::keyDown(unsigned char key, int x, int y)
{
    state[key] = true;

    if (keyDownCallback != NULL)
        (*keyDownCallback)(key, x, y);
}

void Keyboard::keyUp(unsigned char key, int x, int y)
{
    state[key] = false;

    if (keyUpCallback != NULL)
        (*keyUpCallback)(key, x, y);
}

void Keyboard::specialKeyDown(int key, int x, int y)
{
    if (key >= GLUT_KEY_F1 && key <= GLUT_KEY_F12)
    {
        state[KEY_F1 + key - GLUT_KEY_F1] = true;
    }
    else switch (key)
    {
        case GLUT_KEY_LEFT:
            state[KEY_LEFT] = true;
            break;
        case GLUT_KEY_UP:
            state[KEY_UP] = true;
            break;
        case GLUT_KEY_RIGHT:
            state[KEY_RIGHT] = true;
            break;
        case GLUT_KEY_DOWN:
            state[KEY_DOWN] = true;
            break;
        case GLUT_KEY_PAGE_UP:
            state[KEY_PGUP] = true;
            break;
        case GLUT_KEY_PAGE_DOWN:
            state[KEY_PGDN] = true;
            break;
        case GLUT_KEY_HOME:
            state[KEY_HOME] = true;
            break;
        case GLUT_KEY_END:
            state[KEY_END] = true;
            break;
        case GLUT_KEY_INSERT:
            state[KEY_INSERT] = true;
            break;
    }
}

void Keyboard::specialKeyUp(int key, int x, int y)
{
    if (key >= GLUT_KEY_F1 && key <= GLUT_KEY_F12)
    {
        state[KEY_F1 + key - GLUT_KEY_F1] = false;
    }
    else switch (key)
    {
        case GLUT_KEY_LEFT:
            state[KEY_LEFT] = false;
            break;
        case GLUT_KEY_UP:
            state[KEY_UP] = false;
            break;
        case GLUT_KEY_RIGHT:
            state[KEY_RIGHT] = false;
            break;
        case GLUT_KEY_DOWN:
            state[KEY_DOWN] = false;
            break;
        case GLUT_KEY_PAGE_UP:
            state[KEY_PGUP] = false;
            break;
        case GLUT_KEY_PAGE_DOWN:
            state[KEY_PGDN] = false;
            break;
        case GLUT_KEY_HOME:
            state[KEY_HOME] = false;
            break;
        case GLUT_KEY_END:
            state[KEY_END] = false;
            break;
        case GLUT_KEY_INSERT:
            state[KEY_INSERT] = false;
            break;
    }
}
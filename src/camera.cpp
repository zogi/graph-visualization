#include <iostream>

#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glut.h>
#include "camera.h"
#include "matrix.h"

using namespace std;

void Camera::update()
{
    glLoadIdentity();

    SpaceVect lookAt = pos + dir;
    gluLookAt(pos.x, pos.y, pos.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z);
}

SpaceVect Camera::getDir() const
{
    SpaceVect v = dir;
    v.normalize();
    return v;
}

SpaceVect Camera::getUp() const
{
    SpaceVect v = up;
    v.normalize();
    return v;
}

SpaceVect Camera::getLeft() const
{
    SpaceVect v = crossProd(up, dir);
    v.normalize();
    return v;
}

void Camera::rotate(float deltaYaw, float deltaPitch)
{
    Matrix rot = Matrix::rotation(-0.01f * deltaYaw, up) * Matrix::rotation(0.01f * deltaPitch, getLeft());
    dir = rot.transformVect(dir);
    up = rot.transformVect(up);
    orthoNormalize();
}

void Camera::orthoNormalize()
{
    up = crossProd(crossProd(dir, up), dir);
    dir = crossProd(crossProd(up, dir), up);

    dir.normalize();
    up.normalize();
}
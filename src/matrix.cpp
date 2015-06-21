#include <iostream>
#include <cmath>
#include "matrix.h"

using namespace std;

Matrix operator*(const Matrix &m1, const Matrix &m2)
{
    int ind;
    Matrix m;

    for (unsigned int i = 0; i < 4; ++i)
        for (unsigned int j = 0; j < 4; ++j)
        {
            ind = i + 4 * j;
            m.m[ind] = 0;
            for (unsigned int k = 0; k < 4; ++k)
                m.m[ind] += m1.m[i + 4 * k] * m2.m[k + 4 * j];
        }
    return m;
}

void Matrix::loadIdentity()
{
    for (unsigned int i = 0; i < 4; ++i)
        for (unsigned int j = 0; j < 4; ++j)
            m[i + 4 * j] = (i == j) ? 1 : 0;
}

Matrix Matrix::rotation(float phi, SpaceVect axis)
{
    double sinPhi = sin(phi);
    double cosPhi = cos(phi);
    double c = 1 - cosPhi;

    Matrix mat;

    axis.normalize();

    // column-major order
    mat.m[0] = axis.x * axis.x * c + cosPhi;
    mat.m[4] = axis.x * axis.y * c - axis.z * sinPhi;
    mat.m[8] = axis.x * axis.z * c + axis.y * sinPhi;
    mat.m[12] = 0;
    mat.m[1] = axis.x * axis.y * c + axis.z * sinPhi;
    mat.m[5] = axis.y * axis.y * c + cosPhi;
    mat.m[9] = axis.y * axis.z * c - axis.x * sinPhi;
    mat.m[13] = 0;
    mat.m[2] = axis.x * axis.z * c - axis.y * sinPhi;
    mat.m[6] = axis.y * axis.z * c + axis.x * sinPhi;
    mat.m[10] = axis.z * axis.z * c + cosPhi;
    mat.m[14] = 0;
    mat.m[3] = 0;
    mat.m[7] = 0;
    mat.m[11] = 0;
    mat.m[15] = 0;

    return mat;
}

SpaceVect Matrix::transformVect(const SpaceVect &v)
{
    return SpaceVect(m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w,
                    m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w,
                    m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
                    m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w);
}
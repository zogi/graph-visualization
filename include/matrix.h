#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "spacevect.h"

struct Matrix
{
    friend Matrix operator*(const Matrix &m1, const Matrix &m2);

    void loadIdentity();
    SpaceVect transformVect(const SpaceVect &v);
    static Matrix rotation(float phi, SpaceVect axis);

    float m[16];   // oszlopfolytonos indexelés (opengl miatt)
};

Matrix operator*(const Matrix &m1, const Matrix &m2);

#endif // MATRIX_H_INCLUDED
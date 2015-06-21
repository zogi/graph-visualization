#ifndef SPACEVECT_H_INCLUDED
#define SPACEVECT_H_INCLUDED

#include <cmath>

struct SpaceVect
{
    friend double dotProd(const SpaceVect &v, const SpaceVect &w);
    friend SpaceVect crossProd(const SpaceVect &v, const SpaceVect &w);
    friend double vectAngle(const SpaceVect &v, const SpaceVect &w);
    friend SpaceVect operator+(const SpaceVect &v, const SpaceVect &w);
    friend SpaceVect operator-(const SpaceVect &v, const SpaceVect &w);
    friend SpaceVect operator-(const SpaceVect &v);
    friend SpaceVect operator*(const double scalar, const SpaceVect &v);
    friend SpaceVect operator*(const SpaceVect &v, const double scalar) {return scalar * v;}

    double x;
    double y;
    double z;
    double w;

    SpaceVect(double _x = 0.0, double _y = 0.0, double _z = 0.0, double _w = 1.0): x(_x), y(_y), z(_z), w(_w) {}

    double length() const {return sqrt(x * x + y * y + z * z) / w;}
    void normalize();
    void homogenize();

    SpaceVect& operator+=(const SpaceVect &v);
    SpaceVect& operator-=(const SpaceVect &v);
};

const SpaceVect XAxis = SpaceVect(1.0, 0.0, 0.0);
const SpaceVect YAxis = SpaceVect(0.0, 1.0, 0.0);
const SpaceVect ZAxis = SpaceVect(0.0, 0.0, 1.0);

double dotProd(const SpaceVect &v, const SpaceVect &w);
SpaceVect operator+(const SpaceVect &v, const SpaceVect &w);
SpaceVect operator-(const SpaceVect &v, const SpaceVect &w);
SpaceVect operator-(const SpaceVect &v);


#endif // SPACEVECT_H_INCLUDED
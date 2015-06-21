#include "spacevect.h"

double dotProd(const SpaceVect &v, const SpaceVect &w)
{
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

SpaceVect crossProd(const SpaceVect &v, const SpaceVect &w)
{
    return SpaceVect(v.y * w.z - v.z * w.y, -v.x * w.z + v.z * w.x, v.x * w.y - v.y * w.x);
}

double vectAngle(const SpaceVect &v, const SpaceVect &w)
{
    double denom = v.length() * w.length();

    if (denom == 0)
        return 0;
    else
        return acos(dotProd(v, w) / denom);
}

SpaceVect operator+(const SpaceVect &v, const SpaceVect &w)
{
    return SpaceVect(v.x + w.x, v.y + w.y, v.z + w.z);
}

SpaceVect operator-(const SpaceVect &v, const SpaceVect &w)
{
    return SpaceVect(v.x - w.x, v.y - w.y, v.z - w.z);
}

SpaceVect operator-(const SpaceVect &v)
{
    return SpaceVect(-v.x, -v.y, -v.z);
}

SpaceVect operator*(const double scalar, const SpaceVect &v)
{
    return SpaceVect(scalar * v.x, scalar * v.y, scalar * v.z);
}

void SpaceVect::normalize()
{
    double len = length();

    if (len != 0)
    {
        x /= len;
        y /= len;
        z /= len;
    }
}

void SpaceVect::homogenize()
{
    x /= w;
    y /= w;
    z /= w;
    w = 1.0;
}

SpaceVect& SpaceVect::operator+=(const SpaceVect &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

SpaceVect& SpaceVect::operator-=(const SpaceVect &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}
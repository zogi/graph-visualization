#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "spacevect.h"

class Camera
{
    public:
        Camera() : pos(), dir(0.0, 0.0, 1.0), up(1.0, 0.0, 0.0) {}

        SpaceVect getPos() const {return pos;}
        SpaceVect getDir() const;
        SpaceVect getUp() const;
        SpaceVect getLeft() const;
        void setPos(const SpaceVect &_pos) {pos = _pos;}
        void setDir(const SpaceVect &_dir) {dir = _dir;}
        void setUp(const SpaceVect &_up) {up = _up;}
        void rotate(float deltaYaw, float deltaPitch);
        void orthoNormalize();
        void update();

    private:
        SpaceVect pos;
        SpaceVect dir;
        SpaceVect up;

};

#endif // CAMERA_H_INCLUDED
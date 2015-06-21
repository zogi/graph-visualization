#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glut.h>
#include "color.h"

const Color Color::White = Color(1.0, 1.0, 1.0);
const Color Color::Black = Color();
const Color Color::Red = Color(1.0, 0.0, 0.0);
const Color Color::Green = Color(0.0, 0.7, 0.0);
const Color Color::Blue = Color(0.1, 0.2, 1.0);
const Color Color::Gray = Color(0.6, 0.6, 0.6);

void Color::apply() const
{
    glColor3f(r, g, b);
}

bool operator==(const Color &c1, const Color &c2)
{
    return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b);
}
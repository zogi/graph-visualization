#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

struct Color
{
    friend bool operator==(const Color &c1, const Color &c2);

    float r;
    float g;
    float b;

    //Color(const Color &c): r(c.r), g(c.g), b(c.b) {}
    Color(float _r = 0.0, float _g = 0.0, float _b = 0.0) : r(_r), g(_g), b(_b) {}
    void apply() const;

    static const Color White;
    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Gray;
};

bool operator==(const Color &c1, const Color &c2);

#endif // COLOR_H_INCLUDED
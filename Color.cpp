#include "Color.h"

double Color::red() const {
    return r;
}

double Color::green() const {
    return g;
}

double Color::blue() const {
    return b;
}

std::istream& operator>>(std::istream& is, Color& c) {
    is >> c.r >> c.g >> c.b;
    return is;
}


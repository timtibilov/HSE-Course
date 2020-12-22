#include "circle.h"


Circle::Circle(const Point &center, double radius) : Ellipse(center, center, radius * 2) {
    r = radius;
}

double Circle::radius() const {
    return r;
}

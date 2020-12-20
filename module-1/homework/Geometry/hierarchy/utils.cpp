#include "utils.h"
#include "const.h"
#include <cmath>

double dist(const Point &a, const Point &b) {
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

double dist(const Point &a) {
    return dist(a, Point());
}

double dot(const Point &a, const Point &b) {
    return a.x * b.x + a.y * b.y;
}

double cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

bool eq(double a, double b) {
    return std::abs(a - b) < EPS;
}

bool le(double a, double b) {
    return b - a - EPS > 0;
}


bool gr(double a, double b) {
    return a - b - EPS > 0;
}

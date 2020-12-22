#include <cmath>
#include "point.h"
#include "const.h"

bool Point::operator==(const Point &p) const {
    return std::abs(x - p.x) < EPS && std::abs(y - p.y) < EPS;
}

bool Point::operator!=(const Point &p) const {
    return !(*this == p);
}

Point Point::operator+(const Point &p) const {
    Point res;
    res.x = x + p.x;
    res.y = y + p.y;
    return res;
}

Point Point::operator-(const Point &p) const {
    Point res;
    res.x = x - p.x;
    res.y = y - p.y;
    return res;
}

Point Point::operator*(double n) const {
    Point res;
    res.x = x * n;
    res.y = y * n;
    return res;
}

Point Point::rotate(double ang) const {
    double new_x = x * cos(ang) - y * sin(ang);
    double new_y = x * sin(ang) + y * cos(ang);
    return {new_x, new_y};
}



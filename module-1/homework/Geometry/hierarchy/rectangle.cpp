#include <utility>
#include <cmath>
#include "rectangle.h"
#include "utils.h"
#include "const.h"


Rectangle::Rectangle(const Point &top_left, const Point &bottom_right, double coefficient) : Polygon() {
    Point b = top_left;
    Point d = bottom_right;
    double angle = atan(coefficient);
    double diag_len = Utils::dist(d, b);
    double ab_len = diag_len * cos(angle);
    double bc_len = diag_len * sin(angle);
    Point a = (d - b).rotate(angle) * (ab_len / diag_len) + b;
    Point c = (d - b).rotate(angle - PI / 2) * (bc_len / diag_len) + b;
    this->vertices = {a, b, c, d};
}

Point Rectangle::center() const {
    return (vertices[1] + vertices[3]) * 0.5;
}

std::pair<Line, Line> Rectangle::diagonals() const {
    return {{vertices[0], vertices[2]},
            {vertices[1], vertices[3]}};
}

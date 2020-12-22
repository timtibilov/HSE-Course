#include <utility>
#include "square.h"
#include "circle.h"
#include "utils.h"


Square::Square(const Point &top_left, const Point &bottom_right) : Rectangle(top_left, bottom_right, 1) {}

Circle Square::circumscribedCircle() const {
    return {(vertices[0] + vertices[2]) * 0.5, Utils::dist(vertices[0], vertices[2]) / 2};
}

Circle Square::inscribedCircle() const {
    return {((vertices[0] + vertices[2]) * 0.5), (Utils::dist(vertices[0], vertices[1]) / 2)};
}




#include <vector>
#include <cmath>
#include "polygon.h"
#include "utils.h"
#include "const.h"


int Polygon::verticesCount() const {
    return vertices.size();
}

std::vector<Point> Polygon::getVertices() const {
    return vertices;
}

bool Polygon::isConvex() const {
    return false;
}

double Polygon::perimeter() const {
    return 0;
}

double Polygon::area() const {
    return 0;
}

bool Polygon::operator==(const Shape &) const {
    return false;
}

bool Polygon::isCongruentTo(const Shape &) const {
    return false;
}

bool Polygon::isSimilarTo(const Shape &) const {
    return false;
}

bool Polygon::containsPoint(const Point &) const {
    return false;
}

void Polygon::rotate(const Point &center, double angle) {
    double rads = angle / 180 * PI;
    for (Point& v : vertices) {
        Point diff = v - center;
        Point rotated{diff.x * cos(rads) - diff.y * sin(rads), diff.x * sin(rads) + diff.y * cos(rads)};
        v.x = rotated.x + center.x;
        v.y = rotated.y + center.y;
    }
}

void Polygon::reflex(const Point &center) {
    for (Point& v : vertices) {
        Point diff = center - v;
        v.x = diff.x * 2.;
        v.y = diff.y * 2.;
    }
}

void Polygon::reflex(const Line &axis) {
    Point vector = axis.getSecond() - axis.getFirst();
    Point norm_vector = vector * (1. / dist(vector));
    for (Point& v : vertices) {
        Point point_vector = v - axis.getFirst();
        double len = dot(vector, point_vector) / dist(vector);
        Point projection = axis.getFirst() + norm_vector * len;
        Point new_v = projection * 2. - v;
        v.x = new_v.x;
        v.y = new_v.y;
    }
}

void Polygon::scale(const Point &center, double coefficient) {
    for (Point& v : vertices) {
        Point diff = v - center;
        Point scaled_diff = diff * coefficient;
        Point new_v = center + scaled_diff;
        v.x = new_v.x;
        v.y = new_v.y;
    }
}

#ifndef GEOMETRY_HIERARCHY_RECTANGLE_H_
#define GEOMETRY_HIERARCHY_RECTANGLE_H_

#include "polygon.h"
#include <utility>


class Rectangle : public Polygon {
public:
    Rectangle(const Point &bottom, const Point &top, double coefficient);

    Point center() const;

    std::pair<Line, Line> diagonals() const;
};

#endif //GEOMETRY_HIERARCHY_RECTANGLE_H_
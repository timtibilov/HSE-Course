#ifndef GEOMETRY_HIERARCHY_ELLIPSE_H_
#define GEOMETRY_HIERARCHY_ELLIPSE_H_

#include "shape.h"
#include <utility>


class Ellipse : public Shape {
    Point focus_a;
    Point focus_b;
    double a;

    double c() const;

public:
    Ellipse(const Point &a, const Point &b, double dist_sum);

    std::pair<Point, Point> focuses() const;

    std::pair<Line, Line> directrices() const;

    double eccentricity() const;

    Point center() const;

    double perimeter() const override;

    double area() const override;

    bool operator==(const Shape &) const override;

    bool isCongruentTo(const Shape &) const override;

    bool isSimilarTo(const Shape &) const override;

    bool containsPoint(const Point &) const override;

    void rotate(const Point &, double) override;

    void reflex(const Point &) override;

    void reflex(const Line &) override;

    void scale(const Point &, double) override;
};

#endif  //GEOMETRY_HIERARCHY_ELLIPSE_H_
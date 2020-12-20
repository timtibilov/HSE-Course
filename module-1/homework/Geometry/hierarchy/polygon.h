#ifndef GEOMETRY_HIERARCHY_POLYGON_H_
#define GEOMETRY_HIERARCHY_POLYGON_H_

#include "shape.h"


class Polygon : public Shape {
public:
    Polygon(std::vector<Point> &points) : vertices(points) {};

    int verticesCount() const;

    std::vector<Point> getVertices() const;

    bool isConvex() const;

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

protected:
    std::vector<Point> vertices;
};

#endif  //GEOMETRY_HIERARCHY_POLYGON_H_
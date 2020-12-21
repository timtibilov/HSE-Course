#ifndef GEOMETRY_HIERARCHY_POINT_H_
#define GEOMETRY_HIERARCHY_POINT_H_

struct Point {
    double x;
    double y;

    Point() : x(0), y(0) {}

    Point(double x, double y) : x(x), y(y) {}

    bool operator==(const Point &) const;

    bool operator!=(const Point &) const;

    Point operator+(const Point &) const;

    Point operator-(const Point &) const;

    Point operator*(double) const;

    Point rotate(double) const;
};

#endif //GEOMETRY_HIERARCHY_POINT_H_
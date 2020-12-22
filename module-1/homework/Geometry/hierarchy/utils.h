#ifndef GEOMETRY_HIERARCHY_UTILS_H
#define GEOMETRY_HIERARCHY_UTILS_H


#include "point.h"
#include "line.h"

struct Utils {
public:
    static double dist(const Point &, const Point &);

    static double dist(const Point &);

    static double dot(const Point &, const Point &);

    static double cross(const Point &, const Point &);

    static bool eq(double, double); // equals

    static bool le(double, double); // less

    static bool gr(double, double); // greater

    static Point intersect(const Line &, const Line &);

};

#endif //GEOMETRY_HIERARCHY_UTILS_H

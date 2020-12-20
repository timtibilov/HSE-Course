#ifndef GEOMETRY_HIERARCHY_UTILS_H
#define GEOMETRY_HIERARCHY_UTILS_H

#include "point.h"


static double dist(const Point &, const Point &);

static double dist(const Point &);

static double dot(const Point &, const Point &);

static double cross(const Point &, const Point &);

static bool eq(double, double); // equals

static bool le(double, double); // less

static bool gr(double, double); // greater


#endif //GEOMETRY_HIERARCHY_UTILS_H

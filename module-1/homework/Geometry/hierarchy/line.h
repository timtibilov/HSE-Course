#ifndef GEOMETRY_HIERARCHY_LINE_H_
#define GEOMETRY_HIERARCHY_LINE_H_

#include "point.h"

class Line {
    struct Formula {
        double x;
        double y;
        double c;
    };

    Point a;
    Point b;

public:
    Line() : a(0, 0), b(0, 0) {}

    Line(const Point &f, const Point &s) : a(f), b(s) {}

    Point getFirst() const;

    Point getSecond() const;

    Formula countFormula() const;

    bool operator==(const Line &) const;

    bool operator!=(const Line &) const;
};

#endif //GEOMETRY_HIERARCHY_CONSTANTS_H_
#ifndef GEOMETRY_HIERARCHY_CIRCLE_H_
#define GEOMETRY_HIERARCHY_CIRCLE_H_

#include "ellipse.h"

class Circle : public Ellipse {
    double r;
public:
    Circle(const Point &, double);

    double radius() const;
};


#endif //GEOMETRY_HIERARCHY_CIRCLE_H_
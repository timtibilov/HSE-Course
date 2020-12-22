#ifndef GEOMETRY_HIERARCHY_SQUARE_H_
#define GEOMETRY_HIERARCHY_SQUARE_H_

#include "rectangle.h"
#include "circle.h"


class Square : public Rectangle {
public:
    Square(const Point &top_left, const Point &bottom_right);

    Circle circumscribedCircle() const;

    Circle inscribedCircle() const;
};

#endif //GEOMETRY_HIERARCHY_SQUARE_H_
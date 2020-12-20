#include <utility>
#include <cmath>
#include <vector>
#include "point.h"
#include "ellipse.h"
#include "utils.h"
#include "const.h"


Ellipse::Ellipse(const Point &a, const Point &b, double dist_sum) : focus_a(a), focus_b(b), dist_(dist_sum / 2.) {}

double Ellipse::c() const {
    return dist(focus_a, focus_b) / 2.;
}

std::pair<Point, Point> Ellipse::focuses() const {
    return {focus_a, focus_b};
}

std::pair<Line, Line> Ellipse::directrices() const {
    double b = (dist_ * dist_ - c() * c());
    double c;
    if (b > dist_)
        c = b / eccentricity();
    else
        c = dist_ / eccentricity();
    Line f = Line({-c, 0}, {-c, 1});
    Line s = Line({c, 0}, {c, 1});
    return {f, s};
}

double Ellipse::eccentricity() const {
    return c() / dist_;
}

Point Ellipse::center() const {
    return (focus_a + focus_b) * 0.5;
}

double Ellipse::perimeter() const {
    double b = (dist_ * dist_ - c() * c());
    return 4 * (PI * dist_ * b + (dist_ - b)) / (dist_ + b); // С точностью до 0.63%
}

double Ellipse::area() const {
    double b = (dist_ * dist_ - c() * c());
    return dist_ * b * PI;
}

bool Ellipse::operator==(const Shape &another) const {
    const auto *shape = dynamic_cast<const Ellipse *>(&another);
    if (shape != nullptr)
        return eq(dist_, shape->dist_) &&
               ((focus_a == shape->focus_a && focus_b == shape->focus_b) ||
                (focus_a == shape->focus_b && focus_b == shape->focus_a));
    else
        return false;
}

bool Ellipse::isCongruentTo(const Shape &another) const {
    const auto *shape = dynamic_cast<const Ellipse *>(&another);
    if (shape != nullptr)
        return eq(dist(focus_a, focus_b), dist(shape->focus_a, shape->focus_b)) && eq(dist_, shape->dist_);
    else
        return false;
}

bool Ellipse::isSimilarTo(const Shape &another) const {
    const auto *shape = dynamic_cast<const Ellipse *>(&another);
    if (shape != nullptr) {
        double coef = dist(focus_a, focus_b) / dist(shape->focus_a, shape->focus_b);
        return eq(dist_, shape->dist_ * coef);
    } else
        return false;
}

bool Ellipse::containsPoint(const Point &point) const {
    return dist(focus_a, point) + dist(focus_b, point) <= dist_ * 2;
}

void Ellipse::rotate(const Point &center, double angle) {
    std::vector<Point *> focuses{&focus_a, &focus_b};
    double rads = angle / 180 * PI;
    for (Point *focus : focuses) {
        Point diff = *focus - center;
        Point rotated{diff.x * cos(rads) - diff.y * sin(rads), diff.x * sin(rads) + diff.y * cos(rads)};
        Point new_focus = rotated + center;
        focus->x = new_focus.x;
        focus->y = new_focus.y;
    }
}

void Ellipse::reflex(const Point &center) {
    std::vector<Point *> focuses{&focus_a, &focus_b};
    for (Point *focus: focuses) {
        Point diff = center - *focus;
        focus->x += diff.x * 2;
        focus->y += diff.y * 2;
    }
}

void Ellipse::reflex(const Line &axis) {
    Point vector = axis.getSecond() - axis.getFirst();
    Point norm_vector = vector * (1. / dist(vector));
    std::vector<Point *> focuses{&focus_a, &focus_b};
    for (Point *focus : focuses) {
        Point point_vector = *focus - axis.getFirst();
        double len = dot(vector, point_vector) / dist(vector);
        Point projection = axis.getFirst() + norm_vector * len;
        Point new_focus = projection * 2. - *focus;
        focus->x = new_focus.x;
        focus->y = new_focus.y;
    }
}

void Ellipse::scale(const Point &center, double coefficient) {
    std::vector<Point *> focuses{&focus_a, &focus_b};
    for (Point *focus : focuses) {
        Point diff = *focus - center;
        Point scaled_diff = diff * coefficient;
        Point new_focus = center + scaled_diff;
        focus->x = new_focus.x;
        focus->y = new_focus.y;
    }
}

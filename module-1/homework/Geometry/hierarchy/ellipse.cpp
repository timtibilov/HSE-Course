#include <utility>
#include <cmath>
#include <vector>
#include "point.h"
#include "ellipse.h"
#include "utils.h"
#include "const.h"


Ellipse::Ellipse(const Point &a, const Point &b, double dist_sum) : focus_a(a), focus_b(b), a(dist_sum / 2.) {}

double Ellipse::c() const {
    return Utils::dist(focus_a, focus_b) / 2.;
}

std::pair<Point, Point> Ellipse::focuses() const {
    return {focus_a, focus_b};
}

std::pair<Line, Line> Ellipse::directrices() const {
    double b = (a * a - c() * c());
    double c;
    if (b > a)
        c = b / eccentricity();
    else
        c = a / eccentricity();
    Line f = Line({-c, 0}, {-c, 1});
    Line s = Line({c, 0}, {c, 1});
    return {f, s};
}

double Ellipse::eccentricity() const {
    return c() / a;
}

Point Ellipse::center() const {
    return (focus_a + focus_b) * 0.5;
}

double Ellipse::perimeter() const {
    double b = sqrt(a * a - c() * c());
    return 4 * (PI * a * b + (a - b)) / (a + b); // С точностью до 0.63%
}

double Ellipse::area() const {
    double b = sqrt(a * a - c() * c());
    return a * b * PI;
}

bool Ellipse::operator==(const Shape &another) const {
    const auto *ellipse = dynamic_cast<const Ellipse *>(&another);
    if (ellipse != nullptr)
        return Utils::eq(a, ellipse->a) &&
               ((focus_a == ellipse->focus_a && focus_b == ellipse->focus_b) ||
                (focus_a == ellipse->focus_b && focus_b == ellipse->focus_a));
    else
        return false;
}

bool Ellipse::isCongruentTo(const Shape &another) const {
    const auto *ellipse = dynamic_cast<const Ellipse *>(&another);
    if (ellipse != nullptr)
        return Utils::eq(Utils::dist(focus_a, focus_b), Utils::dist(ellipse->focus_a, ellipse->focus_b)) &&
               Utils::eq(a, ellipse->a);
    else
        return false;
}

bool Ellipse::isSimilarTo(const Shape &another) const {
    const auto *ellipse = dynamic_cast<const Ellipse *>(&another);
    if (ellipse != nullptr) {
        double coef = Utils::dist(focus_a, focus_b) / Utils::dist(ellipse->focus_a, ellipse->focus_b);
        return Utils::eq(a, ellipse->a * coef);
    } else
        return false;
}

bool Ellipse::containsPoint(const Point &point) const {
    return Utils::dist(focus_a, point) + Utils::dist(focus_b, point) <= a * 2;
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
    Point norm_vector = vector * (1. / Utils::dist(vector));
    std::vector<Point *> focuses{&focus_a, &focus_b};
    for (Point *focus : focuses) {
        Point point_vector = *focus - axis.getFirst();
        double len = Utils::dot(vector, point_vector) / Utils::dist(vector);
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

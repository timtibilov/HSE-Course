#include "utils.h"
#include "const.h"
#include <cmath>


double Utils::dist(const Point &a, const Point &b) {
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

double Utils::dist(const Point &a) {
    return Utils::dist(a, Point());
}

double Utils::dot(const Point &a, const Point &b) {
    return a.x * b.x + a.y * b.y;
}

double Utils::cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

bool Utils::eq(double a, double b) {
    return std::abs(a - b) < EPS;
}

bool Utils::le(double a, double b) {
    return b - a - EPS > 0;
}

bool Utils::gr(double a, double b) {
    return a - b - EPS > 0;
}

Point Utils::intersect(const Line &f, const Line &s) {
    const auto &first_formula = f.countFormula();
    const auto &second_formula = s.countFormula();
    double x_nom = -(first_formula.c * second_formula.y - second_formula.c * first_formula.y);
    double denom = (first_formula.x * second_formula.y - second_formula.x * first_formula.y);
    double y_nom = -(first_formula.x * second_formula.c - second_formula.x * first_formula.c);
    return {x_nom / denom, y_nom / denom};
}

#include <vector>
#include <cmath>
#include "triangle.h"
#include "utils.h"


Triangle::Triangle(const Point &a, const Point &b, const Point &c) : Polygon() {
    this->vertices = {a, b, c};
}

Circle Triangle::circumscribedCircle() const {
    Point first_vector = vertices[0] - vertices[1];
    Point second_vector = vertices[2] - vertices[1];
    Point first_perpend_vec{-first_vector.y, first_vector.x};
    Point second_perpend_vec{-second_vector.y, second_vector.x};
    Point first_center = (vertices[0] + vertices[1]) * 0.5;
    Point second_center = (vertices[1] + vertices[2]) * 0.5;
    Line first_perpendicular{first_perpend_vec + first_center, first_center};
    Line second_perpendicular{second_perpend_vec + second_center, second_center};
    Point intersection = Utils::intersect(first_perpendicular, second_perpendicular);
    double radius = Utils::dist(intersection, vertices[0]);
    return {intersection, radius};
}

Circle Triangle::inscribedCircle() const {
    Point a = vertices[1] - vertices[0];
    Point b = vertices[2] - vertices[0];
    Point c = vertices[2] - vertices[1];
    double a_angle = acos(Utils::dot(a, b) / Utils::dist(a) / Utils::dist(b)) / 2;
    Point first_vector{a.x * cos(a_angle) - a.y * sin(a_angle),
                       a.x * sin(a_angle) + a.y * cos(a_angle)};
    Line first_line{vertices[0], vertices[0] + first_vector};
    double b_angle = acos(Utils::dot(b, c) / Utils::dist(b) / Utils::dist(c)) / 2;
    Point second_vector{b.x * cos(b_angle) - b.y * sin(b_angle),
                        b.x * sin(b_angle) + b.y * cos(b_angle)};
    Line second_line{vertices[2], vertices[2] + second_vector};
    Point intersection = Utils::intersect(first_line, second_line);
    Point intersection_vector = intersection - vertices[0];
    double projection_len = Utils::dot(a, intersection_vector) / Utils::dist(a);
    double height_len = sqrt(
            Utils::dist(intersection_vector) * Utils::dist(intersection_vector) - projection_len * projection_len);
    return {intersection, height_len};
}

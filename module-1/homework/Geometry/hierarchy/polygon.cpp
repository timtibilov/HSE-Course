#include <vector>
#include <cmath>
#include <algorithm>
#include "polygon.h"
#include "utils.h"
#include "const.h"


Polygon::Polygon(std::vector<Point> &points) : vertices(points) {}

int Polygon::verticesCount() const {
    return vertices.size();
}

std::vector<Point> Polygon::getVertices() const {
    return vertices;
}

bool Polygon::isConvex() const {
    for (int i = 0; i < verticesCount(); i++) {
        Point first_vec = vertices[(i + 1) % verticesCount()] - vertices[i];
        Point second_vec = vertices[(i + 2) % verticesCount()] - vertices[i];
        if (Utils::cross(first_vec, second_vec) < 0)
            return false;
    }
    return true;
}

double Polygon::perimeter() const {
    double perimeter = 0;
    for (int i = 0; i < verticesCount(); ++i)
        perimeter += Utils::dist(vertices[i], vertices[(i + 1) % verticesCount()]);
    return perimeter;
}

double Polygon::area() const {
    Point inside = (vertices[0] + vertices[1]) * 0.5;
    double area = 0;
    for (int i = 0; i < verticesCount(); ++i) {
        const Point &current = vertices[i];
        const Point &next = vertices[(i + 1) % verticesCount()];
        area += std::abs(Utils::cross(current - inside, next - inside));
    }
    return area / 2;
}

bool Polygon::operator==(const Shape &another) const {
    const auto *polygon = dynamic_cast<const Polygon *>(&another);
    if (polygon == nullptr || verticesCount() != polygon->verticesCount())
        return false;
    int start = std::find(vertices.begin(), vertices.end(), polygon->vertices[0]) - vertices.begin();
    if (start >= verticesCount())
        return false;
    for (int i = 0; i < verticesCount(); i++) {
        if (vertices[(start + i + verticesCount()) % verticesCount()] != polygon->vertices[i])
            return false;
    }
    return true;
}

bool Polygon::isCongruentTo(const Shape &another) const {
    const auto *polygon = dynamic_cast<const Polygon *>(&another);
    if (polygon == nullptr || verticesCount() != polygon->verticesCount())
        return false;
    std::vector<Point> vectors;
    std::vector<Point> polygon_vectors;
    for (int i = 0; i < verticesCount(); i++) {
        vectors.push_back(vertices[(i + 1) % verticesCount()] - vertices[i]);
        polygon_vectors.push_back(polygon->vertices[(i + 1) % verticesCount()] - polygon->vertices[i]);
    }
    for (int i = 0; i < verticesCount(); i++) {
        bool equals = true;
        for (int j = 0; j < verticesCount(); j++) {
            if (vectors[(i + j + verticesCount()) % verticesCount()] != polygon_vectors[j]) {
                equals = false;
                break;
            }
        }
        if (equals)
            return true;
    }
    return false;
}

bool Polygon::isSimilarTo(const Shape &another) const {
    const auto *polygon = dynamic_cast<const Polygon *>(&another);
    if (polygon == nullptr || verticesCount() != polygon->verticesCount())
        return false;
    std::vector<Point> vectors;
    std::vector<Point> polygon_vectors;
    for (int i = 0; i < verticesCount(); i++) {
        vectors.push_back(vertices[(i + 1) % verticesCount()] - vertices[i]);
        polygon_vectors.push_back(polygon->vertices[(i + 1) % verticesCount()] - polygon->vertices[i]);
    }
    for (int i = 0; i < verticesCount(); i++) {
        bool equals = true;
        double coefficient = 0;
        for (int j = 0; j < verticesCount(); j++) {
            double vec_len = Utils::dist(vectors[(i + j + verticesCount()) % verticesCount()]);
            double polygon_vec_len = Utils::dist(polygon_vectors[j]);
            if (coefficient == 0)
                coefficient = vec_len / polygon_vec_len;

            if (!Utils::eq(vec_len, polygon_vec_len * coefficient)) {
                equals = false;
                break;
            }
        }
        if (equals)
            return true;
    }
    return false;
}

bool Polygon::containsPoint(const Point &point) const {
    double angle_sum = 0;
    for (int i = 0; i < verticesCount(); ++i) {
        Point current = vertices[i] - point;
        Point next = vertices[(i + 1) % verticesCount()] - point;
        double cross_ = Utils::cross(current, next);
        double dot_ = Utils::dot(current, next);
        if (std::abs(cross_) < EPS && dot_ <= 0)
            return true;
        else
            angle_sum += atan2(cross_, dot_);
    }
    return Utils::eq(angle_sum, 2 * PI);
}

void Polygon::rotate(const Point &center, double angle) {
    double rads = angle / 180 * PI;
    for (Point &v : vertices) {
        Point diff = v - center;
        Point rotated{diff.x * cos(rads) - diff.y * sin(rads), diff.x * sin(rads) + diff.y * cos(rads)};
        v.x = rotated.x + center.x;
        v.y = rotated.y + center.y;
    }
}

void Polygon::reflex(const Point &center) {
    for (Point &v : vertices) {
        Point diff = center - v;
        v.x = diff.x * 2.;
        v.y = diff.y * 2.;
    }
}

void Polygon::reflex(const Line &axis) {
    Point vector = axis.getSecond() - axis.getFirst();
    Point norm_vector = vector * (1. / Utils::dist(vector));
    for (Point &v : vertices) {
        Point point_vector = v - axis.getFirst();
        double len = Utils::dot(vector, point_vector) / Utils::dist(vector);
        Point projection = axis.getFirst() + norm_vector * len;
        Point new_v = projection * 2. - v;
        v.x = new_v.x;
        v.y = new_v.y;
    }
}

void Polygon::scale(const Point &center, double coefficient) {
    for (Point &v : vertices) {
        Point diff = v - center;
        Point scaled_diff = diff * coefficient;
        Point new_v = center + scaled_diff;
        v.x = new_v.x;
        v.y = new_v.y;
    }
}

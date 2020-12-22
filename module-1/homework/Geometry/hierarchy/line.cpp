#include <cmath>
#include "line.h"
#include "const.h"

Point Line::getFirst() const {
    return a;
}

Point Line::getSecond() const {
    return b;
}

Line::Formula Line::countFormula() const {
    double x = a.y - b.y;
    double y = b.x - a.x;
    double c = a.x * b.y - b.x * a.y;

    return {x, y, c};
}

bool Line::operator==(const Line &line) const {
    Formula thisFormula = countFormula();
    Formula lineFormula = line.countFormula();
    if (std::abs(thisFormula.x) < EPS) {
        if (std::abs(lineFormula.y) < EPS)
            return false;
    }

    if (std::abs(thisFormula.y) < EPS) {
        if (std::abs(lineFormula.x) < EPS)
            return false;
    }
    double factorX = lineFormula.x / thisFormula.x;
    double factorY = lineFormula.y / thisFormula.y;
    double factorC = lineFormula.c / thisFormula.c;

    return std::abs(factorX - factorY) < EPS &&
           std::abs(factorX - factorC) < EPS;
}

bool Line::operator!=(const Line &line) const {
    return !(*this == line);
}

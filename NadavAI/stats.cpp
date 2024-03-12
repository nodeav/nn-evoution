#include <iostream>
#include "stats.h"

void Stats::addPoint(StatsPoint point) {
    points.push_back(point);
}

void Stats::printLast(size_t n) {
    for (auto it = points.end() - n; it != points.end(); ++it) {
        auto point = *it;
        std::cout << "Frame: " << point.frame << ", Torefs: " << point.torefs << ", Tarifs: " << point.tarifs << std::endl;
    }
}


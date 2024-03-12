#pragma once

struct StatsPoint {
    uint32_t torefs;
    uint32_t tarifs;
    uint32_t frame;
};

class Stats {
    std::vector<StatsPoint> points;
public:
    void addPoint(StatsPoint point);
    void printLast(size_t n);
};
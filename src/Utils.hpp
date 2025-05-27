#pragma once

#include <stdint.h>

#include <string>
#include <unordered_map>
#include <vector>

struct Point {
    size_t group;
    size_t x;
    size_t y;

    Point(size_t _group, size_t _x, size_t _y) : group(_group), x(_x), y(_y) {};
};

using FileData = std::vector<Point>;
using ParsedData = std::unordered_map<std::string, FileData>;

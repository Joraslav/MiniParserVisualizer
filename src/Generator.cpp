#include "Utils.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std::literals;

void PrintTxt(std::ofstream& out, const Point& point) {
    out << "t" << point.group << ":" << point.x << "," << point.y;
}

struct BinaryPointData {
    uint32_t group : 8;
    uint32_t x : 12;
    uint32_t y : 12;
};

void WriteBinaryPointData(std::ofstream& out, const BinaryPointData& point) {
    uint32_t packed = 0;
    packed |= (point.group & 0xFF) << 24;
    packed |= (point.x & 0xFFF) << 12;
    packed |= (point.y & 0xFFF);

    out.write(reinterpret_cast<const char*>(&packed), sizeof(packed));
}

Point GeneratePointTxt(size_t group, size_t index) {
    size_t x = index;
    size_t y = index * 3;
    return Point(group, x, y);
}

BinaryPointData GeneratePointBinary(size_t group, size_t index) {
    BinaryPointData point;
    point.group = static_cast<uint32_t>(group);
    point.x = static_cast<uint32_t>(index);
    point.y = static_cast<uint32_t>(index * 2);
    return point;
}

void MakeTxt(const std::string& filename) {
    std::ofstream fout(filename);
    if (!fout) {
        throw std::runtime_error("Failed to generate .txt file"s);
    }

    for (size_t i = 1; i != 4; ++i) {
        Point point = GeneratePointTxt(i, i);
        PrintTxt(fout, point);
        fout << '\n';
    }

    fout.close();
}

void MakeBinary(const std::string& filename) {
    std::ofstream fout(filename, std::ios::binary);
    if (!fout) {
        throw std::runtime_error("Failed to generate .bin file"s);
    }

    for (size_t i = 1; i != 4; ++i) {
        BinaryPointData point = GeneratePointBinary(i, i);
        WriteBinaryPointData(fout, point);
    }

    fout.close();
}

int main() {
    std::cout << "Begin Generate data for Project"s << std::endl;
    try {
        MakeTxt("../data/filetxt.txt"s);
        MakeBinary("../data/filebin.bin"s);
    } catch (const std::exception& e) {
        std::cerr << "Error: "s << e.what() << std::endl;
        return 1;
    }
    std::cout << "End Generate data for Project"s << std::endl;
    return 0;
}
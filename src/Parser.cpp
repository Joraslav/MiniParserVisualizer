#include "Parser.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

ParsedData ParseTxt(const std::string &filename) {
    ParsedData parse_data;
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) {
            break;
        }

        size_t pos = line.find(':');
        if (pos == std::string::npos) {
            throw std::runtime_error("Invalid group format in file: " + filename);
        }

        std::string group_str = line.substr(
            1, pos);  // Поскольку строка выглядит как "t1:1,2". Необходимо отделить символ t
        size_t group = std::stoull(group_str);
        std::string coords = line.substr(pos + 1);

        size_t comma = coords.find(',');
        if (comma == std::string::npos) {
            throw std::runtime_error("Invalid coordinates format in file: " + filename);
        }

        size_t x = std::stoul(coords.substr(0, comma));
        size_t y = std::stoul(coords.substr(comma + 1));

        parse_data[filename].emplace_back(group, x, y);
    }

    fin.close();
    return parse_data;
}

ParsedData ParseBin(const std::string &filename) {
    ParsedData parse_data;
    std::ifstream fin(filename, std::ios::binary);
    if (!fin.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    while (true) {
        uint8_t group;
        uint32_t x, y;
        if (fin.eof()) {
            break;
        }
        fin.read(reinterpret_cast<char *>(&group), sizeof(group));
        fin.read(reinterpret_cast<char *>(&x), sizeof(x));
        fin.read(reinterpret_cast<char *>(&y), sizeof(y));

        parse_data[filename].emplace_back(static_cast<size_t>(group), static_cast<size_t>(x),
                                          static_cast<size_t>(y));
    }

    fin.close();
    return parse_data;
}

ParsedData ParseJSON(const std::string &filename) {
    ParsedData parse_data;
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    json json_data = json::parse(fin);
    if (!json_data.contains("points") || !json_data["points"].is_array()) {
        throw std::runtime_error("JSON does not contain 'points' array: " + filename);
    }

    for (const auto &point : json_data["points"]) {
        if (!point.contains("group") || !point.contains("x") || !point.contains("y")) {
            throw std::runtime_error("Invalid point format in file: " + filename);
        }
        uint32_t group = point["group"].get<uint32_t>();
        uint32_t x = point["x"].get<uint32_t>();
        uint32_t y = point["y"].get<uint32_t>();
        parse_data[filename].emplace_back(group, x, y);
    }

    fin.close();
    return parse_data;
}

ParsedData Parse(const std::string &filename) {
    static const std::map<std::string, ParserFunction> parsers = {
        {".txt", ParseTxt}, {".bin", ParseBin}, {".json", ParseJSON}};

    size_t dot_pos = filename.rfind('.');
    if (dot_pos == std::string::npos) {
        throw std::runtime_error("Unknown file extension: " + filename);
    }
    std::string ext = filename.substr(dot_pos);

    auto it = parsers.find(ext);
    if (it == parsers.end()) {
        throw std::runtime_error("No parser for extension: " + ext);
    }

    return it->second(filename);
}

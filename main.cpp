#include "Parser.hpp"

#include <nlohmann/json.hpp>

#include <iostream>
#include <vector>
#include <string>

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./parser <file1> <file2> ..." << std::endl;
        return 1;
    }

    json output;
    output["files"] = json::array();

    try {
        for (int i = 1; i < argc; ++i) {
            std::string filename = argv[i];
            ParsedData data = Parse(filename);

            for (const auto& [fname, points] : data) {
                json file_entry;
                file_entry["filename"] = fname;
                file_entry["points"] = json::array();

                for (const auto& [group, x, y] : points) {
                    file_entry["points"].push_back({{"group", group}, {"x", x}, {"y", y}});
                }

                output["files"].push_back(file_entry);
            }
        }

        std::cout << output.dump() << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}

#include "Parser.hpp"

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <cstdio>
#include <fstream>

// Helper to create and remove test files
class FileGuard {
 public:
    explicit FileGuard(const std::string& filename) : filename_(filename) {}
    ~FileGuard() { std::remove(filename_.c_str()); }
    const std::string& name() const { return filename_; }

 private:
    std::string filename_;
};

TEST(ParserTests, ParseTxt_ValidFile) {
    std::string filename = "test_points.txt";
    {
        std::ofstream fout(filename);
        fout << "t1:10,20\n";
        fout << "2:30,40\n";
    }
    FileGuard guard(filename);

    ParsedData data = ParseTxt(filename);
    ASSERT_EQ(data.size(), 1);
    ASSERT_EQ(data[filename].size(), 2);
    EXPECT_EQ(data[filename][0].group, 1);
    EXPECT_EQ(data[filename][0].x, 10);
    EXPECT_EQ(data[filename][0].y, 20);
    EXPECT_EQ(data[filename][1].group, 2);
    EXPECT_EQ(data[filename][1].x, 30);
    EXPECT_EQ(data[filename][1].y, 40);
}

TEST(ParserTests, ParseTxt_InvalidFormat) {
    std::string filename = "bad_points.txt";
    {
        std::ofstream fout(filename);
        fout << "bad_line\n";
    }
    FileGuard guard(filename);

    EXPECT_THROW(ParseTxt(filename), std::runtime_error);
}

TEST(ParserTests, ParseBin_ValidFile) {
    std::string filename = "test_points.bin";
    {
        std::ofstream fout(filename, std::ios::binary);
        uint32_t packed1 = (1 << 24) | (10 << 12) | 20;
        uint32_t packed2 = (2 << 24) | (30 << 12) | 40;
        fout.write(reinterpret_cast<const char*>(&packed1), sizeof(packed1));
        fout.write(reinterpret_cast<const char*>(&packed2), sizeof(packed2));
    }
    FileGuard guard(filename);

    ParsedData data = ParseBin(filename);
    ASSERT_EQ(data.size(), 1);
    ASSERT_EQ(data[filename].size(), 2);
    EXPECT_EQ(data[filename][0].group, 1);
    EXPECT_EQ(data[filename][0].x, 10);
    EXPECT_EQ(data[filename][0].y, 20);
    EXPECT_EQ(data[filename][1].group, 2);
    EXPECT_EQ(data[filename][1].x, 30);
    EXPECT_EQ(data[filename][1].y, 40);
}

TEST(ParserTests, ParseBin_EmptyFile) {
    std::string filename = "empty.bin";
    {
        std::ofstream fout(filename, std::ios::binary);
    }
    FileGuard guard(filename);

    ParsedData data = ParseBin(filename);
    ASSERT_TRUE(data[filename].empty());
}

TEST(ParserTests, ParseJSON_ValidFile) {
    std::string filename = "test_points.json";
    {
        nlohmann::json j;
        j["points"] = {{{"group", "1"}, {"x", 10}, {"y", 20}},
                       {{"group", "2"}, {"x", 30}, {"y", 40}}};
        std::ofstream fout(filename);
        fout << j.dump();
    }
    FileGuard guard(filename);

    ParsedData data = ParseJSON(filename);
    ASSERT_EQ(data.size(), 1);
    ASSERT_EQ(data[filename].size(), 2);
    EXPECT_EQ(data[filename][0].group, 1);
    EXPECT_EQ(data[filename][0].x, 10);
    EXPECT_EQ(data[filename][0].y, 20);
    EXPECT_EQ(data[filename][1].group, 2);
    EXPECT_EQ(data[filename][1].x, 30);
    EXPECT_EQ(data[filename][1].y, 40);
}

TEST(ParserTests, ParseJSON_MissingPoints) {
    std::string filename = "bad.json";
    {
        nlohmann::json j;
        j["not_points"] = {};
        std::ofstream fout(filename);
        fout << j.dump();
    }
    FileGuard guard(filename);

    EXPECT_THROW(ParseJSON(filename), std::runtime_error);
}

TEST(ParserTests, Parse_DispatchTxt) {
    std::string filename = "dispatch.txt";
    {
        std::ofstream fout(filename);
        fout << "1:2,3\n";
    }
    FileGuard guard(filename);

    ParsedData data = Parse(filename);
    ASSERT_EQ(data.size(), 1);
    ASSERT_EQ(data[filename].size(), 1);
    EXPECT_EQ(data[filename][0].group, 1);
    EXPECT_EQ(data[filename][0].x, 2);
    EXPECT_EQ(data[filename][0].y, 3);
}

TEST(ParserTests, Parse_UnknownExtension) {
    std::string filename = "file.unknown";
    {
        std::ofstream fout(filename);
        fout << "data";
    }
    FileGuard guard(filename);

    EXPECT_THROW(Parse(filename), std::runtime_error);
}

TEST(ParserTests, Parse_FileNotFound) {
    std::string filename = "no_such_file.txt";
    EXPECT_THROW(Parse(filename), std::runtime_error);
}

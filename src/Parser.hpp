#pragma once

#include "Utils.hpp"

#include <functional>

using ParserFunction = std::function<ParsedData(const std::string &)>;

ParsedData ParseTxt(const std::string &filename);

ParsedData ParseBin(const std::string &filename);

ParsedData ParseJSON(const std::string &filename);

ParsedData Parse(const std::string &filename);

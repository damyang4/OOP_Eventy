#pragma once

#include <vector>
#include <string>

class StringTokenizer {
public:
    static std::vector<std::string> tokenize(const std::string& commandLine);
};
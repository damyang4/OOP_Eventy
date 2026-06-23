#pragma once

#include <vector>
#include <string>

class PersistenceEngine {
public:
    static bool writeRow(const std::string& filename, const std::vector<std::string>& columns);
    static void clearFile(const std::string& filename);
    static std::vector<std::vector<std::string>> readRows(const std::string& filename);
};
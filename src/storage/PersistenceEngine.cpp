#include "PersistenceEngine.h"
#include <fstream>

bool PersistenceEngine::writeRow(const std::string& filename, const std::vector<std::string>& columns) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return false;

    for (size_t i = 0; i < columns.size(); ++i) {
        file << columns[i];
        if (i < columns.size() - 1) {
            file << "|"; // Dynamic internal CSV delimiter mapping
        }
    }
    file << "\n";
    file.close();
    return true;
}

void PersistenceEngine::clearFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc);
    file.close();
}

std::vector<std::vector<std::string>> PersistenceEngine::readRows(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    if (!file.is_open()) return data;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::string> row;
        std::string currentColumn;
        for (char c : line) {
            if (c == '|') {
                row.push_back(currentColumn);
                currentColumn.clear();
            } else {
                currentColumn.push_back(c);
            }
        }
        row.push_back(currentColumn);
        data.push_back(row);
    }
    file.close();
    return data;
}
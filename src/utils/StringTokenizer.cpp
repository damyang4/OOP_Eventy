#include "StringTokenizer.h"

std::vector<std::string> StringTokenizer::tokenize(const std::string& commandLine) {
        std::vector<std::string> tokens;
        std::string currentToken;
        bool inQuotes = false;

        for (size_t i = 0; i < commandLine.length(); ++i) {
            char c = commandLine[i];

            if (c == '"') {
                inQuotes = !inQuotes; // Toggle quote state, skip appending quote character
                continue;
            }

            if (c == ' ' && !inQuotes) {
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
            } else {
                currentToken.push_back(c);
            }
        }

        if (!currentToken.empty()) {
            tokens.push_back(currentToken);
        }

        return tokens;
    }
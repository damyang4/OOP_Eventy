#include "UserManager.h"
#include "./NotificationManager.h"
#include "../storage/PersistenceEngine.h"
#include "../core/Config.h"
#include "../domain/Client.h"
#include "../domain/Admin.h"
#include <sstream>

std::string UserManager::generateNextId() {
    auto rows = PersistenceEngine::readRows(Config::USERS_FILE);
    int maxId = 0;
    for (const auto& row : rows) {
        if (row.size() >= 2 && row[0] == "CLIENT") {
            std::string rawId = row[1]; // formats: CLIENT_1, CLIENT_2...
            size_t underscorePos = rawId.find('_');
            if (underscorePos != std::string::npos) {
                try {
                    int numericId = std::stoi(rawId.substr(underscorePos + 1));
                    if (numericId > maxId) maxId = numericId;
                } catch (...) {}
            }
        }
    }
    return "CLIENT_" + std::to_string(maxId + 1);
}

bool UserManager::usernameExists(const std::string& username) {
    if (username == Config::DEFAULT_ADMIN_USER) return true;
    
    auto rows = PersistenceEngine::readRows(Config::USERS_FILE);
    for (const auto& row : rows) {
        if (row.size() >= 3 && row[2] == username) {
            return true;
        }
    }
    return false;
}

bool UserManager::registerClient(const std::string& username, const std::string& password) {
    if (username.empty() || password.empty() || usernameExists(username)) {
        return false;
    }

    std::string newId = generateNextId();
    Client templateClient(newId, username, password, 0.0);
    
    std::vector<std::string> row = { "CLIENT", newId, username, password, "0.0" };
    bool success = PersistenceEngine::writeRow(Config::USERS_FILE, row);
    
    if (success) {
        NotificationManager::sendNotification(newId, "Welcome to Eventy! Your profile has been created successfully.");
    }
    return success;
}

User* UserManager::authenticate(const std::string& username, const std::string& password) {
    // Check default hardcoded admin account credentials
    if (username == Config::DEFAULT_ADMIN_USER && password == Config::DEFAULT_ADMIN_PASS) {
        return new Admin();
    }

    auto rows = PersistenceEngine::readRows(Config::USERS_FILE);
    for (const auto& row : rows) {
        if (row.size() >= 5 && row[2] == username && row[3] == password) {
            if (row[0] == "CLIENT") {
                try {
                    double balance = std::stod(row[4]);
                    return new Client(row[1], row[2], row[3], balance);
                } catch (...) {}
            }
        }
    }
    return nullptr; // Auth failure
}

bool UserManager::updateUserBalance(const std::string& userId, double balanceAdjustment, bool isDeduction) {
    auto rows = PersistenceEngine::readRows(Config::USERS_FILE);
    bool foundAndUpdated = false;
    
    for (auto& row : rows) {
        // Index Match: row[0] is Type, row[1] is ID, row[4] is Balance
        if (row.size() >= 5 && row[1] == userId) {
            try {
                double currentBalance = std::stod(row[4]);
                if (isDeduction) {
                    if (currentBalance < balanceAdjustment) {
                        return false; // Insufficient funds, stop early without destroying file
                    }
                    currentBalance -= balanceAdjustment;
                } else {
                    currentBalance += balanceAdjustment;
                }
                
                row[4] = std::to_string(currentBalance);
                foundAndUpdated = true;
            } catch (...) {
                return false; // Malformed row entry, safely abort transaction
            }
        }
    }
    
    // Only flush data to the physical storage layer if a match was successfully updated
    if (foundAndUpdated) {
        PersistenceEngine::clearFile(Config::USERS_FILE);
        for (const auto& row : rows) {
            PersistenceEngine::writeRow(Config::USERS_FILE, row);
        }
    }
    
    return foundAndUpdated;
}

double UserManager::getUserBalance(const std::string& userId) {
    // Read all rows to get the freshest live data state
    auto rows = PersistenceEngine::readRows(Config::USERS_FILE);
    
    for (const auto& row : rows) {
        // Index Match: row[0] is Type, row[1] is ID, row[4] is Balance
        if (row.size() >= 5 && row[1] == userId) {
            try {
                return std::stod(row[4]);
            } catch (...) {
                return 0.0; // Fail-safe default if data is unparsable
            }
        }
    }
    return 0.0; 
}


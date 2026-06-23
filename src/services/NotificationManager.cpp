#include "NotificationManager.h"
#include "../storage/PersistenceEngine.h"
#include "../core/Config.h"
#include "../utils/DateTime.h"

void NotificationManager::sendNotification(const std::string& userId, const std::string& message) {
    DateTime now;
    std::vector<std::string> row = { userId, message, now.toString(), "0" };
    PersistenceEngine::writeRow(Config::NOTIFICATIONS_FILE, row);
}

std::vector<Notification> NotificationManager::getUserNotifications(const std::string& userId) {
    std::vector<Notification> list;
    auto rows = PersistenceEngine::readRows(Config::NOTIFICATIONS_FILE);
    
    for (const auto& row : rows) {
        if (row.size() >= 4 && row[0] == userId) {
            bool isRead = (row[3] == "1");
            list.push_back(Notification(row[0], row[1], row[2], isRead));
        }
    }
    return list;
}

void NotificationManager::markAllAsRead(const std::string& userId) {
    auto rows = PersistenceEngine::readRows(Config::NOTIFICATIONS_FILE);
    PersistenceEngine::clearFile(Config::NOTIFICATIONS_FILE);
    
    for (auto& row : rows) {
        if (row.size() >= 4 && row[0] == userId) {
            row[3] = "1"; // Mark as read
        }
        if (row.size() >= 4) {
            PersistenceEngine::writeRow(Config::NOTIFICATIONS_FILE, row);
        }
    }
}
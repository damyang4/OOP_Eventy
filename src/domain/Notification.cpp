#include "Notification.h"

Notification::Notification(std::string userId, std::string message, std::string timestamp, bool isRead)
        : userId(userId), message(message), timestamp(timestamp), isRead(isRead) {}

std::string Notification::getUserId() const { return userId; }
std::string Notification::getMessage() const { return message; }
std::string Notification::getTimestamp() const { return timestamp; }
bool Notification::getIsRead() const { return isRead; }

void Notification::markAsRead() { isRead = true; }

// Formats payload to write perfectly as a flat file line
std::string Notification::serialize() const {
    // format: userId|message|timestamp|isRead
    return userId + "|" + message + "|" + timestamp + "|" + (isRead ? "1" : "0");
}
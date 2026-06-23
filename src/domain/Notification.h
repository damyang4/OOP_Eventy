#pragma once

#include <string>

class Notification {
private:
    std::string userId;
    std::string message;
    std::string timestamp;
    bool isRead;

public:
    Notification(std::string userId, std::string message, std::string timestamp, bool isRead = false);

    std::string getUserId() const;
    std::string getMessage() const;
    std::string getTimestamp() const;
    bool getIsRead() const;

    void markAsRead();

    // Formats payload to write perfectly as a flat file line
    std::string serialize() const;
};
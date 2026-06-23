#pragma once
#include "../domain/Notification.h"
#include <string>
#include <vector>

class NotificationManager {
public:
    // Dispatches an unread alert to a target user
    static void sendNotification(const std::string& userId, const std::string& message);
    
    // Retrieves all notifications belonging to a specific user
    static std::vector<Notification> getUserNotifications(const std::string& userId);
    
    // Marks all pending notifications for a specific user as read
    static void markAllAsRead(const std::string& userId);
};
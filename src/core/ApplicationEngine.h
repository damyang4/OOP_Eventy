#pragma once

#include "../domain/User.h"
#include "../domain/Event.h"
#include "../services/UserManager.h"
#include "../services/EventManager.h"
#include "../services/NotificationManager.h"

class ApplicationEngine {
private:
    User* currentUser;
    Event* activeEvent; // Handles context switching into specific event management modes
    bool running;

    UserManager userManager;
    EventManager eventManager;
    NotificationManager notificationManager;

public:
    ApplicationEngine();
    ~ApplicationEngine();

    void start();
    void stop();

    User* getCurrentUser() const;
    void setCurrentUser(User* user);

    Event* getActiveEvent() const;
    void setActiveEvent(Event* ev);

    UserManager& getUserManager();
    EventManager& getEventManager();
    NotificationManager& getNotificationManager();
};
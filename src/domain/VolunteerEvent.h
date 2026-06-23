#pragma once
#include "Event.h"

class VolunteerEvent : public Event {
private:
    int maxVolunteers;
    int currentVolunteers;
    std::string tasksDescription;

public:
    VolunteerEvent(std::string id, std::string title, std::string description, std::string date, std::string creatorId, bool isApproved, int maxVolunteers, int currentVolunteers, std::string tasks);

    int getMaxVolunteers() const;
    int getCurrentVolunteers() const;
    std::string getTasksDescription() const;
    
    bool addVolunteer();
    void removeVolunteer();

    std::string serialize() const override;

    std::string getTaskDescription() const;
};
#include "VolunteerEvent.h"
#include <sstream>

VolunteerEvent::VolunteerEvent(std::string id, std::string title, std::string description, std::string date, std::string creatorId, bool isApproved, int maxVolunteers, int currentVolunteers, std::string tasks)
    : Event(id, title, description, date, creatorId, isApproved, EventType::VOLUNTEER), maxVolunteers(maxVolunteers), currentVolunteers(currentVolunteers), tasksDescription(tasks) {}

int VolunteerEvent::getMaxVolunteers() const { return maxVolunteers; }
int VolunteerEvent::getCurrentVolunteers() const { return currentVolunteers; }
std::string VolunteerEvent::getTasksDescription() const { return tasksDescription; }

bool VolunteerEvent::addVolunteer() {
    if (currentVolunteers < maxVolunteers) {
        currentVolunteers++;
        return true;
    }
    return false;
}

void VolunteerEvent::removeVolunteer() {
    if (currentVolunteers > 0) {
        currentVolunteers--;
    }
}

std::string VolunteerEvent::serialize() const {
    std::ostringstream ss;
    ss << "VOLUNTEER|" << id << "|" << title << "|" << description << "|" << date << "|" << creatorId << "|" << (isApproved ? "1" : "0") << "|" << maxVolunteers << "|" << currentVolunteers << "|" << tasksDescription;
    return ss.str();
}

std::string VolunteerEvent::getTaskDescription() const {
    return this->tasksDescription;
}
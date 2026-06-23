#include "Event.h"

Event::Event(std::string id, std::string title, std::string description, std::string date, std::string creatorId, bool isApproved, EventType type)
    : id(id), title(title), description(description), date(date), creatorId(creatorId), isApproved(isApproved), type(type) {}

std::string Event::getId() const { return id; }
std::string Event::getTitle() const { return title; }
std::string Event::getDescription() const { return description; }
std::string Event::getDate() const { return date; }
std::string Event::getCreatorId() const { return creatorId; }
bool Event::getIsApproved() const { return isApproved; }
EventType Event::getType() const { return type; }

void Event::approve() { isApproved = true; }
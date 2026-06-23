#pragma once
#include <string>

enum class EventType {
    TICKET,
    VOLUNTEER
};

class Event {
protected:
    std::string id;
    std::string title;
    std::string description;
    std::string date;
    std::string creatorId;
    bool isApproved;
    EventType type;

public:
    Event(std::string id, std::string title, std::string description, std::string date, std::string creatorId, bool isApproved, EventType type);
    virtual ~Event() = default;

    std::string getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getDate() const;
    std::string getCreatorId() const;
    bool getIsApproved() const;
    EventType getType() const;

    void approve();
    
    virtual std::string serialize() const = 0;
};
#pragma once
#include <string>
#include <vector>

class Ticket {
private:
    std::string id;
    std::string eventId;
    std::string clientId;
    std::string seatInfo; // "General" or coordinates like "Row 3, Col 5"

public:
    Ticket(std::string id, std::string eventId, std::string clientId, std::string seatInfo);

    std::string getId() const;
    std::string getEventId() const;
    std::string getClientId() const;
    std::string getSeatInfo() const;

    std::string serialize() const;
    static Ticket deserialize(const std::vector<std::string>& columns);
};
#include "Ticket.h"

Ticket::Ticket(std::string id, std::string eventId, std::string clientId, std::string seatInfo)
    : id(id), eventId(eventId), clientId(clientId), seatInfo(seatInfo) {}

std::string Ticket::getId() const { return id; }
std::string Ticket::getEventId() const { return eventId; }
std::string Ticket::getClientId() const { return clientId; }
std::string Ticket::getSeatInfo() const { return seatInfo; }

std::string Ticket::serialize() const {
    return id + "|" + eventId + "|" + clientId + "|" + seatInfo;
}

Ticket Ticket::deserialize(const std::vector<std::string>& columns) {
    // Expected order: id|eventId|clientId|seatInfo
    return Ticket(columns[0], columns[1], columns[2], columns[3]);
}
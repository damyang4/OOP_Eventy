#include "TicketEvent.h"
#include <sstream>

TicketEvent::TicketEvent(std::string id, std::string title, std::string description, std::string date, std::string creatorId, bool isApproved, double price, SeatingGrid grid)
    : Event(id, title, description, date, creatorId, isApproved, EventType::TICKET), ticketPrice(price), seatingGrid(grid) {}

double TicketEvent::getTicketPrice() const { return ticketPrice; }

SeatingGrid& TicketEvent::getSeatingGrid() { return seatingGrid; }

std::string TicketEvent::serialize() const {
    std::ostringstream ss;
    ss << "TICKET|" << id << "|" << title << "|" << description << "|" << date << "|" << creatorId << "|" << (isApproved ? "1" : "0") << "|" << ticketPrice << "|" << seatingGrid.serialize();
    return ss.str();
}
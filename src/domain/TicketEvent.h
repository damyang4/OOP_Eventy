#pragma once
#include "Event.h"
#include "SeatingGrid.h"

class TicketEvent : public Event {
private:
    double ticketPrice;
    SeatingGrid seatingGrid;

public:
    TicketEvent(std::string id, std::string title, std::string description, std::string date, std::string creatorId, bool isApproved, double price, SeatingGrid grid);

    double getTicketPrice() const;
    SeatingGrid& getSeatingGrid();
    
    std::string serialize() const override;
};
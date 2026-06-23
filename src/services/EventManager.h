#pragma once

#include "../domain/Event.h"
#include "../domain/TicketEvent.h"
#include "../domain/VolunteerEvent.h"
#include "../domain/PublishRequest.h"
#include <string>
#include <vector>

class EventManager {
private:
    std::string generateNextEventId();
    std::string generateNextRequestId();
    std::string generateNextTicketId();
    
    // Helper to parse polymorphic data records cleanly
    Event* instantiateFromRow(const std::vector<std::string>& row);

public:
    // Creates a structural payload event and logs a PENDING request line
    bool createEvent(const std::string& title, const std::string& desc, const std::string& date, const std::string& creatorId, bool isTicket, double price, int opt1, int opt2, const std::string& tasks);

    // Dynamic filtering for visible system models
    std::vector<Event*> getAllApprovedEvents();
    std::vector<PublishRequest> getPendingPublishRequests();
    
    // Admin state transition actions
    bool processPublishRequest(const std::string& requestId, bool approve);
    
    // Ticket purchase execution 
    bool purchaseTicket(const std::string& eventId, const std::string& clientId, int count, int row = 0, int col = 0);
    
    // Volunteer registration process
    bool applyAsVolunteer(const std::string& eventId, const std::string& volunteerId);

    Event* getEventById(const std::string& eventId);
};
#include "EventManager.h"
#include "../storage/PersistenceEngine.h"
#include "../core/Config.h"
#include "NotificationManager.h"
#include "UserManager.h"
#include <sstream>
#include <fstream>

std::string EventManager::generateNextEventId() {
    auto rows = PersistenceEngine::readRows(Config::EVENTS_FILE);
    return "EVT_" + std::to_string(rows.size() + 1);
}

std::string EventManager::generateNextRequestId() {
    auto rows = PersistenceEngine::readRows(Config::REQUESTS_FILE);
    return "REQ_" + std::to_string(rows.size() + 1);
}

std::string EventManager::generateNextTicketId() {
    // Quick fallback counting utility file mapping rows
    std::ifstream file("tickets.csv");
    int count = 0;
    std::string dummy;
    while (std::getline(file, dummy)) count++;
    return "TCK_" + std::to_string(count + 1);
}

Event* EventManager::instantiateFromRow(const std::vector<std::string>& row) {
    if (row.empty()) return nullptr;
    
    bool approved = (row[6] == "1");
    if (row[0] == "TICKET" && row.size() >= 9) {
        double price = std::stod(row[7]);
        SeatingGrid grid = SeatingGrid::deserialize(row[8]);
        return new TicketEvent(row[1], row[2], row[3], row[4], row[5], approved, price, grid);
    } else if (row[0] == "VOLUNTEER" && row.size() >= 10) {
        int maxV = std::stoi(row[7]);
        int curV = std::stoi(row[8]);
        return new VolunteerEvent(row[1], row[2], row[3], row[4], row[5], approved, maxV, curV, row[9]);
    }
    return nullptr;
}

bool EventManager::createEvent(const std::string& title, const std::string& desc, const std::string& date, const std::string& creatorId, bool isTicket, double price, int opt1, int opt2, const std::string& tasks) {
    std::string eId = generateNextEventId();
    std::string rId = generateNextRequestId();
    
    std::vector<std::string> eventRow;
    if (isTicket) {
        SeatingGrid grid = (opt2 > 0) ? SeatingGrid(opt1, opt2) : SeatingGrid(opt1);
        eventRow = { "TICKET", eId, title, desc, date, creatorId, "0", std::to_string(price), grid.serialize() };
    } else {
        eventRow = { "VOLUNTEER", eId, title, desc, date, creatorId, "0", std::to_string(opt1), "0", tasks };
    }
    
    if (PersistenceEngine::writeRow(Config::EVENTS_FILE, eventRow)) {
        std::vector<std::string> reqRow = { "PUBLISH", rId, eId, creatorId, "PENDING" };
        PersistenceEngine::writeRow(Config::REQUESTS_FILE, reqRow);
        return true;
    }
    return false;
}

std::vector<Event*> EventManager::getAllApprovedEvents() {
    std::vector<Event*> list;
    auto rows = PersistenceEngine::readRows(Config::EVENTS_FILE);
    for (const auto& row : rows) {
        Event* e = instantiateFromRow(row);
        if (e) {
            if (e->getIsApproved()) list.push_back(e);
            else delete e;
        }
    }
    return list;
}

std::vector<PublishRequest> EventManager::getPendingPublishRequests() {
    std::vector<PublishRequest> list;
    auto rows = PersistenceEngine::readRows(Config::REQUESTS_FILE);
    for (const auto& row : rows) {
        if (row.size() >= 5 && row[0] == "PUBLISH" && row[4] == "PENDING") {
            list.push_back(PublishRequest(row[1], row[2], row[3], RequestStatus::PENDING));
        }
    }
    return list;
}

bool EventManager::processPublishRequest(const std::string& requestId, bool approve) {
    auto reqRows = PersistenceEngine::readRows(Config::REQUESTS_FILE);
    std::string targetEventId, creatorId;
    bool foundReq = false;

    PersistenceEngine::clearFile(Config::REQUESTS_FILE);
    for (auto& r : reqRows) {
        if (r.size() >= 5 && r[1] == requestId) {
            r[4] = approve ? "APPROVED" : "REJECTED";
            targetEventId = r[2];
            creatorId = r[3];
            foundReq = true;
        }
        PersistenceEngine::writeRow(Config::REQUESTS_FILE, r);
    }

    if (!foundReq) return false;

    auto evtRows = PersistenceEngine::readRows(Config::EVENTS_FILE);
    PersistenceEngine::clearFile(Config::EVENTS_FILE);
    for (auto& e : evtRows) {
        if (e.size() >= 7 && e[1] == targetEventId) {
            if (approve) e[6] = "1"; // Approved identity mutation
        }
        PersistenceEngine::writeRow(Config::EVENTS_FILE, e);
    }

    NotificationManager::sendNotification(creatorId, "Your publish request for event " + targetEventId + (approve ? " was APPROVED." : " was REJECTED."));
    return true;
}

bool EventManager::purchaseTicket(const std::string& eventId, const std::string& clientId, int count, int row, int col) {
    auto rows = PersistenceEngine::readRows(Config::EVENTS_FILE);
    bool transacted = false;
    double cost = 0.0;

    for (auto& r : rows) {
        if (r.size() >= 9 && r[1] == eventId && r[0] == "TICKET") {
            double price = std::stod(r[7]);
            cost = price * count;
            SeatingGrid grid = SeatingGrid::deserialize(r[8]);

            UserManager um;
            // Immediate structural validation checks before making changes
            if (!grid.isMapped()) {
                if (grid.reserveGeneral(count)) {
                    if (um.updateUserBalance(clientId, cost, true)) {
                        r[8] = grid.serialize();
                        transacted = true;
                    } else {
                        grid.releaseGeneral(count); // Rolling back changes on error
                    }
                }
            } else {
                if (grid.reserveSeat(row, col)) {
                    if (um.updateUserBalance(clientId, cost, true)) {
                        r[8] = grid.serialize();
                        transacted = true;
                    } else {
                        grid.releaseSeat(row, col); // Rolling back changes on error
                    }
                }
            }
            break;
        }
    }

    if (transacted) {
        PersistenceEngine::clearFile(Config::EVENTS_FILE);
        for (const auto& r : rows) PersistenceEngine::writeRow(Config::EVENTS_FILE, r);

        std::string tId = generateNextTicketId();
        std::string seatDesc = (row > 0) ? ("Row " + std::to_string(row) + ", Col " + std::to_string(col)) : "General Admission";
        std::vector<std::string> ticketRow = { tId, eventId, clientId, seatDesc };
        PersistenceEngine::writeRow("tickets.csv", ticketRow);

        NotificationManager::sendNotification(clientId, "Ticket purchase successful for event: " + eventId + " Balance deducted: " + std::to_string(cost));
        return true;
    }
    return false;
}

bool EventManager::applyAsVolunteer(const std::string& eventId, const std::string& volunteerId) {
    auto rows = PersistenceEngine::readRows(Config::EVENTS_FILE);
    bool complete = false;

    for (auto& r : rows) {
        if (r.size() >= 10 && r[1] == eventId && r[0] == "VOLUNTEER") {
            int maxV = std::stoi(r[7]);
            int curV = std::stoi(r[8]);
            if (curV < maxV) {
                curV++;
                r[8] = std::to_string(curV);
                complete = true;
            }
            break;
        }
    }

    if (complete) {
        PersistenceEngine::clearFile(Config::EVENTS_FILE);
        for (const auto& r : rows) PersistenceEngine::writeRow(Config::EVENTS_FILE, r);

        std::vector<std::string> vRow = { eventId, volunteerId };
        PersistenceEngine::writeRow("event_volunteers.csv", vRow);
        NotificationManager::sendNotification(volunteerId, "You have successfully registered as a volunteer for event: " + eventId);
        return true;
    }
    
    return false;
}

Event* EventManager::getEventById(const std::string& eventId) {
    std::vector<Event*> events = getAllApprovedEvents();
    Event* matchedEvent = nullptr;

    for (auto* curr : events) {
        if (curr != nullptr && curr->getId() == eventId) {
            matchedEvent = curr; 
        } else {
            // Clean up non-matching objects
            delete curr; 
        }
    }

    return matchedEvent;
}
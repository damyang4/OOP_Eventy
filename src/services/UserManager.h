#pragma once

#include "../domain/User.h"
#include <string>
#include <vector>

class UserManager {
private:
    // Helper to generate completely unique serial Client IDs
    std::string generateNextId();

public:
    // Creates a new client profile if the username is unique
    bool registerClient(const std::string& username, const std::string& password);
    
    // Validates credentials and returns an allocated User pointer (caller takes ownership)
    User* authenticate(const std::string& username, const std::string& password);
    
    // Safely modifies a client's stored wallet balance inside the CSV database
    bool updateUserBalance(const std::string& userId, double balanceAdjustment, bool isDeduction);
    
    // Validates whether a given username is already taken
    bool usernameExists(const std::string& username);

    // Fetches a client's current wallet balance directly from the CSV storage 
    double getUserBalance(const std::string& userId);

    // // Prints or returns all tickets currently owned by a specific client
    // void printOwnedTickets(const std::string& userId);

    // // Prints or returns a unified view of both paid tickets and accepted volunteer actions
    // void printParticipationHistory(const std::string& userId);
};
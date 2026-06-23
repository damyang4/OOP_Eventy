#pragma once

#include "User.h"
#include <vector>

class Client : public User {
private:
    double balance;
    std::vector<std::string> createdEventIds;

public:
    Client(std::string id, std::string username, std::string password, double balance = 0.0);

    double getBalance() const;
    void addBalance(double amount);
    bool deductBalance(double amount);
    void setBalance(double amount);
    void linkCreatedEvent(const std::string& eventId);
    
    std::string serialize() const override;
};
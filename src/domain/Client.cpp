#include "Client.h"
#include <sstream>

Client::Client(std::string id, std::string username, std::string password, double balance)
    : User(id, username, password, UserType::CLIENT), balance(balance) {}

double Client::getBalance() const { return balance; }

void Client::addBalance(double amount) {
    if (amount > 0) balance += amount;
}

bool Client::deductBalance(double amount) {
    if (balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}

void Client::setBalance(double amount) {
    this->balance = balance;
}

void Client::linkCreatedEvent(const std::string& eventId) {
    createdEventIds.push_back(eventId);
}

std::string Client::serialize() const {
    std::ostringstream ss;
    ss << "CLIENT|" << id << "|" << username << "|" << password << "|" << balance;
    return ss.str();
}
#include "User.h"

User::User(std::string id, std::string username, std::string password, UserType type)
    : id(id), username(username), password(password), type(type) {}

std::string User::getId() const { return id; }
std::string User::getUsername() const { return username; }
UserType User::getType() const { return type; }

bool User::verifyPassword(const std::string& inputPassword) const {
    return password == inputPassword;
}
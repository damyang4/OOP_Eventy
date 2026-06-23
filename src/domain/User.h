#pragma once

#include <string>

enum class UserType {
    GUEST,
    CLIENT,
    ADMIN
};

class User {
protected:
    std::string id;
    std::string username;
    std::string password;
    UserType type;

public:
    User(std::string id, std::string username, std::string password, UserType type);
    virtual ~User() = default;

    std::string getId() const;
    std::string getUsername() const;
    UserType getType() const;
    bool verifyPassword(const std::string& inputPassword) const;

    virtual std::string serialize() const = 0;
};
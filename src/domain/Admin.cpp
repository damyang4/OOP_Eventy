#include "Admin.h"
#include "../core/Config.h"

Admin::Admin() 
    : User("ADMIN_0", Config::DEFAULT_ADMIN_USER, Config::DEFAULT_ADMIN_PASS, UserType::ADMIN) {}

std::string Admin::serialize() const {
    return "ADMIN|ADMIN_0|" + username + "|" + password + "|0.0";
}
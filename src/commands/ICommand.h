#pragma once
#include <vector>
#include <string>

class ApplicationEngine; // Forward declaration

class ICommand {
public:
    virtual ~ICommand() = default;
    
    // Executes the concrete operation and returns true if successful
    virtual bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) = 0;
};
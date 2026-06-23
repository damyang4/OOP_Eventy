#include "ApplicationEngine.h"
#include "../utils/StringTokenizer.h"
#include "../commands/CommandFactory.h"
#include <iostream>

ApplicationEngine::ApplicationEngine() 
    : currentUser(nullptr), activeEvent(nullptr), running(false) {}

ApplicationEngine::~ApplicationEngine() {
    delete currentUser;
    delete activeEvent;
}

User* ApplicationEngine::getCurrentUser() const { return currentUser; }
void ApplicationEngine::setCurrentUser(User* user) { 
    delete currentUser;
    currentUser = user; 
}

Event* ApplicationEngine::getActiveEvent() const { return activeEvent; }
void ApplicationEngine::setActiveEvent(Event* ev) { 
    delete activeEvent;
    activeEvent = ev; 
}

UserManager& ApplicationEngine::getUserManager() { return userManager; }
EventManager& ApplicationEngine::getEventManager() { return eventManager; }
NotificationManager& ApplicationEngine::getNotificationManager() { return notificationManager; }

void ApplicationEngine::stop() { running = false; }

void ApplicationEngine::start() {
    running = true;
    std::string inputLine;
    
    std::cout << "--- Welcome to Eventy CLI Platform ---\n";
    while (running) {
        // Render precise context line indications
        if (currentUser) {
            std::cout << "[" << currentUser->getUsername() << "";
            if (activeEvent) {
                std::cout << " -> Managing: " << activeEvent->getTitle();
            }
            std::cout << "] $ ";
        } else {
            std::cout << "[Guest] $ ";
        }

        if (!std::getline(std::cin, inputLine)) break;
        if (inputLine.empty()) continue;
        if (inputLine == "exit") {
            stop();
            break;
        }

        auto tokens = StringTokenizer::tokenize(inputLine);
        if (tokens.empty()) continue;

        std::string commandName = tokens[0];
        // Erase command target name to leave payload arguments only
        tokens.erase(tokens.begin()); 

        ICommand* command = CommandFactory::createCommand(commandName);
        if (!command) {
            std::cout << "Error: Unknown target command keyword \"" << commandName << "\". Try again.\n";
            continue;
        }

        try {
            bool result = command->execute(*this, tokens);
            // For debugging
            // if (!result) {
            //     std::cout << "Command status report: Execution returned failure.\n";
            // }
        } catch (const std::exception& e) {
            std::cout << "Systemic Exception caught: " << e.what() << "\n";
        } catch (...) {
            std::cout << "Fatal Systemic Exception: Unknown data layout error encountered.\n";
        }

        delete command; // Garbage clean factory allocation maps immediately
    }
    std::cout << "Exiting Eventy. All session storage layers closed cleanly.\n";
}
#pragma once
#include "ICommand.h"
#include "../core/ApplicationEngine.h"
#include <string>

class CommandFactory {
public:
    // Factory method tracking system context to return appropriate executable allocations
    static ICommand* createCommand(const std::string& commandName);
};

// Concrete implementations declared locally or inline for compact execution
class RegisterCommand : public ICommand {
public:
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class LoginCommand : public ICommand {
public:
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class LogoutCommand : public ICommand {
public:
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class CreateEventCommand : public ICommand {
public:
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class ListEventsCommand : public ICommand {
public:
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class BuyTicketCommand : public ICommand {
public:
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class HelpCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class ApproveRequestCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class RejectRequestCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class ListRequestsCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class WalletCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class AddBalanceCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class EnterEventCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class ViewDetailsCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class CancelEventCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class ExitEventCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class ViewNotificationsCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};

class ShowSeatingCommand : public ICommand {
public: 
    bool execute(ApplicationEngine& engine, const std::vector<std::string>& args) override;
};
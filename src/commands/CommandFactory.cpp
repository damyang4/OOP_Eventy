#include "CommandFactory.h"

#include <iostream>

ICommand* CommandFactory::createCommand(const std::string& commandName) {
    if (commandName == "register") return new RegisterCommand();
    if (commandName == "login")    return new LoginCommand();
    if (commandName == "logout")   return new LogoutCommand();
    if (commandName == "help")   return new HelpCommand();
    // user
    if (commandName == "create-event") return new CreateEventCommand();
    if (commandName == "list-events")  return new ListEventsCommand();
    if (commandName == "buy-ticket")   return new BuyTicketCommand();
    if (commandName == "wallet")   return new WalletCommand();
    if (commandName == "add-balance")   return new AddBalanceCommand();
    if (commandName == "enter-event") return new EnterEventCommand();
    
    // admin
    if (commandName == "approve-request") return new ApproveRequestCommand();
    if (commandName == "reject-request") return new RejectRequestCommand();
    if (commandName == "list-requests") return new ListRequestsCommand();

    if (commandName == "view-details")       return new ViewDetailsCommand();
    if (commandName == "show-seating")       return new ShowSeatingCommand();
    if (commandName == "cancel-event")       return new CancelEventCommand();
    if (commandName == "exit-event")         return new ExitEventCommand();
    if (commandName == "notifications") return new ViewNotificationsCommand();
    return nullptr;
}

// Implementation of Standard Commands
bool RegisterCommand::execute(ApplicationEngine& engine, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Syntax Error: usage: register <username> <password>\n";
        return false;
    }
    if (engine.getCurrentUser() != nullptr) {
        std::cout << "Error: Logged-in accounts cannot run registrations. Please log out first.\n";
        return false;
    }
    if (engine.getUserManager().registerClient(args[0], args[1])) {
        std::cout << "Success: Client account registered successfully.\n";
        return true;
    } else {
        std::cout << "Error: Registration failed. Username may already be taken.\n";
        return false;
    }
}

bool LoginCommand::execute(ApplicationEngine& engine, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Syntax Error: usage: login <username> <password>\n";
        return false;
    }
    if (engine.getCurrentUser() != nullptr) {
        std::cout << "Error: A user session is already open. Close it before changing credentials.\n";
        return false;
    }
    
    User* authed = engine.getUserManager().authenticate(args[0], args[1]);
    if (authed) {
        engine.setCurrentUser(authed);
        std::cout << "Success: Authentication completed. Welcome back, " << authed->getUsername() << "!\n";
        return true;
    } else {
        std::cout << "Error: Authentication failed. Invalid username or password.\n";
        return false;
    }
}

bool LogoutCommand::execute(ApplicationEngine& engine, const std::vector<std::string>& args) {
    if (!engine.getCurrentUser()) {
        std::cout << "Error: No session active to drop.\n";
        return false;
    }
    engine.setCurrentUser(nullptr);
    engine.setActiveEvent(nullptr); // Clear isolation modes upon session end
    std::cout << "Success: Session closed. Context returned to Guest mode.\n";
    return true;
}

bool CreateEventCommand::execute(ApplicationEngine& engine, const std::vector<std::string>& args) {
    if (!engine.getCurrentUser() || engine.getCurrentUser()->getType() != UserType::CLIENT) {
        std::cout << "Access Error: Only registered Clients can create new event listings.\n";
        return false;
    }
    
    // Check baseline syntax requirements
    if (args.size() < 5) {
        std::cout << "Syntax Error variants:\n"
                  << " 1. Volunteer: create-event <title> <desc> <date> volunteer <max_volunteers> <tasks>\n"
                  << " 2. General Ticket: create-event <title> <desc> <date> ticket <price> <capacity>\n"
                  << " 3. Mapped Ticket:  create-event <title> <desc> <date> ticket <price> <rows> <cols>\n";
        return false;
    }

    std::string title = args[0];
    std::string desc  = args[1];
    std::string date  = args[2];
    std::string type  = args[3];

    bool isTicket = (type == "ticket");
    double price = 0.0;
    std::string tasks = "";
    
    int opt1 = 0; // Will store capacity OR rows
    int opt2 = 0; // Will store cols (if mapped)

    try {
        if (isTicket) {
            price = std::stod(args[4]);
            
            // Look ahead at arguments to decide if it's Mapped (rows & cols) or General (capacity)
            if (args.size() >= 7) {
                opt1 = std::stoi(args[5]); // rows
                opt2 = std::stoi(args[6]); // cols
            } else if (args.size() >= 6) {
                opt1 = std::stoi(args[5]); // capacity flat
                opt2 = 0;                  // Indicates unmapped (General Admission)
            } else {
                std::cout << "Syntax Error: Missing seating capacity configuration details.\n";
                return false;
            }
        } else {
            // Volunteer execution path
            opt1 = std::stoi(args[4]); // Max volunteers capacity limit
            if (args.size() >= 6) {
                tasks = args[5];
            }
        }
    } catch (...) {
        std::cout << "Parsing Error: Numeric values expected for price, capacity, or row/col inputs.\n";
        return false;
    }

    // Call your EventManager backend method passing the structural grid sizes
    bool res = engine.getEventManager().createEvent(
        title, desc, date, engine.getCurrentUser()->getId(), 
        isTicket, price, opt1, opt2, tasks
    );
    
    if (res) {
        std::cout << "Success: Event placeholder initialized. Request sent to Admin queue.\n";
        return true;
    }
    
    std::cout << "Execution Error: Service engine failed to record the event transaction.\n";
    return false;
}

bool ListEventsCommand::execute(ApplicationEngine& engine, const std::vector<std::string>& args) {
    auto list = engine.getEventManager().getAllApprovedEvents();
    if (list.empty()) {
        std::cout << "System state: No public approved events currently open for view.\n";
        return true;
    }
    std::cout << "\n--- Currently Active System Events ---\n";
    for (auto* e : list) {
        std::cout << "ID: " << e->getId() << " | Title: " << e->getTitle() << " | Date: " << e->getDate() << "\nDesc: " << e->getDescription() << "\n-------------------------------------\n";
        delete e; // Clean up the copy returned by the manager
    }
    return true;
}

bool BuyTicketCommand::execute(ApplicationEngine& engine, const std::vector<std::string>& args) {
    if (!engine.getCurrentUser() || engine.getCurrentUser()->getType() != UserType::CLIENT) {
        std::cout << "Error: Only logged-in clients can purchase event tickets.\n";
        return false;
    }
    if (args.size() < 2) {
        std::cout << "Syntax Error: buy-ticket <eventId> <count> [row] [col]\n";
        return false;
    }

    std::string eId = args[0];
    int count = std::stoi(args[1]);
    int r = 0, c = 0;
    if (args.size() >= 4) {
        r = std::stoi(args[2]);
        c = std::stoi(args[3]);
    }

    if (engine.getEventManager().purchaseTicket(eId, engine.getCurrentUser()->getId(), count, r, c)) {
        std::cout << "Success: Ticket transaction completed. Check your notifications for receipt information.\n";
        return true;
    } else {
        std::cout << "Transaction Error: Booking rejected. Check capacity configurations or wallet balances.\n";
        return false;
    }
}

bool HelpCommand::execute(ApplicationEngine& engine, const std::vector<std::string>& args) {
    std::cout << "\n================= EVENTY HELP SYSTEM =================\n";
        
        // Context 1: Guest Commands (Always available when logged out)
        if (engine.getCurrentUser() == nullptr) {
            std::cout << " Available Guest Commands:\n";
            std::cout << "   register <username> <password>  - Create a new client profile\n";
            std::cout << "   login <username> <password>     - Log into an existing profile\n";
            std::cout << "   list-events                     - View all approved public events\n";
        } 
        // Context 2: Logged-in User Commands
        else {
            UserType role = engine.getCurrentUser()->getType();
            
            if (role == UserType::CLIENT) {
                // If they are a Client, check if they are in "Event Management Mode"
                if (engine.getActiveEvent() != nullptr) {
                    std::cout << " Active Event Management Mode Commands:\n";
                    std::cout << "   view-details                    - See capacity & details\n";
                    std::cout << "   show-seating                    - See full layout\n";
                    std::cout << "   edit-event <field> <value>      - Modify descriptions/tasks\n";
                    std::cout << "   cancel-event                    - Cancel the event & refund buyers\n";
                    std::cout << "   exit-event                      - Exit management mode back to main dashboard\n";
                } else {
                    std::cout << " Available Client Commands:\n";
                    std::cout << "   create-event <title> <desc> <date> <ticket/volunteer> <capacity> [price/tasks]\n";
                    std::cout << "   list-events                              - View all approved public events\n";
                    std::cout << "   buy-ticket <eventId> <count> [row] [col] - Purchase seats\n";
                    std::cout << "   enter-event <eventId>                    - Enter specialized management mode for your event\n";
                    std::cout << "   wallet                                   - Check balance / Add simulated funds\n";
                    std::cout << "   add-balance                              - Add simulated funds\n";
                    std::cout << "   notifications                            - Open notification center inbox\n";
                }
            } 
            else if (role == UserType::ADMIN) {
                std::cout << " Available Administrator Commands:\n";
                std::cout << "   list-requests                   - View all pending event publish requests\n";
                std::cout << "   approve-request <reqId>         - Approve an event to make it public\n";
                std::cout << "   reject-request <reqId>          - Reject and drop a publication request\n";
            }
            
            // Common logout command for anyone authenticated
            std::cout << "   logout                          - Disconnect and drop current session\n";
        }
        
    // Universal command
    std::cout << "   exit                            - Completely shut down the application cleanly\n";
    std::cout << "======================================================\n\n";
    return true;
}

bool ApproveRequestCommand::execute(ApplicationEngine& engine, const std::vector<std::string>& args) {
    if (!engine.getCurrentUser() || engine.getCurrentUser()->getType() != UserType::ADMIN) {
        std::cout << "Error: Administrative privileges required.\n";
        return false;
    }

    if (args.empty()) {
        std::cout << "Usage error: approve-request <request_id>\n";
        return false;
    }

    std::string requestId = args[0];
    
    // Call your processPublishRequest method with approve = true
    if (engine.getEventManager().processPublishRequest(requestId, true)) {
        std::cout << "Success: Request " << requestId << " has been approved and published.\n";
        return true;
    } else {
        std::cout << "Error: Failed to process request. Invalid Request ID or state.\n";
        return false;
    }
}

bool RejectRequestCommand::execute(ApplicationEngine& engine, const std::vector<std::string>& args) {
    if (!engine.getCurrentUser() || engine.getCurrentUser()->getType() != UserType::ADMIN) {
        std::cout << "Error: Administrative privileges required.\n";
        return false;
    }

    if (args.empty()) {
        std::cout << "Usage error: reject-request <request_id> [optional_reason]\n";
        return false;
    }

    std::string requestId = args[0];
    
    // Call your processPublishRequest method with approve = false
    if (engine.getEventManager().processPublishRequest(requestId, false)) {
        std::cout << "Success: Request " << requestId << " has been rejected and closed.\n";
        return true;
    } else {
        std::cout << "Error: Failed to process request. Invalid Request ID or state.\n";
        return false;
    }
}

bool ListRequestsCommand::execute(ApplicationEngine& engine, const std::vector<std::string>&) { 
    // Call your exact method (returns by value, not pointers)
    if (!engine.getCurrentUser() || engine.getCurrentUser()->getType() != UserType::ADMIN) {
        std::cout << "Error: Administrative privileges required.\n";
        return false;
    }
    
    std::vector<PublishRequest> list = engine.getEventManager().getPendingPublishRequests();
    
    if (list.empty()) {
        std::cout << "System state: No pending requests.\n";
        return true;
    }
    
    std::cout << "\n--- Pending Requests ---\n";
    for (const auto& req: list) {
        std::cout << "Request ID: " << req.getId() 
                  << " | Event ID: " << req.getTargetId() 
                  << " | Status: Pending\n"
                  << "-------------------------------------\n";
    }

    return true;
}

bool WalletCommand::execute(ApplicationEngine& engine, const std::vector<std::string>&) {
    if(!engine.getCurrentUser()) return false;
    std::cout << "Current Balance: $" << engine.getUserManager().getUserBalance(engine.getCurrentUser()->getId()) << "\n"; return true;
}

class Client;
#include "../domain/Client.h"

bool AddBalanceCommand::execute(ApplicationEngine& eng, const std::vector<std::string>& args) {
    // Ensure someone is logged in
    if (!eng.getCurrentUser()) {
        std::cout << "Error: You must be logged in to modify your wallet.\n";
        return false;
    }
    
    // Admin isn't trying to add wallet funds
    if (eng.getCurrentUser()->getType() != UserType::CLIENT) {
        std::cout << "Access Error: Only Client accounts maintain financial wallets.\n";
        return false;
    }
    
    if (args.empty()) {
        std::cout << "Usage error: add-balance <amount>\n";
        return false;
    }
    
    try {
        double amount = std::stod(args[0]);
        if (amount <= 0) {
            std::cout << "Error: Deposit amount must be positive.\n";
            return false;
        }

        std::string userId = eng.getCurrentUser()->getId();

        // Persist the change
        if (eng.getUserManager().updateUserBalance(userId, amount, false)) {
            
            // Fetch the freshly written balance directly from the engine manager
            double newBalance = eng.getUserManager().getUserBalance(userId);

            // Cast base User* to derived Client*
            Client* clientUser = static_cast<Client*>(eng.getCurrentUser());
            clientUser->setBalance(newBalance); 

            std::cout << "Success: $" << amount << " added to your wallet.\n";
            std::cout << "New live profile balance: $" << newBalance << "\n";
            return true;
        } else {
            std::cout << "Error: Failed to write transaction record to database file.\n";
            return false;
        }
    } 
    catch (const std::invalid_argument&) {
        std::cout << "Syntax Error: Allocation token must be a numeric fractional value.\n";
        return false;
    }
    catch (const std::out_of_range&) {
        std::cout << "Syntax Error: Input value exceeds numerical balance limits.\n";
        return false;
    }
}

bool EnterEventCommand::execute(ApplicationEngine& engine, const std::vector<std::string>& args) {
    if(args.empty() || !engine.getCurrentUser()) return false;

    Event* e = engine.getEventManager().getEventById(args[0]);
    if(e && e->getCreatorId() == engine.getCurrentUser()->getId()) { 
        engine.setActiveEvent(e); 
        return true; 
    }

    delete e; 
    return false;
}

// --- [MANAGEMENT MODE] View Details Command ---
bool ViewDetailsCommand::execute(ApplicationEngine& engine, const std::vector<std::string>&) {
    if (!engine.getCurrentUser() || !engine.getActiveEvent()) {
        std::cout << "Error: No event active in management scope.\n";
        return false;
    }

    Event* e = engine.getActiveEvent();
    std::cout << "\n================ EVENT MANAGEMENT SHEET ================\n";
    std::cout << " ID: " << e->getId() << "\n";
    std::cout << " Title: " << e->getTitle() << "\n";
    std::cout << " Schedule: " << e->getDate() << "\n";
    std::cout << " Description: " << e->getDescription() << "\n";
    
    // Check if polymorphism holds an underlying Ticket Event or Volunteer structure
    if (e->getType() == EventType::TICKET) {
        TicketEvent* te = static_cast<TicketEvent*>(e);
        std::cout << " Type: Paid Seating Assignment\n";
        std::cout << " Ticket Entry Price: $" << te->getTicketPrice() << "\n";
    } else {
        VolunteerEvent* ve = static_cast<VolunteerEvent*>(e);
        std::cout << " Type: Civic Volunteer Outing\n";
        std::cout << " Allocated Operational Tasks: " << ve->getTaskDescription() << "\n";
    }
    std::cout << "========================================================\n\n";
    return true;
}

// --- [MANAGEMENT MODE] Cancel Event Command ---
bool CancelEventCommand::execute(ApplicationEngine& engine, const std::vector<std::string>&) {
    if (!engine.getCurrentUser() || !engine.getActiveEvent()) {
        std::cout << "Error: No event active to cancel.\n";
        return false;
    }

    std::string eventId = engine.getActiveEvent()->getId();
    
    // Mark as cancelled/inactive via the event management system
    // (This uses the processPublishRequest or fallback cancellation pipeline)
    if (engine.getEventManager().processPublishRequest(eventId, false)) {
        std::cout << "Success: Event " << eventId << " has been removed and marked cancelled.\n";
        engine.setActiveEvent(nullptr); // Exit isolation context automatically
        return true;
    }
    
    std::cout << "Error: Failed to safely cancel event records.\n";
    return false;
}

// --- [MANAGEMENT MODE] Exit Event Context Mode ---
bool ExitEventCommand::execute(ApplicationEngine& engine, const std::vector<std::string>&) {
    if (!engine.getActiveEvent()) {
        std::cout << "Error: You are not currently inside an isolated event management scope.\n";
        return false;
    }

    engine.setActiveEvent(nullptr);
    std::cout << "Success: Exited event context. Returned safely to your main profile dashboard.\n";
    return true;
}

// --- [CLIENT DASHBOARD] View Notifications Command ---
bool ViewNotificationsCommand::execute(ApplicationEngine& engine, const std::vector<std::string>&) {
    if (!engine.getCurrentUser()) {
        std::cout << "Error: Active login session required to extract notices.\n";
        return false;
    }

    std::string userId = engine.getCurrentUser()->getId();
    auto alerts = NotificationManager::getUserNotifications(userId);

    if (alerts.empty()) {
        std::cout << "Inbox clear: No unread notifications found.\n";
        return true;
    }

    std::cout << "\n--- Notification Inbox System ---\n";
    for (const auto& alert : alerts) {
        std::cout << " -> " << alert.getMessage() << "\n";
    }
    std::cout << "---------------------------------\n";

    // Batch clear alerts once they are displayed to the client layout interface
    NotificationManager::markAllAsRead(userId);
    return true;
}

bool ShowSeatingCommand::execute(ApplicationEngine& eng, const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage Error: show-seating <eventId>\n";
        return false;
    }

    std::string targetId = args[0];
    Event* e = eng.getEventManager().getEventById(targetId);
    
    if (!e) {
        std::cout << "Database Query Error: Event with ID \"" << targetId << "\" does not exist.\n";
        return false;
    }

    if (e->getType() != EventType::TICKET) {
        std::cout << "Filter Error: Event \"" << e->getTitle() << "\" is a Volunteer event and has no seats.\n";
        delete e;
        return false;
    }

    TicketEvent* te = static_cast<TicketEvent*>(e);
    SeatingGrid& g = te->getSeatingGrid();

    if (g.isMapped()) {
        int totalRows = g.getRows();
        int totalCols = g.getCols();

        if (totalRows <= 0 || totalCols <= 0) {
            std::cout << "Structure Warning: This event has a seat map configuration, but rows/cols are uninitialized (0x0).\n";
            delete e;
            return true;
        }

        std::cout << "\n--- Seating Map Grid Layout for: " << te->getTitle() << " ---\n   ";
        for (int j = 1; j <= totalCols; ++j) std::cout << " " << j << " ";
        std::cout << "\n";

        for (int i = 1; i <= totalRows; ++i) {
            std::cout << i << " |";
            for (int j = 1; j <= totalCols; ++j) {
                std::cout << (g.isSeatOccupied(i, j) ? " [X]" : " [O]");
            }
            std::cout << "\n";
        }
        std::cout << "-----------------------------------------\n";
        std::cout << "Legend: [O] Available Slot | [X] Occupied/Booked\n";
    } else {
        std::cout << "\n--- General Admission Layout ---\n";
        std::cout << "Event Title: " << te->getTitle() << "\n";
        std::cout << "Open seating allocation context. Total available open tickets: " << g.getAvailableCount() << "\n";
    }

    delete e; 
    return true;
}
#pragma once
#include <string>

enum class RequestStatus {
    PENDING,
    APPROVED,
    REJECTED
};

enum class RequestType {
    PUBLISH,
    VOLUNTEER
};

class Request {
protected:
    std::string id;
    std::string targetId; // represent EventId
    std::string requesterId;
    RequestStatus status;
    RequestType type;

public:
    Request(std::string id, std::string targetId, std::string requesterId, RequestStatus status, RequestType type);
    virtual ~Request() = default;

    std::string getId() const;
    std::string getTargetId() const;
    std::string getRequesterId() const;
    RequestStatus getStatus() const;
    RequestType getType() const;

    void setStatus(RequestStatus newStatus);

    virtual std::string serialize() const = 0;
};
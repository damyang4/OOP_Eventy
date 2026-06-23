#include "Request.h"

Request::Request(std::string id, std::string targetId, std::string requesterId, RequestStatus status, RequestType type)
    : id(id), targetId(targetId), requesterId(requesterId), status(status), type(type) {}

std::string Request::getId() const { return id; }
std::string Request::getTargetId() const { return targetId; }
std::string Request::getRequesterId() const { return requesterId; }
RequestStatus Request::getStatus() const { return status; }
RequestType Request::getType() const { return type; }

void Request::setStatus(RequestStatus newStatus) { status = newStatus; }
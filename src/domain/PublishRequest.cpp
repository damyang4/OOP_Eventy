#include "PublishRequest.h"

PublishRequest::PublishRequest(std::string id, std::string eventId, std::string clientCreatorId, RequestStatus status)
    : Request(id, eventId, clientCreatorId, status, RequestType::PUBLISH) {}

std::string PublishRequest::serialize() const {
    std::string statusStr = "PENDING";
    if (status == RequestStatus::APPROVED) statusStr = "APPROVED";
    if (status == RequestStatus::REJECTED) statusStr = "REJECTED";

    return "PUBLISH|" + id + "|" + targetId + "|" + requesterId + "|" + statusStr;
}
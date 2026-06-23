#pragma once
#include "Request.h"

class PublishRequest : public Request {
public:
    PublishRequest(std::string id, std::string eventId, std::string clientCreatorId, RequestStatus status = RequestStatus::PENDING);
    
    std::string serialize() const override;
};
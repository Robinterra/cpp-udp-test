#pragma once

#include "../Data/Net/Server.h"
#include "../Data/Net/UdpControl.h"

class MessageEndpoint : public IEndpoint {
    public:
    MessageEndpoint() {}
    ~MessageEndpoint() {}

    Response<int> Accept(ConnectionContext * connectionContext) override;
};
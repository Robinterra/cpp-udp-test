#include "../Models/Response.h"
#include "../Repos/Messages/IMessageRepo.h"

#pragma once

class MessagePingPongService
{

    private:
    IMessageRepo * m_messageRepo;

    public:

    MessagePingPongService(IMessageRepo * messageRepo)
    {
        m_messageRepo = messageRepo;
    }

    ~MessagePingPongService() {}

    Response<int> Ping()
    {
        Response<int> res = m_messageRepo->SendMessage("Ping", 4);
        return res;
    }

    Response<int> Pong(char * message, int bufferSize)
    {
        Response<int> res = m_messageRepo->GetMessage(message, bufferSize);
        return res;
    }

};
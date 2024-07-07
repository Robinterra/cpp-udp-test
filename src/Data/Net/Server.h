
#pragma once

#include "UdpControl.h"

class ConnectionContext
{
    sockaddr_in m_clientAddr;
    UdpControl * m_udpControl;

    public:
    ConnectionContext(sockaddr_in clientAddr, UdpControl * udpControl)
    {
        m_clientAddr = clientAddr;
        m_udpControl = udpControl;
    }

    ~ConnectionContext() {}

    Response<UdpControl::Result> Send(const char * message, int size)
    {
        return m_udpControl->Send(message, size, (sockaddr*)&m_clientAddr);
    }

    Response<UdpControl::Result> Receive(char * message, int bufferSize)
    {
        return m_udpControl->Receive(message, bufferSize, (sockaddr*)&m_clientAddr);
    }
};

class IEndpoint
{
    public:
    virtual ~IEndpoint() {}

    virtual Response<int> Accept(ConnectionContext * connectionContext) = 0;
};

class Server
{
    sockaddr_in m_listenAddr;
    UdpControl m_udpControl;
    IEndpoint * m_endpoint;

    public:
    Server(sockaddr_in listenAddr, IEndpoint * endpoint)
    {
        m_listenAddr = listenAddr;
        m_endpoint = endpoint;
    }

    ~Server() {}

    Response<UdpControl::Result> Start()
    {
        return m_udpControl.Start((sockaddr*)&m_listenAddr);
    }

    Response<int> Run()
    {
        if (m_endpoint == nullptr) return Error(-1);

        char buffer[1];
        //int size = 0;

        while (true)
        {
            sockaddr_in clientAddr;
            Response<UdpControl::Result> res = m_udpControl.Receive(buffer, sizeof(buffer), (sockaddr*)&clientAddr);
            if (!res.Success) return Error((int)res.Code);

            ConnectionContext connectionContext(clientAddr, &m_udpControl);

            Response<int> rec = m_endpoint->Accept(&connectionContext);
            if (!rec.Success) return Error((int)rec.Code);
        }

        return Ok(0);
    }
};
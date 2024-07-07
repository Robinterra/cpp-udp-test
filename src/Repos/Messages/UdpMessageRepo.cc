#include "UdpMessageRepo.h"

Response<int> UdpMessageRepo::SendMessage(const char * message, int size)
{
    if (message == nullptr) return Error(-1);

    Response<UdpControl::Result> res = m_udpControl.Send(message, size, (sockaddr*)&m_sendAddr);
    if (!res.Success) return Error(-1);

    int sentBytes = res.Payload;

    return Ok(sentBytes);
}

Response<int> UdpMessageRepo::GetMessage(char * message, int bufferSize)
{
    if (message == nullptr) return Error(-1);
    if (bufferSize <= 0) return Error(-1);

    sockaddr_in clientAddr;
    Response<UdpControl::Result> res = m_udpControl.Receive(message, bufferSize, (sockaddr*)&clientAddr);
    if (!res.Success) return Error(-1);

    int receivedBytes = res.Payload;

    return Ok(receivedBytes);
}
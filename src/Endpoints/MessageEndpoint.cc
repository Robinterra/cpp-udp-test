#include "MessageEndpoint.h"

#include <iostream>

int printResult(char * buffer, int size)
{
    if (buffer == nullptr) return -1;
    if (size <= 0) return -1;
    if (size > 100) return -1;

    int i = 0;
    while (i < size && i < 100)
    {
        std::cout << buffer[i++];
    }
    std::cout << std::endl;

    return 0;
}

Response<int> MessageEndpoint::Accept(ConnectionContext * connectionContext)
{
    char buffer[100];
    Response<UdpControl::Result> res = connectionContext->Receive(buffer, 100);
    if (res.Code != UdpControl::Result::OK) return Error(-1);

    int size = res.Payload;
    printResult(buffer, size);

    res = connectionContext->Send(buffer, size);
    if (res.Code != UdpControl::Result::OK) return Error(-1);

    return Ok(0);
}
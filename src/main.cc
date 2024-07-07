// src/main.cpp
#include <iostream>
#include <cstring>
#include <netinet/in.h>

#include "Data/Net/UdpControl.h"
#include "Repos/Messages/UdpMessageRepo.h"
#include "Services/MessagePingPongService.h"

int errorPrint(const char* msg, Response<UdpControl::Result> res)
{
    std::cout << msg << std::endl;
    std::cout << "Error code: " << (int)res.Code << std::endl;
    std::cout << "Error number: " << res.ErrorCode << std::endl;
    return 1;
}

int errorPrint(const char* msg, Response<int> res)
{
    std::cout << msg << std::endl;
    std::cout << "Error code: " << (int)res.Code << std::endl;
    std::cout << "Error number: " << res.ErrorCode << std::endl;
    return 1;
}

int errorPrint(const char* msg)
{
    std::cout << msg << std::endl;
    return 1;
}

const int BUFFER_SIZE = 1024;
char buffer[BUFFER_SIZE];

int setupSockaddr(sockaddr_in * addr)
{
    if (addr == nullptr) return -1;

    memset(addr, 0, sizeof(sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(80);
    addr->sin_addr.s_addr = INADDR_ANY;

    return 0;

}

int printResult(char * buffer, int size)
{
    if (buffer == nullptr) return -1;
    if (size <= 0) return -1;
    if (size > BUFFER_SIZE) return -1;

    int i = 0;
    while (i < size && i < BUFFER_SIZE)
    {
        std::cout << buffer[i++];
    }
    std::cout << std::endl;

    return 0;
}





int main()
{
    // SETUP
    sockaddr_in serverAddr;
    if (setupSockaddr(&serverAddr)) return errorPrint("Failed to setup sockaddr");

    UdpMessageRepo udpMessageRepo(serverAddr);
    Response<UdpControl::Result> res = udpMessageRepo.Setup();
    if (!res.Success) return errorPrint("Failed to setup repo", res);

    MessagePingPongService service((IMessageRepo *)&udpMessageRepo);

    // LOGIC
    Response<int> recPong = service.Pong(buffer, BUFFER_SIZE);
    if (!recPong.Success) return errorPrint("Failed to pong", recPong);

    int receivedSize = recPong.Payload;
    printResult(buffer, receivedSize);

    return 0;
}













    /*UdpControl server;
    Response<UdpControl::Result> res;

    sockaddr_in clientAddr;
    sockaddr_in serverAddr;
    if (setupSockaddr(&serverAddr)) return errorPrint("Failed to setup sockaddr");

    res = server.Start((sockaddr*)&serverAddr);
    if (!res.Success) return errorPrint("Failed to start server", res);

    res = server.Receive(buffer, sizeof(buffer), (sockaddr*)&clientAddr);
    if (!res.Success) return errorPrint("Failed to receive data", res);

    int receivedSize = res.Payload;
    printResult(buffer, receivedSize);

    res = server.Send("Hello from server", 17, (sockaddr*)&clientAddr);
    if (!res.Success) return errorPrint("Failed to send data", res);

    res = server.Stop();
    if (!res.Success) return errorPrint("Failed to stop server", res);*/

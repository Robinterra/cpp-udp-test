// src/main.cpp
#include <iostream>
#include <cstring>
#include <netinet/in.h>

#include "Data/Net/UdpControl.h"
#include "Data/Net/Server.h"
#include "Repos/Messages/UdpMessageRepo.h"
#include "Services/MessagePingPongService.h"
#include "Endpoints/MessageEndpoint.h"

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

int main()
{
    sockaddr_in listenAddr;
    if (setupSockaddr(&listenAddr)) return errorPrint("Failed to setup sockaddr");

    MessageEndpoint endpoint;

    Server server(listenAddr, &endpoint);

    Response<UdpControl::Result> res = server.Start();
    if (!res.Success) return errorPrint("Failed to start server", res);

    Response<int> runRes = server.Run();
    if (!runRes.Success) return errorPrint("Failed to run server", runRes);

    return 0;

    // SETUP
    /*
    sockaddr_in targetAddr;

    UdpMessageRepo udpMessageRepo(listenAddr, targetAddr);
    Response<UdpControl::Result> res = udpMessageRepo.Setup();
    if (!res.Success) return errorPrint("Failed to setup repo", res);

    MessagePingPongService service((IMessageRepo *)&udpMessageRepo);

    // LOGIC
    Response<int> recPong = service.Pong(buffer, BUFFER_SIZE);
    if (!recPong.Success) return errorPrint("Failed to pong", recPong);

    int receivedSize = recPong.Payload;
    printResult(buffer, receivedSize);*/

    //return 0;
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

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#include "../../Models/Response.h"

#pragma once

#define SOCKET_NOT_SET -1

class UdpControl
{
    // Prevent copies and assignments
    UdpControl(const UdpControl&) = delete;
    UdpControl& operator=(const UdpControl&) = delete;

    private:
    int m_socket = SOCKET_NOT_SET;

    public:

    enum class Result
    {
        OK,
        AlreadyStarted,
        NotStarted,
        SocketError,
        BindError,
        SendToError,
        CloseError,
        ParameterInvalid,
        ERROR
    };

    // ------------------------------

    #pragma region ctors

    UdpControl() {}

    ~UdpControl()
    {
        close(m_socket);
    }

    #pragma endregion ctors

    // ------------------------------

    #pragma region StartStop

    Response<Result> Start(sockaddr* serverAddr)
    {
        if (m_socket != SOCKET_NOT_SET) return Error(Result::AlreadyStarted);
        if (serverAddr == nullptr) return Error(Result::ParameterInvalid);

        const int USE_DEFAULT_PROTOCOL = 0;

        int socketDescriptor = socket(AF_INET, SOCK_DGRAM, USE_DEFAULT_PROTOCOL);
        if (socketDescriptor < 0) return SystemError(Result::SocketError);

        const socklen_t clientAddrSize = sizeof(struct sockaddr_in);

        int returnCode = bind(socketDescriptor, serverAddr, clientAddrSize);
        if (returnCode != 0) return SystemError(Result::BindError);

        m_socket = socketDescriptor;
        return Ok(Result::OK);
    }

    Response<Result> Stop()
    {
        if (m_socket == SOCKET_NOT_SET) return Error(Result::NotStarted);

        int returnCode = close(m_socket);
        if (returnCode != 0) return SystemError(Result::CloseError);

        m_socket = SOCKET_NOT_SET;
        return Ok(Result::OK);
    }

    #pragma endregion StartStop

    // ------------------------------

    #pragma region SendReceive

    Response<Result> Send(const char* data, size_t size, const struct sockaddr * clientAddr)
    {
        if (m_socket == SOCKET_NOT_SET) return Error(Result::NotStarted);
        if (data == nullptr) return Error(Result::ParameterInvalid);
        if (size > strlen(data)) return Error(Result::ParameterInvalid);

        const socklen_t CLIENT_ADDR_SIZE = sizeof(struct sockaddr_in);
        const int USE_NO_FLAGS = 0;

        int returnCode = sendto(m_socket, data, size, USE_NO_FLAGS, clientAddr, CLIENT_ADDR_SIZE);
        if (returnCode < 0) return SystemError(Result::SendToError);

        return Ok(Result::OK);
    }

    Response<Result> Receive(char* buffer, size_t size, struct sockaddr * clientAddr)
    {
        if (m_socket == SOCKET_NOT_SET) return Error(Result::NotStarted);
        if (buffer == nullptr) return Error(Result::ParameterInvalid);

        socklen_t clientAddrSize = sizeof(struct sockaddr_in);
        const int USE_NO_FLAGS = 0;

        int receiveBytes = recvfrom(m_socket, buffer, size, USE_NO_FLAGS, clientAddr, &clientAddrSize);
        if (receiveBytes < 0) return SystemError(Result::SendToError);

        return Ok(Result::OK, receiveBytes);
    }

    #pragma endregion SendReceive

    // ------------------------------

};
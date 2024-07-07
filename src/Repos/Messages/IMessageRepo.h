#include "../../Models/Response.h"

#pragma once

class IMessageRepo
{
public:
    virtual ~IMessageRepo() = default;

    virtual Response<int> SendMessage(const char * , int size) = 0;
    virtual Response<int> GetMessage(char * message, int bufferSize) = 0;
};
#include <cerrno>

#pragma once

template<typename T>
struct Response
{
    T Code;
    int ErrorCode;
    bool Success;
    int Payload;
};


template<typename T>
Response<T> Ok(T code)
{
    Response<T> res = {code, 0, true, 0};
    return res;
}

template<typename T>
Response<T> Ok(T code, int payload)
{
    Response<T> res = {code, 0, true, payload};
    return res;
}

template<typename T>
Response<T> Error(T code)
{
    Response<T> res = {code, 0, false, 0};
    return res;
}

template<typename T>
Response<T> SystemError(T code)
{
    Response<T> res = {code, errno, false, 0};
    return res;
}
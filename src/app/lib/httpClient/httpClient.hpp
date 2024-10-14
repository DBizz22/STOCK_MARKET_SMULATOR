#ifndef HTTPCLIENT_HPP_
#define HTTPCLIENT_HPP_
#include <iostream>
#include <string>

class HttpClient
{
public:
    virtual bool sendRequest(const std::string &) = 0;
    virtual const std::string &getResponse() const = 0;
    virtual ~HttpClient() = default;
};

#endif
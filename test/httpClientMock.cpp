#pragma once
#include <gmock/gmock.h>
#include "httpClient.hpp"

class HttpClientMock : public HttpClient
{
public:
    MOCK_METHOD(bool, sendRequest, (const std::string &request));
    MOCK_METHOD((const std::string &), getResponse, (), (const));
};

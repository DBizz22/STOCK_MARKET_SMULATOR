#pragma once
#include <gmock/gmock.h>
#include "timeApi.hpp"

class UTCTimeApiMock : public TimeApi
{
public:
    MOCK_METHOD(bool, sendRequest, ());
    MOCK_METHOD(std::string, getResponse, (), (const));
    MOCK_METHOD(DateTime, getTime, ());
};
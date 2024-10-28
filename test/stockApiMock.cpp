#pragma once
#include <gmock/gmock.h>
#include "stockApi.hpp"

class StockApiMock : public stockApi::ApiBase
{
public:
    MOCK_METHOD(const std::string &, apiKey, (), (const));
    MOCK_METHOD(std::string &, apiKey, ());
    MOCK_METHOD(const std::string &, getRequest, (), (const));
    MOCK_METHOD(std::string &, getRequest, ());
    MOCK_METHOD(bool, sendRequest, ());
    MOCK_METHOD(const std::string &, getResponse, (), (const));
    MOCK_METHOD(void, setRequest, (const std::string &symbol));
    MOCK_METHOD(void, setRequest, ((const std::string &fromCurrency), (const std::string &toCurrency)));
    MOCK_METHOD(stockApi::Stock, getStock, ());
    MOCK_METHOD(stockApi::Forex, getForex, ());
    MOCK_METHOD(stockApi::ForexHistory, getForexHistory, ());
    MOCK_METHOD(stockApi::CryptoHistory, getCryptoHistory, ());
};
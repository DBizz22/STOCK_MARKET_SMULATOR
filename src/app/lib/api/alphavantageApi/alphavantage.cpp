#include "alphavantage.hpp"
#include <stdexcept>
#define SERIES 12

bool isErrorMessage(const boost::json::value &json)
{
    auto ptr = json.as_object().find("Error Message");
    if (ptr != json.as_object().end())
        return true;
    return false;
}

class BadPriceResponse : public std::logic_error
{
public:
    explicit BadPriceResponse(const std::string &msg) : logic_error(msg)
    {
    }
};

double ConvertJsonStringToDouble(const std::string &str)
{
    double price = 0;
    char *doublePtr = nullptr;
    price = std::strtod(str.c_str(), &doublePtr);
    if (doublePtr == str)
        throw(BadPriceResponse("Bad Price Response: No digits were found: " + str));
    else if (*doublePtr != '\0')
        throw(BadPriceResponse("Bad Price Response: Invalid input: " + str));
    return price;
}

stockApi::alphavantageApiSuite::SuiteStockData::SuiteStockData()
{
}

void stockApi::alphavantageApiSuite::SuiteStockData::reset()
{
    data.reset();
}

bool stockApi::alphavantageApiSuite::SuiteStockData::isEmpty() const
{
    return data.isEmpty();
}

bool stockApi::alphavantageApiSuite::operator==(const stockApi::alphavantageApiSuite::SuiteStockData &lhs, const stockApi::alphavantageApiSuite::SuiteStockData &rhs)
{
    return lhs.data == rhs.data;
}

bool stockApi::alphavantageApiSuite::operator!=(const stockApi::alphavantageApiSuite::SuiteStockData &lhs, const stockApi::alphavantageApiSuite::SuiteStockData &rhs)
{
    return lhs.data != rhs.data;
}

stockApi::alphavantageApiSuite::SuiteStockData stockApi::alphavantageApiSuite::tag_invoke(const boost::json::value_to_tag<SuiteStockData> &, const boost::json::value &json)
{
    if (isErrorMessage(json))
        throw std::runtime_error("Bad Request");
    stockApi::alphavantageApiSuite::SuiteStockData stock;
    stock.data.header = json.as_object().begin()->key();
    auto ptr = json.as_object().begin()->value();
    stock.data.timeZone = "UTC";
    stock.data.timestamp = ptr.as_object()["07. latest trading day"].as_string().c_str();
    stock.data.symbol = ptr.as_object()["01. symbol"].as_string().c_str();
    std::string price = ptr.as_object()["05. price"].as_string().c_str();
    stock.data.price = ConvertJsonStringToDouble(price);
    return stock;
}

stockApi::alphavantageApiSuite::SuiteForexData::SuiteForexData()
{
}

void stockApi::alphavantageApiSuite::SuiteForexData::reset()
{
    data.reset();
}

bool stockApi::alphavantageApiSuite::SuiteForexData::isEmpty() const
{
    return data.header.empty() && data.timeZone.empty() && data.timestamp.empty() && data.fromCurrency.empty() && data.toCurrency.empty() && data.price == 0;
}

bool stockApi::alphavantageApiSuite::operator==(const stockApi::alphavantageApiSuite::SuiteForexData &lhs, const stockApi::alphavantageApiSuite::SuiteForexData &rhs)
{
    return lhs.data == rhs.data;
}

bool stockApi::alphavantageApiSuite::operator!=(const stockApi::alphavantageApiSuite::SuiteForexData &lhs, const stockApi::alphavantageApiSuite::SuiteForexData &rhs)
{
    return lhs.data != rhs.data;
}

stockApi::alphavantageApiSuite::SuiteForexData stockApi::alphavantageApiSuite::tag_invoke(const boost::json::value_to_tag<SuiteForexData> &, const boost::json::value &json)
{
    if (isErrorMessage(json))
        throw std::runtime_error("Bad Request");

    stockApi::alphavantageApiSuite::SuiteForexData forex;
    /*forex.data.header = json.as_object().begin()->key();
    auto header = json.as_object().find("Realtime Currency Exchange Rate");
    auto ptr = header->value();*/
    forex.data.header = json.as_object().begin()->key();
    auto ptr = json.as_object().begin()->value();

    forex.data.timeZone = ptr.as_object()["7. Time Zone"].as_string().c_str();
    forex.data.timestamp = ptr.as_object()["6. Last Refreshed"].as_string().c_str();
    forex.data.fromCurrency = ptr.as_object()["1. From_Currency Code"].as_string().c_str();
    forex.data.toCurrency = ptr.as_object()["3. To_Currency Code"].as_string().c_str();
    std::string price = ptr.as_object()["5. Exchange Rate"].as_string().c_str();
    forex.data.price = ConvertJsonStringToDouble(price);
    return forex;
}

stockApi::alphavantageApiSuite::SuiteForexHistoryData::SuiteForexHistoryData()
{
}

void stockApi::alphavantageApiSuite::SuiteForexHistoryData::reset()
{
    data.reset();
}

bool stockApi::alphavantageApiSuite::SuiteForexHistoryData::isEmpty() const
{
    return data.isEmpty();
}

bool stockApi::alphavantageApiSuite::operator==(const stockApi::alphavantageApiSuite::SuiteForexHistoryData &lhs, const stockApi::alphavantageApiSuite::SuiteForexHistoryData &rhs)
{
    return lhs.data == rhs.data;
}

bool stockApi::alphavantageApiSuite::operator!=(const stockApi::alphavantageApiSuite::SuiteForexHistoryData &lhs, const stockApi::alphavantageApiSuite::SuiteForexHistoryData &rhs)
{
    return lhs.data != rhs.data;
}

stockApi::alphavantageApiSuite::SuiteForexHistoryData stockApi::alphavantageApiSuite::tag_invoke(const boost::json::value_to_tag<SuiteForexHistoryData> &, const boost::json::value &json)
{
    stockApi::alphavantageApiSuite::SuiteForexHistoryData forexHistory;
    boost::json::object metaData = json.as_object().find("Meta Data")->value().as_object();
    forexHistory.data.header = metaData["1. Information"].as_string().c_str();
    forexHistory.data.timeZone = metaData["5. Time Zone"].as_string().c_str();
    forexHistory.data.timestamp = metaData["4. Last Refreshed"].as_string().c_str();
    forexHistory.data.fromCurrency = metaData["2. From Symbol"].as_string().c_str();
    forexHistory.data.toCurrency = metaData["3. To Symbol"].as_string().c_str();

    auto timeSeries = (std::next(json.as_object().begin(), 1))->value();

    for (int i = 0; i < SERIES; i++)
    {
        boost::json::object::iterator dates = (std::next(timeSeries.as_object().begin(), i));
        forexHistory.data.datePrices[dates->key()] = ConvertJsonStringToDouble(dates->value().as_object()["4. close"].as_string().c_str());
    }
    return forexHistory;
}

bool stockApi::alphavantageApiSuite::operator==(const stockApi::alphavantageApiSuite::SuiteCryptoHistoryData &lhs, const stockApi::alphavantageApiSuite::SuiteCryptoHistoryData &rhs)
{
    return lhs.data == rhs.data;
}

bool stockApi::alphavantageApiSuite::operator!=(const stockApi::alphavantageApiSuite::SuiteCryptoHistoryData &lhs, const stockApi::alphavantageApiSuite::SuiteCryptoHistoryData &rhs)
{
    return lhs.data != rhs.data;
}

stockApi::alphavantageApiSuite::SuiteCryptoHistoryData::SuiteCryptoHistoryData()
{
}

void stockApi::alphavantageApiSuite::SuiteCryptoHistoryData::reset()
{
    data.reset();
}

bool stockApi::alphavantageApiSuite::SuiteCryptoHistoryData::isEmpty() const
{
    return data.isEmpty();
}

stockApi::alphavantageApiSuite::SuiteCryptoHistoryData stockApi::alphavantageApiSuite::tag_invoke(const boost::json::value_to_tag<SuiteCryptoHistoryData> &, const boost::json::value &json)
{
    stockApi::alphavantageApiSuite::SuiteCryptoHistoryData cryptoHistory;
    boost::json::object metaData = json.as_object().find("Meta Data")->value().as_object();
    cryptoHistory.data.header = metaData["1. Information"].as_string().c_str();
    cryptoHistory.data.timeZone = metaData["7. Time Zone"].as_string().c_str();
    cryptoHistory.data.timestamp = metaData["6. Last Refreshed"].as_string().c_str();
    cryptoHistory.data.symbol = metaData["2. Digital Currency Code"].as_string().c_str();
    cryptoHistory.data.market = metaData["4. Market Code"].as_string().c_str();

    auto timeSeries = (std::next(json.as_object().begin(), 1))->value();

    for (int i = 0; i < SERIES; i++)
    {
        boost::json::object::iterator dates = (std::next(timeSeries.as_object().begin(), i));
        cryptoHistory.data.datePrices[dates->key()] = ConvertJsonStringToDouble(dates->value().as_object()["4. close"].as_string().c_str());
    }
    return cryptoHistory;
}
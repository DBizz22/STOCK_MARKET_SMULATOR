#include "alphavantage.hpp"
#include <stdexcept>
#define SERIES 12

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

stockApi::alphavantageApiSuite::StockData::StockData()
{
    price = 0;
}

void stockApi::alphavantageApiSuite::StockData::reset()
{
    header.clear();
    timeZone.clear();
    timestamp.clear();
    symbol.clear();
    price = 0;
}

bool stockApi::alphavantageApiSuite::StockData::isEmpty() const
{
    return header.empty() && timestamp.empty() && timeZone.empty() && symbol.empty() && price == 0;
}

bool stockApi::alphavantageApiSuite::operator==(const stockApi::alphavantageApiSuite::StockData &lhs, const stockApi::alphavantageApiSuite::StockData &rhs)
{
    return (lhs.header == rhs.header &&
            lhs.timeZone == rhs.timeZone &&
            lhs.timestamp == rhs.timestamp &&
            lhs.symbol == rhs.symbol &&
            lhs.price == rhs.price);
}

bool stockApi::alphavantageApiSuite::operator!=(const stockApi::alphavantageApiSuite::StockData &lhs, const stockApi::alphavantageApiSuite::StockData &rhs)
{
    return !(lhs.header == rhs.header &&
             lhs.timeZone == rhs.timeZone &&
             lhs.timestamp == rhs.timestamp &&
             lhs.symbol == rhs.symbol &&
             lhs.price == rhs.price);
}

stockApi::alphavantageApiSuite::StockData stockApi::alphavantageApiSuite::tag_invoke(const boost::json::value_to_tag<StockData> &, const boost::json::value &json)
{
    stockApi::alphavantageApiSuite::StockData data;
    data.header = json.as_object().begin()->key();
    auto ptr = json.as_object().begin()->value();
    data.timeZone = "UTC";
    data.timestamp = ptr.as_object()["07. latest trading day"].as_string().c_str();
    data.symbol = ptr.as_object()["01. symbol"].as_string().c_str();
    std::string price = ptr.as_object()["05. price"].as_string().c_str();
    data.price = ConvertJsonStringToDouble(price);
    return data;
}

stockApi::alphavantageApiSuite::ForexData::ForexData()
{
    price = 0;
}

void stockApi::alphavantageApiSuite::ForexData::reset()
{
    header.clear();
    timeZone.clear();
    timestamp.clear();
    fromCurrency.clear();
    toCurrency.clear();
    price = 0;
}

bool stockApi::alphavantageApiSuite::ForexData::isEmpty() const
{
    return header.empty() && timeZone.empty() && timestamp.empty() && fromCurrency.empty() && toCurrency.empty() && price == 0;
}

bool stockApi::alphavantageApiSuite::operator==(const stockApi::alphavantageApiSuite::ForexData &lhs, const stockApi::alphavantageApiSuite::ForexData &rhs)
{
    return (lhs.header == rhs.header &&
            lhs.timeZone == rhs.timeZone &&
            lhs.timestamp == rhs.timestamp &&
            lhs.fromCurrency == rhs.fromCurrency &&
            lhs.toCurrency == rhs.toCurrency &&
            lhs.price == rhs.price);
}

bool stockApi::alphavantageApiSuite::operator!=(const stockApi::alphavantageApiSuite::ForexData &lhs, const stockApi::alphavantageApiSuite::ForexData &rhs)
{
    return !(lhs.header == rhs.header &&
             lhs.timeZone == rhs.timeZone &&
             lhs.timestamp == rhs.timestamp &&
             lhs.fromCurrency == rhs.fromCurrency &&
             lhs.toCurrency == rhs.toCurrency &&
             lhs.price == rhs.price);
}

stockApi::alphavantageApiSuite::ForexData stockApi::alphavantageApiSuite::tag_invoke(const boost::json::value_to_tag<ForexData> &, const boost::json::value &json)
{
    stockApi::alphavantageApiSuite::ForexData data;
    data.header = json.as_object().begin()->key();
    auto header = json.as_object().find("Realtime Currency Exchange Rate");
    auto ptr = header->value();
    data.timeZone = ptr.as_object()["7. Time Zone"].as_string().c_str();
    data.timestamp = ptr.as_object()["6. Last Refreshed"].as_string().c_str();
    data.fromCurrency = ptr.as_object()["1. From_Currency Code"].as_string().c_str();
    data.toCurrency = ptr.as_object()["3. To_Currency Code"].as_string().c_str();
    std::string price = ptr.as_object()["5. Exchange Rate"].as_string().c_str();
    data.price = ConvertJsonStringToDouble(price);
    return data;
}

stockApi::alphavantageApiSuite::ForexHistoryData::ForexHistoryData()
{
}

void stockApi::alphavantageApiSuite::ForexHistoryData::reset()
{
    header.clear();
    timeZone.clear();
    timestamp.clear();
    fromCurrency.clear();
    toCurrency.clear();
    datePrices.clear();
}

bool stockApi::alphavantageApiSuite::ForexHistoryData::isEmpty() const
{
    return header.empty() && timeZone.empty() && timestamp.empty() && fromCurrency.empty() && toCurrency.empty() && datePrices.empty();
}

bool stockApi::alphavantageApiSuite::operator==(const stockApi::alphavantageApiSuite::ForexHistoryData &lhs, const stockApi::alphavantageApiSuite::ForexHistoryData &rhs)
{
    return (lhs.header == rhs.header &&
            lhs.timeZone == rhs.timeZone &&
            lhs.timestamp == rhs.timestamp &&
            lhs.fromCurrency == rhs.fromCurrency &&
            lhs.toCurrency == rhs.toCurrency &&
            lhs.datePrices == rhs.datePrices);
}

bool stockApi::alphavantageApiSuite::operator!=(const stockApi::alphavantageApiSuite::ForexHistoryData &lhs, const stockApi::alphavantageApiSuite::ForexHistoryData &rhs)
{
    return !(lhs.header == rhs.header &&
             lhs.timeZone == rhs.timeZone &&
             lhs.timestamp == rhs.timestamp &&
             lhs.fromCurrency == rhs.fromCurrency &&
             lhs.toCurrency == rhs.toCurrency &&
             lhs.datePrices == rhs.datePrices);
}

stockApi::alphavantageApiSuite::ForexHistoryData stockApi::alphavantageApiSuite::tag_invoke(const boost::json::value_to_tag<ForexHistoryData> &, const boost::json::value &json)
{
    stockApi::alphavantageApiSuite::ForexHistoryData data;
    boost::json::object metaData = json.as_object().find("Meta Data")->value().as_object();
    data.header = metaData["1. Information"].as_string().c_str();
    data.timeZone = metaData["5. Time Zone"].as_string().c_str();
    data.timestamp = metaData["4. Last Refreshed"].as_string().c_str();
    data.fromCurrency = metaData["2. From Symbol"].as_string().c_str();
    data.toCurrency = metaData["3. To Symbol"].as_string().c_str();

    auto timeSeries = (std::next(json.as_object().begin(), 1))->value();

    for (int i = 0; i < SERIES; i++)
    {
        boost::json::object::iterator dates = (std::next(timeSeries.as_object().begin(), i));
        data.datePrices[dates->key()] = ConvertJsonStringToDouble(dates->value().as_object()["4. close"].as_string().c_str());
    }
    return data;
}

bool stockApi::alphavantageApiSuite::operator==(const stockApi::alphavantageApiSuite::CryptoHistoryData &lhs, const stockApi::alphavantageApiSuite::CryptoHistoryData &rhs)
{
    return (lhs.header == rhs.header &&
            lhs.timeZone == rhs.timeZone &&
            lhs.timestamp == rhs.timestamp &&
            lhs.symbol == rhs.symbol &&
            lhs.market == rhs.market &&
            lhs.datePrices == rhs.datePrices);
}

bool stockApi::alphavantageApiSuite::operator!=(const stockApi::alphavantageApiSuite::CryptoHistoryData &lhs, const stockApi::alphavantageApiSuite::CryptoHistoryData &rhs)
{
    return !(lhs.header == rhs.header &&
             lhs.timeZone == rhs.timeZone &&
             lhs.timestamp == rhs.timestamp &&
             lhs.symbol == rhs.symbol &&
             lhs.market == rhs.market &&
             lhs.datePrices == rhs.datePrices);
}

stockApi::alphavantageApiSuite::CryptoHistoryData::CryptoHistoryData()
{
}

void stockApi::alphavantageApiSuite::CryptoHistoryData::reset()
{
    header.clear();
    timeZone.clear();
    timestamp.clear();
    symbol.clear();
    market.clear();
    datePrices.clear();
}

bool stockApi::alphavantageApiSuite::CryptoHistoryData::isEmpty() const
{
    return header.empty() && timeZone.empty() && timestamp.empty() && symbol.empty() && market.empty() && datePrices.empty();
}

stockApi::alphavantageApiSuite::CryptoHistoryData stockApi::alphavantageApiSuite::tag_invoke(const boost::json::value_to_tag<CryptoHistoryData> &, const boost::json::value &json)
{
    stockApi::alphavantageApiSuite::CryptoHistoryData data;
    boost::json::object metaData = json.as_object().find("Meta Data")->value().as_object();
    data.header = metaData["1. Information"].as_string().c_str();
    data.timeZone = metaData["7. Time Zone"].as_string().c_str();
    data.timestamp = metaData["6. Last Refreshed"].as_string().c_str();
    data.symbol = metaData["2. Digital Currency Code"].as_string().c_str();
    data.market = metaData["4. Market Code"].as_string().c_str();

    auto timeSeries = (std::next(json.as_object().begin(), 1))->value();

    for (int i = 0; i < SERIES; i++)
    {
        boost::json::object::iterator dates = (std::next(timeSeries.as_object().begin(), i));
        data.datePrices[dates->key()] = ConvertJsonStringToDouble(dates->value().as_object()["4. close"].as_string().c_str());
    }
    return data;
}
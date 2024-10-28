#include <iostream>
#include <string>
#include <typeinfo>
#include "gtest/gtest.h"
#include "httpClientMock.cpp"
#include "alphavantage.hpp"

using stockApi::alphavantageApiSuite::SuiteCryptoHistoryData;
using stockApi::alphavantageApiSuite::SuiteForexData;
using stockApi::alphavantageApiSuite::SuiteForexHistoryData;
using stockApi::alphavantageApiSuite::SuiteStockData;

template <typename T>
class AlphavantageApiTest : public testing::Test
{
public:
    static std::shared_ptr<HttpClientMock> mock;
    static std::string urlRequest;
    static std::string urlResponse;
    static T alphavantageData;
    stockApi::alphavantageApiSuite::alphavantageApi<T> *sample;

    AlphavantageApiTest()
    {
    }
    ~AlphavantageApiTest()
    {
    }
    static void SetUpTestSuite()
    {
    }
    static void TearDownTestSuite()
    {
    }
    void SetUp() override
    {
        sample = new stockApi::alphavantageApiSuite::alphavantageApi<T>("demo", mock);
    }
    void TearDown() override
    {
        delete sample;
    }
};

template <>
void AlphavantageApiTest<SuiteStockData>::SetUpTestSuite()
{
    urlRequest = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=IBM&apikey=demo";
    urlResponse = "{\n"
                  "\"Global Quote\": {\n"
                  "\"01. symbol\": \"IBM\",\n"
                  "\"02. open\": \"222.1100\",\n"
                  "\"03. high\": \"224.0000\",\n"
                  "\"04. low\": \"221.3550\",\n"
                  "\"05. price\" : \"223.4300\",\n "
                  "\"06. volume\" : \"2628932\",\n"
                  "\"07. latest trading day\" : \"2024-09-26\",\n"
                  "\"08. previous close\" : \"221.2300\",\n"
                  "\"09. change\" : \"2.2000\",\n"
                  "\"10. change percent\" : \"0.9944%\"\n"
                  "}\n"
                  "\n}";
    alphavantageData.data.header = "Global Quote";
    alphavantageData.data.timeZone = "UTC";
    alphavantageData.data.timestamp = "2024-09-26";
    alphavantageData.data.symbol = "IBM";
    alphavantageData.data.price = 223.43;
    mock = std::make_shared<HttpClientMock>();
}

template <>
void AlphavantageApiTest<SuiteForexData>::SetUpTestSuite()
{
    urlRequest = "https://www.alphavantage.co/query?function=CURRENCY_EXCHANGE_RATE&from_currency=USD&to_currency=JPY&apikey=demo";
    urlResponse = "{\n    \"Realtime Currency Exchange Rate\": "
                  "{\n        \"1. From_Currency Code\": \"USD\",\n        "
                  "\"2. From_Currency Name\": \"United States Dollar\",\n        "
                  "\"3. To_Currency Code\": \"JPY\",\n        "
                  "\"4. To_Currency Name\": \"Japanese Yen\",\n        "
                  "\"5. Exchange Rate\": \"143.44300000\",\n        "
                  "\"6. Last Refreshed\": \"2024-09-24 18:10:33\",\n        "
                  "\"7. Time Zone\": \"UTC\",\n        "
                  "\"8. Bid Price\": \"143.44040000\",\n        "
                  "\"9. Ask Price\": \"143.44400000\"\n    }\n}";
    alphavantageData.data.fromCurrency = "USD";
    alphavantageData.data.toCurrency = "JPY";
    alphavantageData.data.header = "Realtime Currency Exchange Rate";
    alphavantageData.data.timeZone = "UTC";
    alphavantageData.data.timestamp = "2024-09-24 18:10:33";
    alphavantageData.data.price = 143.44300000;
    mock = std::make_shared<HttpClientMock>();
}

template <>
void AlphavantageApiTest<SuiteForexHistoryData>::SetUpTestSuite()
{
    urlRequest = "https://www.alphavantage.co/query?function=FX_WEEKLY&from_symbol=EUR&to_symbol=USD&apikey=demo";
    urlResponse = "{"
                  "\"Meta Data\": {"
                  "\"1. Information\" : \"Forex Weekly Prices (open, high, low, close)\","
                  "\"2. From Symbol\" : \"EUR\","
                  "\"3. To Symbol\" : \"USD\","
                  "\"4. Last Refreshed\" : \"2024-09-13 21:05:00\","
                  "\"5. Time Zone\" : \"UTC\""
                  "},"
                  "\"Time Series FX (Weekly)\" : {"
                  "\"2024-09-13\" : {"
                  "\"1. open\" : \"1.10375\","
                  "\"2. high\" : \"1.11025\","
                  "\"3. low\" : \"1.10028\","
                  "\"4. close\" : \"1.10737\""
                  "},"
                  "\"2024-09-08\" : {"
                  "\"1. open\" : \"1.10675\","
                  "\"2. high\" : \"1.11496\","
                  "\"3. low\" : \"1.10337\","
                  "\"4. close\" : \"1.10872\""
                  "},"
                  "\"2024-09-01\" : {"
                  "\"1. open\" : \"1.11647\","
                  "\"2. high\" : \"1.11819\","
                  "\"3. low\" : \"1.10425\","
                  "\"4. close\" : \"1.10448\""
                  "},"
                  "\"2024-08-25\" : {"
                  "\"1. open\" : \"1.10865\","
                  "\"2. high\" : \"1.12011\","
                  "\"3. low\" : \"1.10725\","
                  "\"4. close\" : \"1.11879\""
                  "},"
                  "\"2024-08-18\" : {"
                  "\"1. open\" : \"1.09363\","
                  "\"2. high\" : \"1.10726\","
                  "\"3. low\" : \"1.09157\","
                  "\"4. close\" : \"1.10250\""
                  "},"
                  "\"2024-08-11\" : {"
                  "\"1. open\" : \"1.09510\","
                  "\"2. high\" : \"1.09631\","
                  "\"3. low\" : \"1.08823\","
                  "\"4. close\" : \"1.09138\""
                  "},"
                  "\"2024-08-04\" : {"
                  "\"1. open\" : \"1.08195\","
                  "\"2. high\" : \"1.10074\","
                  "\"3. low\" : \"1.07783\","
                  "\"4. close\" : \"1.09108\""
                  "},"
                  "\"2024-07-28\" : {"
                  "\"1. open\" : \"1.08898\","
                  "\"2. high\" : \"1.09004\","
                  "\"3. low\" : \"1.08031\","
                  "\"4. close\" : \"1.08538\""
                  "},"
                  "\"2024-07-21\" : {"
                  "\"1. open\" : \"1.09016\","
                  "\"2. high\" : \"1.09472\","
                  "\"3. low\" : \"1.08722\","
                  "\"4. close\" : \"1.08992\""
                  "},"
                  "\"2024-07-14\" : {"
                  "\"1. open\" : \"1.08309\","
                  "\"2. high\" : \"1.09219\","
                  "\"3. low\" : \"1.08062\","
                  "\"4. close\" : \"1.08855\""
                  "},"
                  "\"2024-07-07\" : {"
                  "\"1. open\" : \"1.07371\","
                  "\"2. high\" : \"1.08472\","
                  "\"3. low\" : \"1.07106\","
                  "\"4. close\" : \"1.08260\""
                  "},"
                  "\"2024-06-30\" : {"
                  "\"1. open\" : \"1.07327\","
                  "\"2. high\" : \"1.07782\","
                  "\"3. low\" : \"1.06677\","
                  "\"4. close\" : \"1.07353\""
                  " }}}";
    alphavantageData.data.header = "Forex Weekly Prices (open, high, low, close)";
    alphavantageData.data.timeZone = "UTC";
    alphavantageData.data.timestamp = "2024-09-13 21:05:00";
    alphavantageData.data.fromCurrency = "EUR";
    alphavantageData.data.toCurrency = "USD";
    alphavantageData.data.datePrices["2024-09-13"] = 1.10737;
    alphavantageData.data.datePrices["2024-09-08"] = 1.10872;
    alphavantageData.data.datePrices["2024-09-01"] = 1.10448;
    alphavantageData.data.datePrices["2024-08-25"] = 1.11879;
    alphavantageData.data.datePrices["2024-08-18"] = 1.1025;
    alphavantageData.data.datePrices["2024-08-11"] = 1.09138;
    alphavantageData.data.datePrices["2024-08-04"] = 1.09108;
    alphavantageData.data.datePrices["2024-07-28"] = 1.08538;
    alphavantageData.data.datePrices["2024-07-21"] = 1.08992;
    alphavantageData.data.datePrices["2024-07-14"] = 1.08855;
    alphavantageData.data.datePrices["2024-07-07"] = 1.08260;
    alphavantageData.data.datePrices["2024-06-30"] = 1.07353;
    mock = std::make_shared<HttpClientMock>();
}

template <>
void AlphavantageApiTest<SuiteCryptoHistoryData>::SetUpTestSuite()
{
    urlRequest = "https://www.alphavantage.co/query?function=DIGITAL_CURRENCY_WEEKLY&symbol=BTC&market=EUR&apikey=demo";
    urlResponse = "{\n    \"Meta Data\": "
                  "{\n        \"1. Information\": \"Weekly Prices and Volumes for Digital Currency\",\n"
                  "\"2. Digital Currency Code\": \"BTC\",\n"
                  "\"3. Digital Currency Name\": \"Bitcoin\",\n"
                  "\"4. Market Code\": \"EUR\",\n"
                  "\"5. Market Name\": \"Euro\",\n"
                  "\"6. Last Refreshed\": \"2024-09-27 00:00:00\",\n"
                  "\"7. Time Zone\": \"UTC\"\n    },\n"
                  "\"Time Series (Digital Currency Weekly)\": {\n"
                  "\"2024-09-27\": {\n"
                  "\"1. open\": \"56974.20000000\",\n"
                  "\"2. high\": \"58940.47000000\",\n"
                  "\"3. low\": \"56080.15000000\",\n"
                  "\"4. close\": \"58299.26000000\",\n"
                  "\"5. volume\": \"937.53824419\"\n"
                  "},\n"
                  "\"2024-09-22\": {\n"
                  "\"1. open\": \"53320.98000000\",\n"
                  "\"2. high\": \"57414.44000000\",\n"
                  "\"3. low\": \"51650.17000000\",\n"
                  "\"4. close\": \"56988.75000000\",\n"
                  "\"5. volume\": \"1655.20071181\"\n"
                  "},\n"
                  "\"2024-09-15\": {\n"
                  "\"1. open\": \"49526.79000000\",\n"
                  "\"2. high\": \"54781.85000000\",\n"
                  "\"3. low\": \"49315.32000000\",\n"
                  "\"4. close\": \"53343.95000000\",\n"
                  "\"5. volume\": \"1707.00085554\"\n"
                  "},\n"
                  "\"2024-09-08\": {\n"
                  "\"1. open\": \"51911.95000000\",\n"
                  "\"2. high\": \"54065.45000000\",\n"
                  "\"3. low\": \"47428.66000000\",\n"
                  "\"4. close\": \"49505.02000000\",\n"
                  "\"5. volume\": \"2198.29055177\"\n"
                  "},\n"
                  "\"2024-09-01\": {\n"
                  "\"1. open\": \"57406.50000000\",\n"
                  "\"2. high\": \"57632.29000000\",\n"
                  "\"3. low\": \"51802.75000000\",\n"
                  "\"4. close\": \"51931.36000000\",\n"
                  "\"5. volume\": \"1877.16287658\"\n"
                  "},\n"
                  "\"2024-08-25\": {\n"
                  "\"1. open\": \"53049.34000000\",\n"
                  "\"2. high\": \"58113.94000000\",\n"
                  "\"3. low\": \"52408.27000000\",\n"
                  "\"4. close\": \"57402.19000000\",\n"
                  "\"5. volume\": \"2010.86924833\"\n"
                  "},\n"
                  "\"2024-08-18\": {\n"
                  "\"1. open\": \"53822.14000000\",\n"
                  "\"2. high\": \"56141.89000000\",\n"
                  "\"3. low\": \"51195.03000000\",\n"
                  "\"4. close\": \"53046.89000000\",\n"
                  "\"5. volume\": \"1836.96086527\"\n"
                  "},\n"
                  "\"2024-08-11\": {\n"
                  "\"1. open\": \"53330.01000000\",\n"
                  "\"2. high\": \"57499.00000000\",\n"
                  "\"3. low\": \"45021.36000000\",\n"
                  "\"4. close\": \"53835.66000000\",\n"
                  "\"5. volume\": \"5859.41666575\"\n"
                  "},\n"
                  "\"2024-08-04\": {\n"
                  "\"1. open\": \"62866.50000000\",\n"
                  "\"2. high\": \"64749.00000000\",\n"
                  "\"3. low\": \"52521.27000000\",\n"
                  "\"4. close\": \"53309.69000000\",\n"
                  "\"5. volume\": \"3522.97605496\"\n"
                  "},\n"
                  "\"2024-07-28\": {\n"
                  "\"1. open\": \"62526.97000000\",\n"
                  "\"2. high\": \"63876.72000000\",\n"
                  "\"3. low\": \"58512.00000000\",\n"
                  "\"4. close\": \"62854.02000000\",\n"
                  "\"5. volume\": \"1840.76222947\"\n"
                  "},\n"
                  "\"2024-07-21\": {\n"
                  "\"1. open\": \"55876.94000000\",\n"
                  "\"2. high\": \"62721.17000000\",\n"
                  "\"3. low\": \"55750.00000000\",\n"
                  "\"4. close\": \"62529.01000000\",\n"
                  "\"5. volume\": \"2216.33637267\"\n"
                  "},\n"
                  "\"2024-07-14\": {\n"
                  "\"1. open\": \"51589.15000000\",\n"
                  "\"2. high\": \"56400.63000000\",\n"
                  "\"3. low\": \"50144.21000000\",\n"
                  "\"4. close\": \"55876.86000000\",\n"
                  "\"5. volume\": \"5468.18414888\"\n"
                  "}\n}\n}";
    alphavantageData.data.header = "Weekly Prices and Volumes for Digital Currency";
    alphavantageData.data.timeZone = "UTC";
    alphavantageData.data.timestamp = "2024-09-27 00:00:00";
    alphavantageData.data.symbol = "BTC";
    alphavantageData.data.market = "EUR";
    alphavantageData.data.datePrices["2024-09-27"] = 58299.26000000;
    alphavantageData.data.datePrices["2024-09-22"] = 56988.75000000;
    alphavantageData.data.datePrices["2024-09-15"] = 53343.95000000;
    alphavantageData.data.datePrices["2024-09-08"] = 49505.02000000;
    alphavantageData.data.datePrices["2024-09-01"] = 51931.36000000;
    alphavantageData.data.datePrices["2024-08-25"] = 57402.19000000;
    alphavantageData.data.datePrices["2024-08-18"] = 53046.89000000;
    alphavantageData.data.datePrices["2024-08-11"] = 53835.66000000;
    alphavantageData.data.datePrices["2024-08-04"] = 53309.69000000;
    alphavantageData.data.datePrices["2024-07-28"] = 62854.02000000;
    alphavantageData.data.datePrices["2024-07-21"] = 62529.01;
    alphavantageData.data.datePrices["2024-07-14"] = 55876.86;
    mock = std::make_shared<HttpClientMock>();
}

template <typename T>
std::string AlphavantageApiTest<T>::urlRequest;

template <typename T>
std::string AlphavantageApiTest<T>::urlResponse;

template <typename T>
std::shared_ptr<HttpClientMock> AlphavantageApiTest<T>::mock = nullptr;

template <typename T>
T AlphavantageApiTest<T>::alphavantageData;

using MyTypes = ::testing::Types<SuiteStockData, SuiteForexData, SuiteForexHistoryData, SuiteCryptoHistoryData>;
TYPED_TEST_SUITE(AlphavantageApiTest, MyTypes);

template <typename T>
void SetTypeRequest(AlphavantageApiTest<T> *testPtr)
{
    if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteStockData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteStockData> *>(testPtr->sample))
        ptr->setRequest("IBM");
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexData> *>(testPtr->sample))
        ptr->setRequest("USD", "JPY");
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexHistoryData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexHistoryData> *>(testPtr->sample))
        ptr->setRequest("EUR", "USD", stockApi::alphavantageApiSuite::TimeSeries::weekly);
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteCryptoHistoryData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteCryptoHistoryData> *>(testPtr->sample))
        ptr->setRequest("BTC", "EUR", stockApi::alphavantageApiSuite::TimeSeries::weekly);
}

template <typename T>
std::string SetRequestResponse(AlphavantageApiTest<T> *testPtr)
{
    if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteStockData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteStockData> *>(testPtr->sample))
    {
        ptr->setRequest("ETH");
        return "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=ETH&apikey=demo";
    }
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexData> *>(testPtr->sample))
    {
        ptr->setRequest("JPY", "USD");
        return "https://www.alphavantage.co/query?function=CURRENCY_EXCHANGE_RATE&from_currency=JPY&to_currency=USD&apikey=demo";
    }
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexHistoryData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexHistoryData> *>(testPtr->sample))
    {
        ptr->setRequest("CNY", "NGN", stockApi::alphavantageApiSuite::TimeSeries::weekly);
        return "https://www.alphavantage.co/query?function=FX_WEEKLY&from_symbol=CNY&to_symbol=NGN&apikey=demo";
    }
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteCryptoHistoryData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteCryptoHistoryData> *>(testPtr->sample))
    {
        ptr->setRequest("DOGE", "AUD", stockApi::alphavantageApiSuite::TimeSeries::weekly);
        return "https://www.alphavantage.co/query?function=DIGITAL_CURRENCY_WEEKLY&symbol=DOGE&market=AUD&apikey=demo";
    }
    else
        return "";
}

template <typename T>
void NonUnderlyingDataCheck(AlphavantageApiTest<T> *testPtr)
{
    if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteStockData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteStockData> *>(testPtr->sample))
    {
        stockApi::Stock stock = ptr->getStock();
        EXPECT_TRUE(stock.isEmpty());
    }
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexData> *>(testPtr->sample))
    {
        stockApi::Forex forex = ptr->getForex();
        EXPECT_TRUE(forex.isEmpty());
    }
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexHistoryData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexHistoryData> *>(testPtr->sample))
    {
        stockApi::ForexHistory forexHistory = ptr->getForexHistory();
        EXPECT_TRUE(forexHistory.isEmpty());
    }
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteCryptoHistoryData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteCryptoHistoryData> *>(testPtr->sample))
    {
        stockApi::CryptoHistory cryptoHistory = ptr->getCryptoHistory();
        EXPECT_TRUE(cryptoHistory.isEmpty());
    }
}

template <typename T>
void UnderlyingDataCheck(AlphavantageApiTest<T> *testPtr)
{
    if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteStockData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteStockData> *>(testPtr->sample))
    {
        stockApi::Stock stock = ptr->getStock();
        EXPECT_FALSE(stock.isEmpty());
    }
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexData> *>(testPtr->sample))
    {
        stockApi::Forex forex = ptr->getForex();
        EXPECT_FALSE(forex.isEmpty());
    }
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexHistoryData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteForexHistoryData> *>(testPtr->sample))
    {
        stockApi::ForexHistory forexHistory = ptr->getForexHistory();
        EXPECT_FALSE(forexHistory.isEmpty());
    }
    else if (stockApi::alphavantageApiSuite::alphavantageApi<SuiteCryptoHistoryData> *ptr = dynamic_cast<stockApi::alphavantageApiSuite::alphavantageApi<SuiteCryptoHistoryData> *>(testPtr->sample))
    {
        stockApi::CryptoHistory cryptoHistory = ptr->getCryptoHistory();
        EXPECT_FALSE(cryptoHistory.isEmpty());
    }
}

TYPED_TEST(AlphavantageApiTest, SetApiKey)
{
    this->sample->apiKey() = "Testing";
    EXPECT_EQ(this->sample->apiKey(), "Testing");
}

TYPED_TEST(AlphavantageApiTest, ResetApiKey)
{
    this->sample->apiKey() = "Testing1";
    EXPECT_EQ(this->sample->apiKey(), "Testing1");
    this->sample->apiKey() = "Testing2";
    EXPECT_EQ(this->sample->apiKey(), "Testing2");
}

TYPED_TEST(AlphavantageApiTest, SetRequestString)
{
    SetTypeRequest(this);
    EXPECT_EQ(this->sample->getRequest(), this->urlRequest);
}

TYPED_TEST(AlphavantageApiTest, ResetRequestString)
{
    std::string request = SetRequestResponse(this);
    EXPECT_EQ(this->sample->getRequest(), request);
    SetTypeRequest(this);
    EXPECT_EQ(this->sample->getRequest(), this->urlRequest);
}

TYPED_TEST(AlphavantageApiTest, SendRequestFailure)
{
    EXPECT_CALL(*this->mock, sendRequest("")).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(this->sample->sendRequest());
}

TYPED_TEST(AlphavantageApiTest, SendRequestSuccess)
{
    EXPECT_CALL(*this->mock, sendRequest(this->urlRequest)).Times(1).WillOnce(testing::Return(true));
    SetTypeRequest(this);
    EXPECT_TRUE(this->sample->sendRequest());
}

TYPED_TEST(AlphavantageApiTest, SendRequestFailureThenSuccess)
{
    EXPECT_CALL(*this->mock, sendRequest("")).Times(1).WillOnce(testing::Return(false));
    EXPECT_CALL(*this->mock, sendRequest(this->urlRequest)).Times(1).WillOnce(testing::Return(true));
    EXPECT_FALSE(this->sample->sendRequest());
    SetTypeRequest(this);
    EXPECT_TRUE(this->sample->sendRequest());
}

TYPED_TEST(AlphavantageApiTest, GetResponse)
{
    const std::string response1("Hello");
    const std::string response2("Hello, world!");
    testing::InSequence seq;
    EXPECT_CALL(*this->mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(response1));
    EXPECT_CALL(*this->mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(response2));
    EXPECT_EQ(this->sample->getResponse(), "Hello");
    EXPECT_EQ(this->sample->getResponse(), "Hello, world!");
}

TYPED_TEST(AlphavantageApiTest, GetDataAfterEmptyResponse)
{
    std::string response;
    EXPECT_CALL(*this->mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(response));
    EXPECT_TRUE(this->sample->getData().isEmpty());
}

TYPED_TEST(AlphavantageApiTest, GetDataAfterBadResponse)
{
    std::string response = "Hello";
    EXPECT_CALL(*this->mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(response));
    EXPECT_TRUE(this->sample->getData().isEmpty());
}

TYPED_TEST(AlphavantageApiTest, GetDataAfterGoodResponse)
{
    EXPECT_CALL(*this->mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(this->urlResponse));
    EXPECT_FALSE(this->sample->getData().isEmpty());
    EXPECT_EQ(this->sample->getData(), this->alphavantageData);
}

TYPED_TEST(AlphavantageApiTest, GetDataAfterGoodBadResponse)
{
    std::string response = "Hello";
    testing::InSequence seq;
    EXPECT_CALL(*this->mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(this->urlResponse));
    EXPECT_CALL(*this->mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(response));
    EXPECT_EQ(this->sample->getData(), this->alphavantageData);
    this->sample->sendRequest();
    EXPECT_TRUE(this->sample->getData().isEmpty());
}

TYPED_TEST(AlphavantageApiTest, GetNonUnderlyingDataAfterBadResponse)
{
    std::string response = "Hello";
    EXPECT_CALL(*this->mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(response));
    EXPECT_TRUE(this->sample->getData().isEmpty());
    NonUnderlyingDataCheck(this);
}

TYPED_TEST(AlphavantageApiTest, GetUnderlyingDataAfterGoodResponse)
{
    EXPECT_CALL(*this->mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(this->urlResponse));
    EXPECT_FALSE(this->sample->getData().isEmpty());
    EXPECT_EQ(this->sample->getData(), this->alphavantageData);
    UnderlyingDataCheck(this);
}

TYPED_TEST(AlphavantageApiTest, GetNonUnderlyingDataAfterGoodBadResponse)
{
    std::string response = "Hello";
    testing::InSequence seq;
    EXPECT_CALL(*this->mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(this->urlResponse));
    EXPECT_CALL(*this->mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(response));
    EXPECT_EQ(this->sample->getData(), this->alphavantageData);
    this->sample->sendRequest();
    NonUnderlyingDataCheck(this);
}
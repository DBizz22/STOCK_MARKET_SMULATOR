#include "gtest/gtest.h"
#include "searchModel.hpp"
#include "databaseClientMock.cpp"
#include "httpClientMock.cpp"
#include "timeApiMock.cpp"
#include "stockApiMock.cpp"
#include "algorithm"

class SearchModelTest : public testing::Test
{
protected:
    static const std::string symbol;
    static const std::string currency;
    static const DateTime dateTime;
    static const database::StockRecord stockRecord, forexStockRecord;
    static const stockApi::Stock stock;
    static const stockApi::Forex forex;
    std::shared_ptr<DatabaseClientMock> databaseClientMock;
    std::shared_ptr<UTCTimeApiMock> timeApiMock;
    std::shared_ptr<StockApiMock> stockApiMock;
    ApiManager apiManager;
    std::shared_ptr<SearchModel> searchModel;

public:
    SearchModelTest() {}
    ~SearchModelTest() {}
    static void SetUpTestSuite()
    {
    }
    static void TearDownTestSuite()
    {
    }
    void SetUp() override
    {
        databaseClientMock = std::make_shared<DatabaseClientMock>();
        timeApiMock = std::make_shared<UTCTimeApiMock>();
        stockApiMock = std::make_shared<StockApiMock>();
        apiManager = {timeApiMock, stockApiMock, stockApiMock};
        searchModel = std::make_shared<SearchModel>(databaseClientMock, apiManager);
    }
    void TearDown() override
    {
    }
};

const std::string SearchModelTest::symbol("Symbol");
const std::string SearchModelTest::currency("USD");
const DateTime SearchModelTest::dateTime{"2024-10-23", "00:00:00"};
const database::StockRecord SearchModelTest::stockRecord{1, symbol, currency, 100, "2024-10-22"};
const database::StockRecord SearchModelTest::forexStockRecord{1, "USD", "NGN", 1600, "2024-10-22"};
const stockApi::Stock SearchModelTest::stock{"Header", "UTC", "2024-10-23", symbol, 200};
const stockApi::Forex SearchModelTest::forex{"Header", "UTC", "2024-10-23", "USD", "NGN", 1600};

TEST_F(SearchModelTest, FindStockCanNotGetCurrentTimeFailure)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(2).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(2).WillRepeatedly(testing::Return(DateTime{}));
    EXPECT_TRUE(searchModel->findStock(symbol, currency).isEmpty());
    EXPECT_TRUE(searchModel->findForex(forex.fromCurrency, forex.toCurrency).isEmpty());
}

TEST_F(SearchModelTest, FindForexInvalidCurrencyFailure)
{
    std::string invalidCurrency("invalid_Currency");
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(2).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(2).WillRepeatedly(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(invalidCurrency, currency)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*databaseClientMock, getStock(currency, invalidCurrency)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*stockApiMock, setRequest(invalidCurrency, currency)).Times(1);
    EXPECT_CALL(*stockApiMock, setRequest(currency, invalidCurrency)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(2).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getForex()).Times(2).WillRepeatedly(testing::Return(stockApi::Forex{}));
    EXPECT_TRUE(searchModel->findForex(invalidCurrency, currency).isEmpty());
    EXPECT_TRUE(searchModel->findForex(currency, invalidCurrency).isEmpty());
}

TEST_F(SearchModelTest, FindForexUpToDateInDatabase)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(DateTime{stockRecord.lastUpdate, "00:00:00"}));
    EXPECT_CALL(*databaseClientMock, getStock(forex.fromCurrency, forex.toCurrency)).Times(1).WillOnce(testing::Return(forexStockRecord));
    SearchData searchedData = searchModel->findStock(forex.fromCurrency, forex.toCurrency);
    EXPECT_FALSE(searchedData.isEmpty());
    EXPECT_EQ(searchedData.symbol, forexStockRecord.symbol);
    EXPECT_EQ(searchedData.currency, forexStockRecord.currency);
}

TEST_F(SearchModelTest, FindForexForexAPISendRequestFailure)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(forex.fromCurrency, forex.toCurrency)).Times(1).WillOnce(testing::Return(forexStockRecord));
    EXPECT_CALL(*stockApiMock, setRequest(forex.fromCurrency, forex.toCurrency)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(false));
    EXPECT_TRUE(searchModel->findForex(forex.fromCurrency, forex.toCurrency).isEmpty());
}

TEST_F(SearchModelTest, FindForexAPIGetForexFailure)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(forex.fromCurrency, forex.toCurrency)).Times(1).WillOnce(testing::Return(forexStockRecord));
    EXPECT_CALL(*stockApiMock, setRequest(forex.fromCurrency, forex.toCurrency)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getForex()).Times(1).WillOnce(testing::Return(stockApi::Forex{}));
    EXPECT_TRUE(searchModel->findForex(forex.fromCurrency, forex.toCurrency).isEmpty());
}

TEST_F(SearchModelTest, FindForexAPIGetDigitalCurrencyToNonUSDFailure)
{
    // testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(3).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(3).WillRepeatedly(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(testing::Eq("BTC"), testing::Eq("NGN"))).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*databaseClientMock, getStock(testing::Eq("BTC"), testing::Eq("USD"))).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*databaseClientMock, getStock(testing::Eq("USD"), testing::Eq("NGN"))).Times(1).WillOnce(testing::Return(database::StockRecord{}));

    EXPECT_CALL(*stockApiMock, setRequest(testing::Eq("BTC"), testing::Eq("NGN"))).Times(1);
    EXPECT_CALL(*stockApiMock, setRequest(testing::Eq("BTC"), testing::Eq("USD"))).Times(1);
    EXPECT_CALL(*stockApiMock, setRequest(testing::Eq("USD"), testing::Eq("NGN"))).Times(1);

    EXPECT_CALL(*stockApiMock, sendRequest()).Times(3).WillRepeatedly(testing::Return(true));

    EXPECT_CALL(*stockApiMock, getForex()).Times(3).WillRepeatedly(testing::Return(stockApi::Forex{}));
    EXPECT_TRUE(searchModel->findForex("BTC", "NGN").isEmpty());
}

TEST_F(SearchModelTest, FindDigitalCurrencyToNonUSDSuccess)
{
    database::StockRecord forexBTCtoUSD{1, "BTC", "USD", 50000, dateTime.date};
    database::StockRecord forexUSDtoNGN{2, "USD", "NGN", 1600, dateTime.date};
    SearchData expectedData{"BTC", forexBTCtoUSD.currentPrice * forexUSDtoNGN.currentPrice, "NGN"};
    // testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(3).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(3).WillRepeatedly(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(testing::Eq("BTC"), testing::Eq("NGN"))).Times(2).WillRepeatedly(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*databaseClientMock, getStock(testing::Eq("BTC"), testing::Eq("USD"))).Times(1).WillOnce(testing::Return(forexBTCtoUSD));
    EXPECT_CALL(*databaseClientMock, getStock(testing::Eq("USD"), testing::Eq("NGN"))).Times(1).WillOnce(testing::Return(forexUSDtoNGN));

    EXPECT_CALL(*stockApiMock, setRequest(testing::Eq("BTC"), testing::Eq("NGN"))).Times(1);

    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));

    EXPECT_CALL(*stockApiMock, getForex()).Times(1).WillOnce(testing::Return(stockApi::Forex{}));
    EXPECT_CALL(*databaseClientMock, insert(database::StockRecord{0, "BTC", "NGN", forexBTCtoUSD.currentPrice * forexUSDtoNGN.currentPrice, dateTime.date})).Times(1).WillOnce(testing::Return(true));
    SearchData searchedData = searchModel->findForex("BTC", "NGN");
    EXPECT_FALSE(searchedData.isEmpty());
    EXPECT_EQ(searchedData.symbol, expectedData.symbol);
    EXPECT_EQ(searchedData.currentPrice, expectedData.currentPrice);
    EXPECT_EQ(searchedData.currency, expectedData.currency);
}

TEST_F(SearchModelTest, FindForexUpdateDatabaseInsertFailure)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(forex.fromCurrency, forex.toCurrency)).Times(2).WillRepeatedly(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*stockApiMock, setRequest(forex.fromCurrency, forex.toCurrency)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getForex()).Times(1).WillOnce(testing::Return(forex));
    EXPECT_CALL(*databaseClientMock, insert(database::StockRecord{0, forex.fromCurrency, forex.toCurrency, forex.price, forex.timestamp})).Times(1).WillOnce(testing::Return(false));
    EXPECT_TRUE(searchModel->findForex(forex.fromCurrency, forex.toCurrency).isEmpty());
}

TEST_F(SearchModelTest, FindForexUpdateDatabaseInsertSuccess)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(forex.fromCurrency, forex.toCurrency)).Times(2).WillRepeatedly(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*stockApiMock, setRequest(forex.fromCurrency, forex.toCurrency)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getForex()).Times(1).WillOnce(testing::Return(forex));
    EXPECT_CALL(*databaseClientMock, insert(database::StockRecord{0, forex.fromCurrency, forex.toCurrency, forex.price, forex.timestamp})).Times(1).WillOnce(testing::Return(true));
    SearchData searchedData = searchModel->findForex(forex.fromCurrency, forex.toCurrency);
    EXPECT_FALSE(searchedData.isEmpty());
    EXPECT_EQ(searchedData.symbol, forex.fromCurrency);
    EXPECT_EQ(searchedData.currency, forex.toCurrency);
}

TEST_F(SearchModelTest, FindForexUpdateDatabaseUpdateFailure)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(forex.fromCurrency, forex.toCurrency)).Times(2).WillRepeatedly(testing::Return(forexStockRecord));
    EXPECT_CALL(*stockApiMock, setRequest(forex.fromCurrency, forex.toCurrency)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getForex()).Times(1).WillOnce(testing::Return(forex));
    EXPECT_CALL(*databaseClientMock, update(database::StockRecord{forexStockRecord.ID, forexStockRecord.symbol, forexStockRecord.currency, forex.price, dateTime.date})).Times(1).WillOnce(testing::Return(false));
    EXPECT_TRUE(searchModel->findForex(forex.fromCurrency, forex.toCurrency).isEmpty());
}

TEST_F(SearchModelTest, FindForexUpdateDatabaseUpdateSuccess)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(forex.fromCurrency, forex.toCurrency)).Times(2).WillRepeatedly(testing::Return(forexStockRecord));
    EXPECT_CALL(*stockApiMock, setRequest(forex.fromCurrency, forex.toCurrency)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getForex()).Times(1).WillOnce(testing::Return(forex));
    EXPECT_CALL(*databaseClientMock, update(database::StockRecord{forexStockRecord.ID, forexStockRecord.symbol, forexStockRecord.currency, forex.price, dateTime.date})).Times(1).WillOnce(testing::Return(true));
    SearchData searchedData = searchModel->findForex(forex.fromCurrency, forex.toCurrency);
    EXPECT_FALSE(searchedData.isEmpty());
    EXPECT_EQ(searchedData.symbol, forex.fromCurrency);
    EXPECT_EQ(searchedData.currency, forex.toCurrency);
}

TEST_F(SearchModelTest, FindStockInvalidSymbolFailure)
{
    std::string invalidSymbol("invalid_Symbol");
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(invalidSymbol, currency)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*stockApiMock, setRequest(invalidSymbol)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getStock()).Times(1).WillOnce(testing::Return(stockApi::Stock{}));
    EXPECT_TRUE(searchModel->findStock(invalidSymbol, currency).isEmpty());
}

TEST_F(SearchModelTest, FindStockInvalidCurrencyFailure)
{
    std::string invalidCurrency = "invalidCurrency";
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, invalidCurrency)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*stockApiMock, setRequest(symbol)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getStock()).Times(1).WillOnce(testing::Return(stockApi::Stock{}));
    EXPECT_TRUE(searchModel->findStock(symbol, invalidCurrency).isEmpty());
}

TEST_F(SearchModelTest, FindStockUpToDateInDatabase)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(DateTime{stockRecord.lastUpdate, "00:00:00"}));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, currency)).Times(1).WillOnce(testing::Return(stockRecord));
    SearchData searchedData = searchModel->findStock(symbol, currency);
    EXPECT_FALSE(searchedData.isEmpty());
    EXPECT_EQ(searchedData.symbol, symbol);
    EXPECT_EQ(searchedData.currency, currency);
}

TEST_F(SearchModelTest, FindStockOnlineStockAPISendRequestFailure)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, currency)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*stockApiMock, setRequest(symbol)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(false));
    EXPECT_TRUE(searchModel->findStock(symbol, currency).isEmpty());
}

TEST_F(SearchModelTest, FindStockAPIGetStockFailure)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, currency)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*stockApiMock, setRequest(symbol)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getStock()).Times(1).WillOnce(testing::Return(stockApi::Stock{}));
    EXPECT_TRUE(searchModel->findStock(symbol, currency).isEmpty());
}

TEST_F(SearchModelTest, FindStockOnlineUpdateDatabaseInsertFailure)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, currency)).Times(2).WillRepeatedly(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*stockApiMock, setRequest(symbol)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getStock()).Times(1).WillOnce(testing::Return(stock));
    EXPECT_CALL(*databaseClientMock, insert(database::StockRecord{0, stock.symbol, currency, stock.price, dateTime.date})).Times(1).WillOnce(testing::Return(false));
    EXPECT_TRUE(searchModel->findStock(symbol, currency).isEmpty());
}

TEST_F(SearchModelTest, FindStockOnlineUpdateDatabaseInsertSuccess)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, currency)).Times(2).WillRepeatedly(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*stockApiMock, setRequest(symbol)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getStock()).Times(1).WillOnce(testing::Return(stock));
    EXPECT_CALL(*databaseClientMock, insert(database::StockRecord{0, stock.symbol, currency, stock.price, dateTime.date})).Times(1).WillOnce(testing::Return(true));
    SearchData searchedData = searchModel->findStock(symbol, currency);
    EXPECT_FALSE(searchedData.isEmpty());
    EXPECT_EQ(searchedData.symbol, symbol);
    EXPECT_EQ(searchedData.currency, currency);
    EXPECT_EQ(searchedData.currentPrice, stock.price);
}

TEST_F(SearchModelTest, FindStockOnlineUpdateDatabaseUpdateFailure)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, currency)).Times(2).WillRepeatedly(testing::Return(stockRecord));
    EXPECT_CALL(*stockApiMock, setRequest(symbol)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getStock()).Times(1).WillOnce(testing::Return(stock));
    EXPECT_CALL(*databaseClientMock, update(database::StockRecord{stockRecord.ID, stockRecord.symbol, stockRecord.currency, stock.price, dateTime.date})).Times(1).WillOnce(testing::Return(false));
    EXPECT_TRUE(searchModel->findStock(symbol, currency).isEmpty());
}

TEST_F(SearchModelTest, FindStockOnlineUpdateDatabaseUpdateSuccess)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(1).WillOnce(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, currency)).Times(2).WillRepeatedly(testing::Return(stockRecord));
    EXPECT_CALL(*stockApiMock, setRequest(symbol)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getStock()).Times(1).WillOnce(testing::Return(stock));
    EXPECT_CALL(*databaseClientMock, update(database::StockRecord{stockRecord.ID, stockRecord.symbol, stockRecord.currency, stock.price, dateTime.date})).Times(1).WillOnce(testing::Return(true));
    SearchData searchedData = searchModel->findStock(symbol, currency);
    EXPECT_FALSE(searchedData.isEmpty());
    EXPECT_EQ(searchedData.symbol, symbol);
    EXPECT_EQ(searchedData.currency, currency);
    EXPECT_EQ(searchedData.currentPrice, stock.price);
}

TEST_F(SearchModelTest, FindStockOnlineNonUSDCurrencyFailure)
{
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(2).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(2).WillRepeatedly(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, "NGN")).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*stockApiMock, setRequest(symbol)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getStock()).Times(1).WillOnce(testing::Return(stock));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, "USD")).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*databaseClientMock, insert(database::StockRecord{0, stockRecord.symbol, stockRecord.currency, stock.price, dateTime.date})).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getStock("USD", "NGN")).Times(1).WillOnce(testing::Return(database::StockRecord{0, "USD", "NGN", 0, dateTime.date}));
    EXPECT_TRUE(searchModel->findStock(symbol, "NGN").isEmpty());
}

TEST_F(SearchModelTest, FindStockOnlineNonUSDCurrencySuccess)
{
    std::string otherCurrency = "NGN";
    testing::Sequence seq;
    EXPECT_CALL(*timeApiMock, sendRequest()).Times(2).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*timeApiMock, getTime()).Times(2).WillRepeatedly(testing::Return(dateTime));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, otherCurrency)).Times(2).WillRepeatedly(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*stockApiMock, setRequest(symbol)).Times(1);
    EXPECT_CALL(*stockApiMock, sendRequest()).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*stockApiMock, getStock()).Times(1).WillOnce(testing::Return(stock));
    EXPECT_CALL(*databaseClientMock, getStock(symbol, "USD")).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_CALL(*databaseClientMock, insert(database::StockRecord{0, stockRecord.symbol, stockRecord.currency, stock.price, dateTime.date})).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getStock("USD", otherCurrency)).Times(1).WillOnce(testing::Return(database::StockRecord{0, "USD", otherCurrency, 1600, dateTime.date}));
    EXPECT_CALL(*databaseClientMock, insert(database::StockRecord{0, stockRecord.symbol, otherCurrency, stock.price * 1600, dateTime.date})).Times(1).WillOnce(testing::Return(true));
    SearchData searchedData = searchModel->findStock(symbol, otherCurrency);
    EXPECT_FALSE(searchedData.isEmpty());
    EXPECT_EQ(searchedData.symbol, symbol);
    EXPECT_EQ(searchedData.currency, otherCurrency);
    EXPECT_EQ(searchedData.currentPrice, stock.price * 1600);
}

#include "searchModel.hpp"

const int SecondsInOneDay = 86400;

void SearchData::convert(const database::StockRecord &stockRecord)
{
    symbol = stockRecord.symbol;
    currentPrice = stockRecord.currentPrice;
    currency = stockRecord.currency;
};

void SearchData::convert(const stockApi::Stock &stock)
{
    symbol = stock.symbol;
    currentPrice = stock.price;
    currency = "USD";
};

bool SearchData::isEmpty() const
{
    return symbol.empty() && currentPrice == 0;
}

void SearchData::convert(const stockApi::Forex &forex)
{
    symbol = forex.fromCurrency;
    currentPrice = forex.price;
    currency = forex.toCurrency;
}

bool SearchModel::getCurrentUTCDateTime()
{
    if (!apiManager_.UTCTimeApi->sendRequest())
        return false;
    currentDateTime_ = apiManager_.UTCTimeApi->getTime();
    return true;
}

bool SearchModel::isUpToDate(const database::StockRecord &stockRecord)
{
    DateTime lastUpdate = {stockRecord.lastUpdate, "00:00:00"};
    return (currentDateTime_ - lastUpdate < SecondsInOneDay);
}

bool SearchModel::updateDatabase(const SearchData &data)
{
    database::StockRecord stockRecord = databaseClient_->getStock(data.symbol, data.currency);
    if (stockRecord.isEmpty())
    {
        stockRecord = {0, data.symbol, data.currency, data.currentPrice, currentDateTime_.date};
        return databaseClient_->insert(stockRecord);
    }
    else
    {
        stockRecord.currentPrice = data.currentPrice;
        stockRecord.lastUpdate = currentDateTime_.date;
        return databaseClient_->update(stockRecord);
    }
}

SearchData SearchModel::apiGetForex(const std::string &fromCurrency, const std::string &toCurrency)
{
    // TODO: update test for additional currency conversion
    SearchData result;
    apiManager_.forexApi->setRequest(fromCurrency, toCurrency);
    if (!apiManager_.forexApi->sendRequest())
        return {};

    result.convert(apiManager_.forexApi->getForex());

    if (result.isEmpty() && toCurrency != "USD")
    {
        SearchData result2, result3;
        result2 = findForex(fromCurrency, "USD");
        result3 = findForex("USD", toCurrency);
        result.currentPrice = result2.currentPrice * result3.currentPrice;
        result.symbol = fromCurrency;
        result.currency = toCurrency;
    }

    if (result.currentPrice == 0 || !updateDatabase(result))
        return {};

    return result;
}

SearchData SearchModel::apiGetStock(const std::string &symbol, const std::string &currency)
{
    SearchData result;
    apiManager_.stockApi->setRequest(symbol);
    if (!apiManager_.stockApi->sendRequest())
        return {};

    result.convert(apiManager_.stockApi->getStock());
    if (result.isEmpty() || !updateDatabase(result))
        return {};

    if (result.currency == currency)
        return result;

    result.currentPrice = result.currentPrice * findForex(result.currency, currency).currentPrice;
    result.currency = currency;
    if (result.currentPrice == 0 || !updateDatabase(result))
        return {};

    return result;
}

SearchModel::SearchModel(const std::shared_ptr<database::DatabaseClient> &database, ApiManager &apiManager) : apiManager_(apiManager)
{
    databaseClient_ = database;
}

SearchData SearchModel::findForex(const std::string &fromCurrency, const std::string &toCurrency)
{
    if (fromCurrency.empty() || toCurrency.empty())
        return {};
    SearchData searchData;
    if (!getCurrentUTCDateTime())
        return {};
    database::StockRecord stockRecord = databaseClient_->getStock(fromCurrency, toCurrency);
    if (!stockRecord.isEmpty() && isUpToDate(stockRecord))
    {
        searchData.convert(stockRecord);
        return searchData;
    }

    return apiGetForex(fromCurrency, toCurrency);
}

SearchData SearchModel::findStock(const std::string &symbol, const std::string &currency)
{
    if (symbol.empty() || currency.empty())
        return {};
    SearchData searchData;
    if (!getCurrentUTCDateTime())
        return {};
    database::StockRecord stockRecord = databaseClient_->getStock(symbol, currency);
    if (!stockRecord.isEmpty() && isUpToDate(stockRecord))
    {
        searchData.convert(stockRecord);
        return searchData;
    }

    return apiGetStock(symbol, currency);
}
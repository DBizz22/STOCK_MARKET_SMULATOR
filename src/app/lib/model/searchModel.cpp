#include "searchModel.hpp"

const int SecondsInOneDay = 86400;
const std::vector<std::string> CurrencyCodes{
    "AED", "AFN", "ALL", "AMD", "ANG", "AOA", "ARS", "AUD", "AWG", "AZN",
    "BAM", "BBD", "BDT", "BGN", "BHD", "BIF", "BMD", "BND", "BOB", "BRL", "BSD", "BTN", "BWP", "BZD",
    "CAD", "CDF", "CHF", "CLF", "CLP", "CNH", "CNY", "COP", "CUP", "CVE", "CZK",
    "DJF", "DKK", "DOP", "DZD",
    "EGP", "ERN", "ETB", "EUR", "FJD", "FKP", "GBP", "GEL", "GHS", "GIP", "GMD", "GNF", "GTQ", "GYD",
    "HKD", "HNL", "HRK", "HTG", "HUF", "ICP", "IDR", "ILS", "INR", "IQD", "IRR", "ISK", "JEP", "JMD", "JOD", "JPY",
    "KES", "KGS", "KHR", "KMF", "KPW", "KRW", "KWD", "KYD", "KZT", "LAK", "LBP", "LKR", "LRD", "LSL", "LYD",
    "MAD", "MDL", "MGA", "MKD", "MMK", "MNT", "MOP", "MRO", "MRU", "MUR", "MVR", "MWK", "MXN", "MYR", "MZN",
    "NAD", "NGN", "NOK", "NPR", "NZD", "OMR", "PAB", "PEN", "PGK", "PHP", "PKR", "PLN", "PYG",
    "QAR", "RON", "RSD", "RUB", "RUR", "RWF", "SAR", "SBDF", "SCR", "SDG", "SDR", "SEK", "SGD", "SHP", "SLL", "SOS", "SRD", "SYP", "SZL",
    "THB", "TJS", "TMT", "TND", "TOP", "TRY", "TTD", "TWD", "TZS", "UAH", "UGX", "USD", "UYU", "UZS",
    "VND", "VUV", "WST", "XAF", "XCD", "XDR", "XOF", "XPF", "YER", "ZAR", "ZMW", "ZWL"};

bool isDigitalCurrency(const std::string &currency)
{
    auto iter = std::find(CurrencyCodes.begin(), CurrencyCodes.end(), currency);
    if (iter != CurrencyCodes.end())
        return false;
    return true;
}

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
    SearchData result;
    apiManager_.forexApi->setRequest(fromCurrency, toCurrency);
    if (!apiManager_.forexApi->sendRequest())
        return {};

    result.convert(apiManager_.forexApi->getForex());

    if (result.isEmpty() && isDigitalCurrency(fromCurrency) && toCurrency != "USD")
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
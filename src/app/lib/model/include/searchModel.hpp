#ifndef SEARCHMODEL_HPP_
#define SEARCHMODEL_HPP_

#include <string>
#include <memory>
#include "databaseClient.hpp"
#include "stockApi.hpp"
#include "timeApi.hpp"

struct ApiManager
{
    std::shared_ptr<TimeApi> UTCTimeApi;
    std::shared_ptr<stockApi::ApiBase> stockApi;
    std::shared_ptr<stockApi::ApiBase> forexApi;
};

struct SearchData
{
    std::string symbol;
    double currentPrice{0};
    std::string currency;
    void convert(const database::StockRecord &);
    void convert(const stockApi::Stock &);
    void convert(const stockApi::Forex &);
    bool isEmpty() const;
};

class SearchModel
{
private:
    std::shared_ptr<database::DatabaseClient> databaseClient_;
    ApiManager &apiManager_;
    DateTime currentDateTime_;
    bool getCurrentUTCDateTime();
    bool isUpToDate(const database::StockRecord &);
    SearchData apiGetStock(const std::string &symbol, const std::string &currency);
    SearchData apiGetForex(const std::string &fromCurrency, const std::string &toCurrency);
    bool updateDatabase(const SearchData &);

public:
    SearchModel(const std::shared_ptr<database::DatabaseClient> &database, ApiManager &apiManager);
    SearchData findStock(const std::string &symbol, const std::string &currency = "USD");
    SearchData findForex(const std::string &fromCurrency, const std::string &toCurrency);
};

#endif
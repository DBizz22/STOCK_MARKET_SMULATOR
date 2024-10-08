#ifndef ALPHAVANTAGE_HPP_
#define ALPHAVANTAGE_HPP_

#include <iostream>
#include <map>
#include "boost/json.hpp"
#include "stockApi.hpp"
#include "httpClient.hpp"
#define USE_TEMPLATE_ 1

namespace stockApi
{
    namespace alphavantageApiSuite
    {
        enum class TimeSeries
        {
            daily,
            weekly,
            monthly
        };

        struct StockData
        {
            std::string header;
            std::string timeZone;
            std::string timestamp;
            std::string symbol;
            double price;
            StockData();
            void reset();
            bool isEmpty() const;
            friend bool operator==(const StockData &lhs, const StockData &rhs);
            friend bool operator!=(const StockData &lhs, const StockData &rhs);
            friend StockData tag_invoke(const boost::json::value_to_tag<StockData> &, const boost::json::value &json);
        };

        struct ForexData
        {
            std::string header;
            std::string timeZone;
            std::string timestamp;
            std::string fromCurrency;
            std::string toCurrency;
            double price;
            ForexData();
            void reset();
            bool isEmpty() const;
            friend bool operator==(const ForexData &lhs, const ForexData &rhs);
            friend bool operator!=(const ForexData &lhs, const ForexData &rhs);
            friend ForexData tag_invoke(const boost::json::value_to_tag<ForexData> &, const boost::json::value &json);
        };

        struct ForexHistoryData
        {
            std::string header;
            std::string timeZone;
            std::string timestamp;
            std::string fromCurrency;
            std::string toCurrency;
            std::map<std::string, double> datePrices;
            ForexHistoryData();
            void reset();
            bool isEmpty() const;
            friend bool operator==(const ForexHistoryData &lhs, const ForexHistoryData &rhs);
            friend bool operator!=(const ForexHistoryData &lhs, const ForexHistoryData &rhs);
            friend ForexHistoryData tag_invoke(const boost::json::value_to_tag<ForexHistoryData> &, const boost::json::value &json);
        };

        struct CryptoHistoryData
        {
            std::string header;
            std::string timeZone;
            std::string timestamp;
            std::string symbol;
            std::string market;
            std::map<std::string, double> datePrices;
            CryptoHistoryData();
            void reset();
            bool isEmpty() const;
            friend bool operator==(const CryptoHistoryData &lhs, const CryptoHistoryData &rhs);
            friend bool operator!=(const CryptoHistoryData &lhs, const CryptoHistoryData &rhs);
            friend CryptoHistoryData tag_invoke(const boost::json::value_to_tag<CryptoHistoryData> &, const boost::json::value &json);
        };

#ifndef USE_TEMPLATE_
        class StockApi : public Api
        {
        private:
            StockData data_;
            bool dataReady_ = false;
            void processResponse();

        public:
            StockApi(const std::string &key, const std::shared_ptr<HttpClient> &client);
            void setRequest(const std::string &symbol);
            bool sendRequest() override;
            StockData getData();
        };

        class ForexApi : public Api
        {
        private:
            ForexData data_;
            bool dataReady_ = false;
            void processResponse();

        public:
            ForexApi(const std::string &key, const std::shared_ptr<HttpClient> &client);
            void setRequest(const std::string &fromCurrency, const std::string &toCurrency);
            bool sendRequest() override;
            ForexData getData();
        };

        class ForexHistoryApi : public Api
        {
        private:
            ForexHistoryData data_;
            bool dataReady_ = false;
            std::map<TimeSeries, std::string> scope_;
            void processResponse();

        public:
            ForexHistoryApi(const std::string &key, const std::shared_ptr<HttpClient> &client);
            void setRequest(const std::string &fromCurrency, const std::string &toCurrency, TimeSeries scope = TimeSeries::daily);
            bool sendRequest() override;
            ForexHistoryData getData();
        };

        class CryptoHistoryApi : public Api
        {
        private:
            CryptoHistoryData data_;
            bool dataReady_ = false;
            std::map<TimeSeries, std::string> scope_;
            void processResponse();

        public:
            CryptoHistoryApi(const std::string &key, const std::shared_ptr<HttpClient> &client);
            void setRequest(const std::string &fromCurrency, const std::string &toCurrency, TimeSeries scope = TimeSeries::daily);
            bool sendRequest() override;
            CryptoHistoryData getData();
        };

#endif

#ifdef USE_TEMPLATE_
        template <typename T>
        class alphavantageApi : public Api<T>
        {
        public:
            alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<T>(key, client)
            {
            }
        };

        template <>
        class alphavantageApi<StockData> : public Api<StockData>
        {
        public:
            alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<StockData>(key, client)
            {
            }
            void setRequest(const std::string &symbol)
            {
                Api::getRequest() = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol + "&apikey=" + Api::apiKey();
            }
        };

        template <>
        class alphavantageApi<ForexData> : public Api<ForexData>
        {
        public:
            alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<ForexData>(key, client)
            {
            }
            void setRequest(const std::string &fromCurrency, const std::string &toCurrency)
            {
                Api::getRequest() = "https://www.alphavantage.co/query?function=CURRENCY_EXCHANGE_RATE&from_currency=" + fromCurrency + "&to_currency=" + toCurrency + "&apikey=" + Api::apiKey();
            }
        };

        template <>
        class alphavantageApi<ForexHistoryData> : public Api<ForexHistoryData>
        {
        private:
            std::map<TimeSeries, std::string> scope_;

        public:
            alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<ForexHistoryData>(key, client)
            {
                scope_[TimeSeries::daily] = "FX_DAILY";
                scope_[TimeSeries::weekly] = "FX_WEEKLY";
                scope_[TimeSeries::monthly] = "FX_MONTHLY";
            }
            void setRequest(const std::string &fromCurrency, const std::string &toCurrency, TimeSeries scope = TimeSeries::daily)
            {
                Api::getRequest() = "https://www.alphavantage.co/query?function=" + scope_[scope] +
                                    "&from_symbol=" + fromCurrency + "&to_symbol=" + toCurrency + "&apikey=" + Api::apiKey();
            }
        };

        template <>
        class alphavantageApi<CryptoHistoryData> : public Api<CryptoHistoryData>
        {
        private:
            std::map<TimeSeries, std::string> scope_;

        public:
            alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<CryptoHistoryData>(key, client)
            {
                scope_[TimeSeries::daily] = "DIGITAL_CURRENCY_DAILY";
                scope_[TimeSeries::weekly] = "DIGITAL_CURRENCY_WEEKLY";
                scope_[TimeSeries::monthly] = "DIGITAL_CURRENCY_MONTHLY";
            }
            void setRequest(const std::string &symbol, const std::string &market, TimeSeries scope = TimeSeries::daily)
            {
                Api::getRequest() = "https://www.alphavantage.co/query?function=" + scope_[scope] +
                                    "&symbol=" + symbol + "&market=" + market + "&apikey=" + Api::apiKey();
            }
        };

        /*template <typename T>
stockApi::alphavantageApiSuite::alphavantageApi<T>::alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<T>(key, client)
{
}

        alphavantageApi<StockData>::alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<StockData>(key, client)
{
}

        void alphavantageApi<StockData>::setRequest(const std::string &symbol)
{
    Api::getRequest() = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=" + symbol + "&interval=5min&apikey=" + Api::apiKey();
}

        alphavantageApi<ForexData>::alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<ForexData>(key, client)
{
}

        void alphavantageApi<ForexData>::setRequest(const std::string &fromCurrency, const std::string &toCurrency)
{
    Api::getRequest() = "https://www.alphavantage.co/query?function=CURRENCY_EXCHANGE_RATE&from_currency=" + fromCurrency + "&to_currency=" + toCurrency + "&apikey=" + Api::apiKey();
}

        alphavantageApi<ForexHistoryData>::alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<ForexHistoryData>(key, client)
{
            scope_[TimeSeries::daily] = "FX_DAILY";
            scope_[TimeSeries::weekly] = "FX_WEEKLY";
            scope_[TimeSeries::monthly] = "FX_MONTHLY";
}

        void alphavantageApi<ForexHistoryData>::setRequest(const std::string &fromCurrency, const std::string &toCurrency, TimeSeries scope)
{
    Api::getRequest() = "https://www.alphavantage.co/query?function=" + scope_[scope] +
                        "&from_currency=" + fromCurrency + "&to_currency=" + toCurrency + "&apikey=" + Api::apiKey();
}

        alphavantageApi<CryptoHistoryData>::alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<CryptoHistoryData>(key, client)
{
            scope_[TimeSeries::daily] = "DIGITAL_CURRENCY_DAILY";
            scope_[TimeSeries::weekly] = "DIGITAL_CURRENCY_WEEKLY";
            scope_[TimeSeries::monthly] = "DIGITAL_CURRENCY_MONTHLY";
}

        void alphavantageApi<CryptoHistoryData>::setRequest(const std::string &fromCurrency, const std::string &toCurrency, TimeSeries scope)
{
    Api::getRequest() = "https://www.alphavantage.co/query?function=" + scope_[scope] +
                        "&from_currency=" + fromCurrency + "&to_currency=" + toCurrency + "&apikey=" + Api::apiKey();
        }*/

#endif
    } // alphavantageApiSuite
} // stockApi

#endif
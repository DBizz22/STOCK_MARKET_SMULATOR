#ifndef ALPHAVANTAGE_HPP_
#define ALPHAVANTAGE_HPP_

#include <iostream>
#include <map>
#include "boost/json.hpp"
#include "stockApi.hpp"
#include "httpClient.hpp"

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

        struct SuiteStockData
        {
            Stock data;
            SuiteStockData();
            void reset();
            bool isEmpty() const;
            friend bool operator==(const SuiteStockData &lhs, const SuiteStockData &rhs);
            friend bool operator!=(const SuiteStockData &lhs, const SuiteStockData &rhs);
            friend SuiteStockData tag_invoke(const boost::json::value_to_tag<SuiteStockData> &, const boost::json::value &json);
        };

        struct SuiteForexData
        {
            Forex data;
            SuiteForexData();
            void reset();
            bool isEmpty() const;
            friend bool operator==(const SuiteForexData &lhs, const SuiteForexData &rhs);
            friend bool operator!=(const SuiteForexData &lhs, const SuiteForexData &rhs);
            friend SuiteForexData tag_invoke(const boost::json::value_to_tag<SuiteForexData> &, const boost::json::value &json);
        };

        struct SuiteForexHistoryData
        {
            ForexHistory data;
            SuiteForexHistoryData();
            void reset();
            bool isEmpty() const;
            friend bool operator==(const SuiteForexHistoryData &lhs, const SuiteForexHistoryData &rhs);
            friend bool operator!=(const SuiteForexHistoryData &lhs, const SuiteForexHistoryData &rhs);
            friend SuiteForexHistoryData tag_invoke(const boost::json::value_to_tag<SuiteForexHistoryData> &, const boost::json::value &json);
        };

        struct SuiteCryptoHistoryData
        {
            CryptoHistory data;
            SuiteCryptoHistoryData();
            void reset();
            bool isEmpty() const;
            friend bool operator==(const SuiteCryptoHistoryData &lhs, const SuiteCryptoHistoryData &rhs);
            friend bool operator!=(const SuiteCryptoHistoryData &lhs, const SuiteCryptoHistoryData &rhs);
            friend SuiteCryptoHistoryData tag_invoke(const boost::json::value_to_tag<SuiteCryptoHistoryData> &, const boost::json::value &json);
        };

        template <typename T>
        class alphavantageApi : public Api<T>
        {
        public:
            alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<T>(key, client)
            {
            }
        };

        template <>
        class alphavantageApi<SuiteStockData> : public Api<SuiteStockData>
        {
        public:
            alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<SuiteStockData>(key, client)
            {
            }
            void setRequest(const std::string &symbol)
            {
                Api::getRequest() = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol + "&apikey=" + Api::apiKey();
            }
            Stock getStock()
            {
                return Api<SuiteStockData>::getData().data;
            }
        };

        template <>
        class alphavantageApi<SuiteForexData> : public Api<SuiteForexData>
        {
        public:
            alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<SuiteForexData>(key, client)
            {
            }
            void setRequest(const std::string &fromCurrency, const std::string &toCurrency)
            {
                Api::getRequest() = "https://www.alphavantage.co/query?function=CURRENCY_EXCHANGE_RATE&from_currency=" + fromCurrency + "&to_currency=" + toCurrency + "&apikey=" + Api::apiKey();
            }
            Forex getForex()
            {
                return Api<SuiteForexData>::getData().data;
            }
        };

        template <>
        class alphavantageApi<SuiteForexHistoryData> : public Api<SuiteForexHistoryData>
        {
        private:
            std::map<TimeSeries, std::string> scope_;

        public:
            alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<SuiteForexHistoryData>(key, client)
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
            ForexHistory getForexHistory()
            {
                return Api<SuiteForexHistoryData>::getData().data;
            }
        };

        template <>
        class alphavantageApi<SuiteCryptoHistoryData> : public Api<SuiteCryptoHistoryData>
        {
        private:
            std::map<TimeSeries, std::string> scope_;

        public:
            alphavantageApi(std::string_view key, const std::shared_ptr<HttpClient> &client) : Api<SuiteCryptoHistoryData>(key, client)
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
            CryptoHistory getCryptoHistory()
            {
                return Api<SuiteCryptoHistoryData>::getData().data;
            }
        };

    } // alphavantageApiSuite namespace section
} // stockApi namespace section

#endif
#ifndef STOCKAPI_HPP_
#define STOCKAPI_HPP_

#include <iostream>
#include <string>
#include <memory>
#include <map>
#include "boost/json.hpp"
#include "httpClient.hpp"

namespace stockApi
{
    struct Stock
    {
        std::string header;
        std::string timeZone;
        std::string timestamp;
        std::string symbol;
        double price{0};
        void reset()
        {
            header.clear();
            timeZone.clear();
            timestamp.clear();
            symbol.clear();
            price = 0;
        }
        bool isEmpty() const
        {
            return header.empty() && timestamp.empty() && timeZone.empty() && symbol.empty() && price == 0;
        }
        friend bool operator==(const Stock &lhs, const Stock &rhs)
        {
            return (lhs.header == rhs.header &&
                    lhs.timeZone == rhs.timeZone &&
                    lhs.timestamp == rhs.timestamp &&
                    lhs.symbol == rhs.symbol &&
                    lhs.price == rhs.price);
        }
        friend bool operator!=(const Stock &lhs, const Stock &rhs)
        {
            return !(lhs.header == rhs.header &&
                     lhs.timeZone == rhs.timeZone &&
                     lhs.timestamp == rhs.timestamp &&
                     lhs.symbol == rhs.symbol &&
                     lhs.price == rhs.price);
        }
    };

    struct Forex
    {
        std::string header;
        std::string timeZone;
        std::string timestamp;
        std::string fromCurrency;
        std::string toCurrency;
        double price{0};
        void reset()
        {
            header.clear();
            timeZone.clear();
            timestamp.clear();
            fromCurrency.clear();
            toCurrency.clear();
            price = 0;
        }
        bool isEmpty() const
        {
            return header.empty() && timeZone.empty() && timestamp.empty() && fromCurrency.empty() && toCurrency.empty() && price == 0;
        }
        friend bool operator==(const Forex &lhs, const Forex &rhs)
        {
            return (lhs.header == rhs.header &&
                    lhs.timeZone == rhs.timeZone &&
                    lhs.timestamp == rhs.timestamp &&
                    lhs.fromCurrency == rhs.fromCurrency &&
                    lhs.toCurrency == rhs.toCurrency &&
                    lhs.price == rhs.price);
        }
        friend bool operator!=(const Forex &lhs, const Forex &rhs)
        {
            return !(lhs.header == rhs.header &&
                     lhs.timeZone == rhs.timeZone &&
                     lhs.timestamp == rhs.timestamp &&
                     lhs.fromCurrency == rhs.fromCurrency &&
                     lhs.toCurrency == rhs.toCurrency &&
                     lhs.price == rhs.price);
        }
    };

    struct ForexHistory
    {
        std::string header;
        std::string timeZone;
        std::string timestamp;
        std::string fromCurrency;
        std::string toCurrency;
        std::map<std::string, double> datePrices;
        void reset()
        {
            header.clear();
            timeZone.clear();
            timestamp.clear();
            fromCurrency.clear();
            toCurrency.clear();
            datePrices.clear();
        }
        bool isEmpty() const
        {
            return header.empty() && timeZone.empty() && timestamp.empty() && fromCurrency.empty() && toCurrency.empty() && datePrices.empty();
        }
        friend bool operator==(const ForexHistory &lhs, const ForexHistory &rhs)
        {
            return (lhs.header == rhs.header &&
                    lhs.timeZone == rhs.timeZone &&
                    lhs.timestamp == rhs.timestamp &&
                    lhs.fromCurrency == rhs.fromCurrency &&
                    lhs.toCurrency == rhs.toCurrency &&
                    lhs.datePrices == rhs.datePrices);
        }
        friend bool operator!=(const ForexHistory &lhs, const ForexHistory &rhs)
        {
            return !(lhs.header == rhs.header &&
                     lhs.timeZone == rhs.timeZone &&
                     lhs.timestamp == rhs.timestamp &&
                     lhs.fromCurrency == rhs.fromCurrency &&
                     lhs.toCurrency == rhs.toCurrency &&
                     lhs.datePrices == rhs.datePrices);
        }
    };

    struct CryptoHistory
    {
        std::string header;
        std::string timeZone;
        std::string timestamp;
        std::string symbol;
        std::string market;
        std::map<std::string, double> datePrices;
        void reset()
        {
            header.clear();
            timeZone.clear();
            timestamp.clear();
            symbol.clear();
            market.clear();
            datePrices.clear();
        }
        bool isEmpty() const
        {
            return header.empty() && timeZone.empty() && timestamp.empty() && symbol.empty() && market.empty() && datePrices.empty();
        }
        friend bool operator==(const CryptoHistory &lhs, const CryptoHistory &rhs)
        {
            return (lhs.header == rhs.header &&
                    lhs.timeZone == rhs.timeZone &&
                    lhs.timestamp == rhs.timestamp &&
                    lhs.symbol == rhs.symbol &&
                    lhs.market == rhs.market &&
                    lhs.datePrices == rhs.datePrices);
        }
        friend bool operator!=(const CryptoHistory &lhs, const CryptoHistory &rhs)
        {
            return !(lhs.header == rhs.header &&
                     lhs.timeZone == rhs.timeZone &&
                     lhs.timestamp == rhs.timestamp &&
                     lhs.symbol == rhs.symbol &&
                     lhs.market == rhs.market &&
                     lhs.datePrices == rhs.datePrices);
        }
    };

    class ApiBase
    {
    public:
        virtual const std::string &apiKey() const = 0;
        virtual std::string &apiKey() = 0;
        virtual const std::string &getRequest() const = 0;
        virtual std::string &getRequest() = 0;
        virtual bool sendRequest() = 0;
        virtual const std::string &getResponse() const = 0;
        virtual void setRequest(const std::string &symbol)
        {
        }
        virtual void setRequest(const std::string &fromCurrency, const std::string &toCurrency)
        {
        }
        virtual Stock getStock()
        {
            return Stock();
        }
        virtual Forex getForex()
        {
            return Forex();
        }
        virtual ForexHistory getForexHistory()
        {
            return ForexHistory();
        }
        virtual CryptoHistory getCryptoHistory()
        {
            return CryptoHistory();
        }
        virtual ~ApiBase() {}
    };

    template <typename T>
    class Api : public ApiBase
    {
    private:
        bool dataReady_ = false;
        std::string apiKey_;
        std::string request_;
        T suiteData;
        std::shared_ptr<HttpClient> client_;

    protected:
        virtual void processResponse();

    public:
        explicit Api(std::string_view key, const std::shared_ptr<HttpClient> &client);
        Api(const Api &copy) = delete;
        Api(Api &&copy) = delete;
        Api &operator=(const Api &copy) = delete;
        Api &operator=(Api &&copy) = delete;
        const std::string &apiKey() const;
        std::string &apiKey();
        const std::string &getRequest() const;
        std::string &getRequest();
        virtual bool sendRequest();
        const std::string &getResponse() const;
        T getData();
        virtual ~Api() = default;
    };

    template <typename T>
    stockApi::Api<T>::Api(std::string_view key, const std::shared_ptr<HttpClient> &client) : apiKey_(key), client_(client)
    {
    }

    template <typename T>
    const std::string &stockApi::Api<T>::apiKey() const
    {
        return apiKey_;
    }

    template <typename T>
    std::string &stockApi::Api<T>::apiKey()
    {
        return apiKey_;
    }

    template <typename T>
    const std::string &stockApi::Api<T>::getRequest() const
    {
        return request_;
    }

    template <typename T>
    std::string &stockApi::Api<T>::getRequest()
    {
        return request_;
    }

    template <typename T>
    bool stockApi::Api<T>::sendRequest()
    {
        dataReady_ = false;
        return client_->sendRequest(request_);
    }

    template <typename T>
    const std::string &stockApi::Api<T>::getResponse() const
    {
        return client_->getResponse();
    }

    template <typename T>
    void stockApi::Api<T>::processResponse()
    {
        try
        {
            boost::json::value json = boost::json::parse(getResponse());
            suiteData = boost::json::value_to<T>(json);
        }
        catch (const boost::system::system_error &e)
        {
            std::cerr << "Error" << e.code() << ": " << e.what() << std::endl;
            suiteData.reset();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            suiteData.reset();
        }
        catch (...)
        {
            std::cerr << "Unknown error" << std::endl;
            suiteData.reset();
        }
    }

    template <typename T>
    T stockApi::Api<T>::getData()
    {
        if (!dataReady_)
        {
            suiteData.reset();
            processResponse();
            dataReady_ = true;
        }
        return suiteData;
    }

} // namespace stockApi

#endif
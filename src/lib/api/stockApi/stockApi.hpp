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
    class ApiBase
    {
    };

    template <typename T>
    class Api : public ApiBase
    {
    private:
        bool dataReady_ = false;
        std::string apiKey_;
        std::string request_;
        T data_;
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
            data_ = boost::json::value_to<T>(json);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            data_.reset();
        }
    }

    template <typename T>
    T stockApi::Api<T>::getData()
    {
        if (!dataReady_)
        {
            data_.reset();
            processResponse();
            dataReady_ = true;
        }
        return data_;
    }

} // namespace stockApi

#endif
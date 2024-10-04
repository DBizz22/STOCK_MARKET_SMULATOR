#ifndef CURLCLIENT_HPP_
#define CURLCLIENT_HPP_

#include <iostream>
#include <string>
#include <curl/curl.h>
#include "httpClient.hpp"

class CurlClient : public HttpClient
{
private:
    inline static bool curlBackendReady = false;
    bool curlOk;
    CURL *curlHandle;
    std::string response;
    static size_t Callback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((std::string *)userp)->append((char *)contents, size * nmemb);
        return size * nmemb;
    }

public:
    CurlClient();
    CurlClient(const CurlClient &copy);
    CurlClient(CurlClient &&copy) noexcept;
    CurlClient &operator=(CurlClient &&copy) noexcept;
    CurlClient &operator=(const CurlClient &copy);
    bool sendRequest(const std::string &request);
    const std::string &getResponse() const;
    ~CurlClient();
};

#endif
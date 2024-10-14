#include "curlClient.hpp"

CurlClient::CurlClient() : curlHandle(NULL), response(""), curlOk(false)
{
    if (!curlBackendReady)
    {
        CURLsslset opt = curl_global_sslset(CURLSSLBACKEND_SCHANNEL, NULL, NULL);
        if (opt == CURLSSLSET_TOO_LATE)
        {
            std::cerr << "WIN SSL Backend late initialization\n";
        }
        else if (opt == CURLSSLSET_NO_BACKENDS)
        {
            std::cerr << "No WIN SSL Backend\n";
        }
        else if (opt == CURLSSLSET_UNKNOWN_BACKEND)
        {
            std::cerr << "Unknown WIN SSL Backend\n";
        }

        if (opt != CURLSSLSET_OK)
        {
            curlOk = false;
            return;
        }
    }
    curl_global_init(CURL_GLOBAL_ALL);
    curlBackendReady = true;

    curlHandle = curl_easy_init();
    if (curlHandle == NULL)
    {
        std::cerr << "CURL easy init failed\n";
        curlOk = false;
        return;
    }
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, Callback);
    curlOk = true;
}
CurlClient::CurlClient(const CurlClient &copy)
{
    curlOk = copy.curlOk;
    response = copy.response;
    curlHandle = curl_easy_duphandle(copy.curlHandle);
}

CurlClient::CurlClient(CurlClient &&copy) noexcept
{
    curlOk = copy.curlOk;
    response = std::move(copy.response);
    curlHandle = curl_easy_duphandle(copy.curlHandle);
    copy.curlHandle = nullptr;
}

CurlClient &CurlClient::operator=(const CurlClient &copy)
{
    if (this != &copy)
    {
        curl_easy_cleanup(curlHandle);
        curlHandle = curl_easy_duphandle(copy.curlHandle);
        curlOk = copy.curlOk;
        response = copy.response;
    }
    return *this;
}
CurlClient &CurlClient::operator=(CurlClient &&copy) noexcept
{
    if (this != &copy)
    {
        curl_easy_cleanup(curlHandle);
        curlHandle = copy.curlHandle;
        copy.curlHandle = NULL;
        curlOk = copy.curlOk;
        response = std::move(copy.response);
    }
    return *this;
}

bool CurlClient::sendRequest(const std::string &request)
{
    response.clear();
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)&response);
    curl_easy_setopt(curlHandle, CURLOPT_URL, request.c_str());
    CURLcode res = curl_easy_perform(curlHandle);

    if (res != CURLE_OK)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }
    return true;
}

const std::string &CurlClient::getResponse() const
{
    return response;
}
CurlClient::~CurlClient()
{
    curl_easy_cleanup(curlHandle);
}

;
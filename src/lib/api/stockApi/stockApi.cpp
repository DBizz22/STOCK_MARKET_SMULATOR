#include "stockApi.hpp"

#ifndef USE_TEMPLATE_
stoc < Api::Api::Api(const std::string &key, const std::shared_ptr<HttpClient> &client) : client_(client), apiKey_(key)
{
}

const std::string &stockApi::Api::apiKey() const
{
    return apiKey_;
}

std::string &stockApi::Api::apiKey()
{
    return apiKey_;
}

const std::string &stockApi::Api::getRequest() const
{
    return request_;
}

std::string &stockApi::Api::getRequest()
{
    return request_;
}

bool stockApi::Api::sendRequest()
{
    return client_->sendRequest(request_);
}

const std::string &stockApi::Api::getResponse() const
{
    return client_->getResponse();
}
#endif

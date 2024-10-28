#ifndef TIMEAPI_HPP_
#define TIMEAPI_HPP_

#include <string>
#include <memory>
#include "httpClient.hpp"
#include "boost/json.hpp"

struct DateTime
{
    std::string date;
    std::string time;
    friend DateTime tag_invoke(const boost::json::value_to_tag<DateTime> &, const boost::json::value &json);
    friend bool operator==(const DateTime &lhs, const DateTime &rhs);
    friend bool operator!=(const DateTime &lhs, const DateTime &rhs);
    friend bool operator<(const DateTime &lhs, const DateTime &rhs);
    friend bool operator>(const DateTime &lhs, const DateTime &rhs);
    friend double operator-(const DateTime &lhs, const DateTime &rhs);
};

class TimeApi
{
public:
    virtual bool sendRequest() = 0;
    virtual std::string getResponse() const = 0;
    virtual DateTime getTime() = 0;
    virtual ~TimeApi() = default;
};

class UTCTimeApi : public TimeApi
{
private:
    std::shared_ptr<HttpClient> client_;
    std::string requestString_;
    bool dataReady_ = false;
    DateTime timestamp_;
    void processResponse();

public:
    explicit UTCTimeApi(const std::shared_ptr<HttpClient> &client);
    bool sendRequest();
    std::string getResponse() const;
    DateTime getTime();
    ~UTCTimeApi();
};

#endif
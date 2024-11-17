#include "timeApi.hpp"
#include <ctime>
#include <iomanip>

DateTime tag_invoke(const boost::json::value_to_tag<DateTime> &, const boost::json::value &json)
{
    DateTime dateTime;
    std::string dateTimeStr = json.as_object().find("dateTime")->value().as_string().c_str();
    size_t pos = dateTimeStr.find('T');
    size_t end = dateTimeStr.find('.');
    dateTime.date = dateTimeStr.substr(0, pos);
    dateTime.time = dateTimeStr.substr(pos + 1, end - pos - 1);
    return dateTime;
}

bool operator==(const DateTime &lhs, const DateTime &rhs)
{
    return lhs.date == rhs.date && lhs.time == rhs.time;
}

bool operator!=(const DateTime &lhs, const DateTime &rhs)
{
    return lhs.date != rhs.date || lhs.time != rhs.time;
}

bool operator<(const DateTime &lhs, const DateTime &rhs)
{
    return lhs.date < rhs.date || (lhs.date == rhs.date && lhs.time < rhs.time);
}

bool operator>(const DateTime &lhs, const DateTime &rhs)
{
    return lhs.date > rhs.date || (lhs.date == rhs.date && lhs.time > rhs.time);
}

double operator-(const DateTime &lhs, const DateTime &rhs)
{
    const char *format = "%Y-%m-%d %H:%M:%S";
    std::tm lhsTm, rhsTm;
    std::istringstream ssL(lhs.date + " " + lhs.time);
    std::istringstream ssR(rhs.date + " " + rhs.time);
    ssL >> std::get_time(&lhsTm, format);
    ssR >> std::get_time(&rhsTm, format);
    time_t lhsTimestamp = mktime(&lhsTm);
    time_t rhsTimestamp = mktime(&rhsTm);
    return difftime(lhsTimestamp, rhsTimestamp);
}

UTCTimeApi::UTCTimeApi(const std::shared_ptr<HttpClient> &client)
{
    client_ = client;
    requestString_ = "https://timeapi.io/api/time/current/zone?timeZone=Europe%2FLondon";
}

bool UTCTimeApi::sendRequest()
{
    dataReady_ = false;
    return client_->sendRequest(requestString_);
}

std::string UTCTimeApi::getResponse() const
{
    return client_->getResponse();
}

void UTCTimeApi::processResponse()
{
    try
    {
        boost::json::value json = boost::json::parse(getResponse());
        timestamp_ = boost::json::value_to<DateTime>(json);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        timestamp_ = {};
    }
}

DateTime UTCTimeApi::getTime()
{
    if (!dataReady_)
    {
        timestamp_ = {};
        processResponse();
        dataReady_ = true;
    }
    return timestamp_;
}

UTCTimeApi::~UTCTimeApi()
{
}
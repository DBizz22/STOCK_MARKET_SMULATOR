#include "include/alphavantage.hpp"
#include "curlClient.hpp"

using namespace stockApi;
int main(int argc, char **argv)
{
    stockApi::alphavantageApiSuite::alphavantageApi<stockApi::alphavantageApiSuite::ForexData> forex("test", std::make_shared<CurlClient>());
    stockApi::alphavantageApiSuite::alphavantageApi<stockApi::alphavantageApiSuite::StockData> stock("test", std::make_shared<CurlClient>());
    stockApi::Api<int> stock2("test", std::make_shared<CurlClient>());
    return 0;
}
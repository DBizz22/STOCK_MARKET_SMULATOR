#include "coordinator.hpp"

const std::string KEY("8ZVH03NQVEYUSF4V");
int main()
{
    std::shared_ptr<database::DatabaseClient> databaseClient;
    databaseClient = std::make_shared<database::mysql::MySQLClient>(database::mysql::connectionParams());
    std::shared_ptr<ApiManager> apiManager;
    apiManager = std::make_shared<ApiManager>();
    std::shared_ptr<HttpClient> httpClient;
    httpClient = std::make_shared<CurlClient>();
    apiManager->UTCTimeApi = std::make_shared<UTCTimeApi>(httpClient);
    apiManager->stockApi = std::make_shared<stockApi::alphavantageApiSuite::alphavantageApi<stockApi::alphavantageApiSuite::SuiteStockData>>(KEY, httpClient);
    apiManager->forexApi = std::make_shared<stockApi::alphavantageApiSuite::alphavantageApi<stockApi::alphavantageApiSuite::SuiteForexData>>(KEY, httpClient);
    std::shared_ptr<RefreshModel> refreshModel_ = std::make_shared<RefreshModel>(4, databaseClient, *apiManager);
    int equities = refreshModel_->refresh();
    std::cout << std::to_string(equities) + " loaded";
    return 0;
}

/*apiManager->forexApi->setRequest("USD", "NGN");
if (apiManager->forexApi->sendRequest())
{
    std::cout << "Send request Success";
    stockApi::Forex forex = apiManager->forexApi->getForex();
    std::cout << "Forex rate: " << forex.price;
}
else
{
    std::cout << "Failed to send request";
}*/
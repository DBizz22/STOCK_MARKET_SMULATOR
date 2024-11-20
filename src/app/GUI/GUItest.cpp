#include "coordinator.hpp"

const std::string KEY("8ZVH03NQVEYUSF4V");
class GUI : public wxFrame
{
private:
    database::mysql::connectionParams params;
    std::shared_ptr<database::DatabaseClient> databaseClient;
    std::shared_ptr<ApiManager> apiManager;
    std::shared_ptr<HttpClient> httpClient;

public:
    GUI() : wxFrame(nullptr, wxID_ANY, "STOCK MARKET SIMULATOR TEST")
    {
        databaseClient = std::make_shared<database::mysql::MySQLClient>(database::mysql::connectionParams());
        httpClient = std::make_shared<CurlClient>();
        apiManager = std::make_shared<ApiManager>();
        apiManager->UTCTimeApi = std::make_shared<UTCTimeApi>(httpClient);
        apiManager->stockApi = std::make_shared<stockApi::alphavantageApiSuite::alphavantageApi<stockApi::alphavantageApiSuite::SuiteStockData>>(KEY, httpClient);
        apiManager->forexApi = std::make_shared<stockApi::alphavantageApiSuite::alphavantageApi<stockApi::alphavantageApiSuite::SuiteForexData>>(KEY, httpClient);
        std::shared_ptr<RefreshModel> refreshModel_ = std::make_shared<RefreshModel>(4, databaseClient, *apiManager);
        int equities = refreshModel_->refresh();
        wxMessageBox(std::to_string(equities) + " Loaded", "Result");
    }
};

class GUITestAPP : public wxApp
{
    virtual bool OnInit();
};

wxIMPLEMENT_APP(GUITestAPP);

bool GUITestAPP::OnInit()
{
    GUI *frame = new GUI();
    frame->Show(true);
    return true;
}
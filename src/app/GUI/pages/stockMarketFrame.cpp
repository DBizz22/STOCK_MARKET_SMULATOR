#include "stockMarketFrame.hpp"
#include <stdbool.h>

const std::string KEY("8ZVH03NQVEYUSF4V");

StockMarketFrame::StockMarketFrame() : wxFrame(nullptr, wxID_ANY, "STOCK MARKET SIMULATOR")
{
    credentials.database = "stock_db";
    credentials.user = "root";
    credentials.password = "enyiudo12real";
    databaseClient = std::make_shared<database::mysql::MySQLClient>(credentials);

    httpClient = std::make_shared<CurlClient>();
    apiManager = std::make_shared<ApiManager>();
    apiManager->UTCTimeApi = std::make_shared<UTCTimeApi>(httpClient);
    apiManager->stockApi = std::make_shared<stockApi::alphavantageApiSuite::alphavantageApi<stockApi::alphavantageApiSuite::SuiteStockData>>(KEY, httpClient);
    apiManager->forexApi = std::make_shared<stockApi::alphavantageApiSuite::alphavantageApi<stockApi::alphavantageApiSuite::SuiteForexData>>(KEY, httpClient);
    loginPage = std::make_shared<LoginPage>(this, currentPage, account, databaseClient);
    registerPage = std::make_shared<RegisterPage>(this, currentPage, databaseClient);
    loginPage->create();
    previousPage = currentPage = PAGES::LOGIN;

    this->SetIcon(wxICON(icon));
    Bind(wxEVT_CLOSE_WINDOW, &StockMarketFrame::close, this, wxID_ANY);
    Bind(wxEVT_BUTTON, &StockMarketFrame::changePageByButton, this, wxID_ANY);
}

void StockMarketFrame::close(wxCloseEvent &event)
{
    this->Destroy();
}

void StockMarketFrame::changePageByButton(wxCommandEvent &event)
{
    if (previousPage == currentPage)
    {
        return;
    }

    if (previousPage == PAGES::LOGIN && currentPage == PAGES::PROFILE)
    {
        refreshModel_ = std::make_shared<RefreshModel>(account.ID, databaseClient, *apiManager);
        if (!refreshModel_->refresh())
        {
            wxMessageBox("Failed to load data", "Error");
            currentPage = PAGES::LOGIN;
            this->Show();
            return;
        }
    }

    switch (previousPage)
    {
    case PAGES::LOGIN:
        loginPage->close();
        break;
    case PAGES::REGISTER:
        registerPage->close();
        break;
    case PAGES::PROFILE:
        profilePage->close();
        break;

    default:
        break;
    }

    switch (currentPage)
    {
    case PAGES::LOGIN:
        loginPage->create();
        break;
    case PAGES::REGISTER:
        registerPage->create();
        break;
    case PAGES::PROFILE:
        childFrame = new wxFrame(this, wxID_ANY, "STOCK MARKET SIMULATOR");
        childFrame->SetIcon(wxICON(icon));
        childFrame->Bind(wxEVT_CLOSE_WINDOW, &StockMarketFrame::close, this, wxID_ANY);
        childFrame->Bind(wxEVT_LIST_ITEM_ACTIVATED, &StockMarketFrame::changePageByList, this, wxID_ANY);
        childFrame->Bind(wxEVT_MENU, &StockMarketFrame::changePageByMenu, this, wxID_ANY);
        profilePage = std::make_shared<ProfilePage>(childFrame, currentPage, selectedProfileData, account, databaseClient, apiManager);
        profilePage->create();
        break;
    case PAGES::EQUITIES:
        equityPage->create(selectedProfileData);
        break;
    default:
        break;
    }
    previousPage = currentPage;
}

void StockMarketFrame::changePageByList(wxListEvent &event)
{
    if (previousPage == currentPage)
    {
        return;
    }

    switch (previousPage)
    {
    case PAGES::PROFILE:
        profilePage->close();
        break;
    case PAGES::EQUITIES:
        equityPage->close();
        break;
    default:
        break;
    }

    switch (currentPage)
    {
    case PAGES::PROFILE:
        profilePage->create();
        break;
    case PAGES::EQUITIES:
        equityPage = std::make_shared<EquityPage>(childFrame, currentPage, account, databaseClient, apiManager);
        equityPage->create(selectedProfileData);
        break;
    default:
        break;
    }
    previousPage = currentPage;
}

void StockMarketFrame::changePageByMenu(wxCommandEvent &event)
{
    if (previousPage == currentPage)
    {
        return;
    }

    switch (previousPage)
    {
    case PAGES::EQUITIES:
        equityPage->close();
        break;
    default:
        break;
    }

    switch (currentPage)
    {
    case PAGES::LOGIN:
        loginPage->create();
        break;
    case PAGES::PROFILE:
        profilePage->create();
        break;
    default:
        break;
    }
    previousPage = currentPage;
}
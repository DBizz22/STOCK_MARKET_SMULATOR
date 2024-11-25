#ifndef STOCKMARKETFRAME
#define STOCKMARKETFRAME

#include "coordinator.hpp"
#include "loginPage.hpp"
#include "registerPage.hpp"
#include "profilePage.hpp"
#include "equityPage.hpp"

class StockMarketFrame : public wxFrame
{
public:
    StockMarketFrame();

private:
    ProfileData selectedProfileData;
    database::AccountRecord account;
    database::mysql::connectionParams credentials;
    std::shared_ptr<RefreshModel> refreshModel_;
    std::shared_ptr<LoginPage> loginPage;
    std::shared_ptr<RegisterPage> registerPage;
    std::shared_ptr<ProfilePage> profilePage;
    std::shared_ptr<EquityPage> equityPage;
    std::shared_ptr<HttpClient> httpClient;
    std::shared_ptr<database::DatabaseClient> databaseClient;
    std::shared_ptr<ApiManager> apiManager;

    std::shared_ptr<MenuBar> menuBar;
    void close(wxCloseEvent &event);
    void changePageByButton(wxCommandEvent &event);
    void changePageByMenu(wxCommandEvent &event);
    void changePageByList(wxListEvent &event);
    PAGES previousPage, currentPage;
    wxFrame *childFrame;
};

#endif
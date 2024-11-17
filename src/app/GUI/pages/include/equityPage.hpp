#ifndef EQUITYPAGE_HPP_
#define EQUITYPAGE_HPP_

#include "coordinator.hpp"

class MenuBar;

class EquityPage
{
private:
    static const int Width, Height;
    double quantity_ = 0;
    double cost_ = 0;
    double totalCost_ = 0;
    SearchData searchData;
    wxFont font;
    wxTextCtrl *tradeQuantity;
    wxStaticText *tradeDetails;
    wxTextCtrl *searchTab;
    wxStaticText *stockDetails;
    wxStaticText *detailsLabel;
    PAGES &currentPage_;
    wxFrame *frame_;
    wxPanel *panel;
    wxListCtrl *equityDataTable;
    unsigned int profileID_;
    std::shared_ptr<database::DatabaseClient> databaseClient_;
    std::shared_ptr<ApiManager> &apiManager_;
    std::shared_ptr<SearchModel> searchModel_;
    std::shared_ptr<EquityModel> equityModel_;
    ProfileData profileData_;
    std::vector<EquityData> equityDatas_;
    std::string selectedEquitySymbol_;
    database::AccountRecord &account_;
    void sortByColumn(int column);
    void addEquityDataToTable(const EquityData &equityData);
    int sortDirection[7] = {-1, -1, -1, -1, -1, -1};

    std::unordered_set<std::unique_ptr<EquityData>> equityDataPtrs;
    static std::string formatValueStr(const double &value);
    static int compareValues(double a, double b, int direction);
    static int compareStrings(const std::string &a, const std::string &b, int direction);
    void updateDetails(const std::string &symbol, const double &price, const std::string &currency);
    void buyEquity();
    void sellEquity();
    void sellAllEquity();
    void createHeaders(wxBoxSizer *mainSizer);
    void createEquityTable(wxBoxSizer *mainSizer);
    void createTradeDeskLabel(wxPanel *tradePanel, wxBoxSizer *tradeBox);
    void createTradeDeskSearcher(wxPanel *tradePanel, wxFlexGridSizer *grid);
    void createTradeDeskStockDetails(wxPanel *tradePanel, wxFlexGridSizer *grid);
    void createTradeDeskTradeDetails(wxPanel *tradePanel, wxFlexGridSizer *grid);
    void createTradeDeskButtons(wxPanel *tradePanel, wxFlexGridSizer *grid);
    void createTradeDesk(wxBoxSizer *mainSizer);
    void updateEquityTable();

public:
    EquityPage(wxFrame *frame, PAGES &currentPage, database::AccountRecord &account, const std::shared_ptr<database::DatabaseClient> &databaseClient, std::shared_ptr<ApiManager> &apiManager);
    void create(ProfileData &profileData);
    void close();
};

#endif
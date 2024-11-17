#include "equityPage.hpp"

const int EquityPage::Width = 760;
const int EquityPage::Height = 610;

void EquityPage::createHeaders(wxBoxSizer *mainSizer)
{
    auto label = new wxStaticText(panel, wxID_ANY, profileData_.name, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    label->SetFont(font);
    label->SetFont(label->GetFont().Scale(2));

    std::string details = " Count: " + std::to_string(profileData_.equitiesCount);
    details += "        Value: " + formatValueStr(profileData_.equitiesValue);
    details += "        Balance: " + formatValueStr(profileData_.balance) + " \n";
    if (profileData_.percentageChange >= 0)
        details += " Profit: ";
    else
        details += " Loss: ";
    double profitLoss = profileData_.balance + profileData_.equitiesValue - databaseClient_->getProfile(profileData_.ID).initialValue;
    details += formatValueStr(profitLoss);
    details += "        % Return: " + formatValueStr(profileData_.percentageChange);
    details += "    " + profileData_.currency + " ";

    detailsLabel = new wxStaticText(panel, wxID_ANY, details.c_str(), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    detailsLabel->SetFont(font);
    if (profileData_.percentageChange < 0)
        detailsLabel->SetForegroundColour(wxColor(255, 0, 0));
    else if (profileData_.percentageChange > 0)
        detailsLabel->SetForegroundColour(wxColor(0, 255, 0));
    else
        detailsLabel->SetForegroundColour(wxColor(255, 255, 255));
    detailsLabel->SetBackgroundColour(wxColor(0, 0, 0));

    mainSizer->Add(label, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(detailsLabel, 0, wxALIGN_CENTER);
}

void EquityPage::createEquityTable(wxBoxSizer *mainSizer)
{
    wxItemAttr attr;
    attr.SetTextColour(wxColour(0, 0, 0));
    // attr.SetBackgroundColour(wxColour(0, 0, 0));
    attr.SetFont(font);
    equityDataTable = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    equityDataTable->SetHeaderAttr(attr);
    equityDataTable->AppendColumn("Symbol", wxLIST_FORMAT_LEFT, 120);
    equityDataTable->AppendColumn("Value", wxLIST_FORMAT_LEFT, 120);
    equityDataTable->AppendColumn("Quantity", wxLIST_FORMAT_LEFT, 120);
    equityDataTable->AppendColumn("Total", wxLIST_FORMAT_LEFT, 120);
    equityDataTable->AppendColumn("Currency", wxLIST_FORMAT_LEFT, 120);
    equityDataTable->AppendColumn(R"(% Return)", wxLIST_FORMAT_LEFT, 120);
    // equityDataTable->SetForegroundColour(wxColor(255, 255, 0));
    equityDataTable->SetBackgroundColour(wxColour(0, 0, 0));

    mainSizer->Add(equityDataTable, 0, wxEXPAND | wxALL, 10);
    equityDataTable->Bind(wxEVT_LIST_COL_CLICK, [this](wxListEvent &event)
                          { int selectedColumn = event.GetColumn();
                            this->sortByColumn(selectedColumn); });
    equityDataTable->Bind(wxEVT_LIST_ITEM_FOCUSED, [this](wxListEvent &event)
                          {wxIntPtr ptr= event.GetData(); 
                            selectedEquitySymbol_=reinterpret_cast<EquityData *>(ptr)->symbol;
                            updateDetails(selectedEquitySymbol_, reinterpret_cast<EquityData *>(ptr)->currentValue, reinterpret_cast<EquityData *>(ptr)->currency); });
    equityDataTable->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, [this](wxListEvent &event)
                          { wxIntPtr ptr = event.GetData();
                            selectedEquitySymbol_ = reinterpret_cast<EquityData *>(ptr)->symbol;
                             updateDetails(selectedEquitySymbol_, reinterpret_cast<EquityData *>(ptr)->currentValue, reinterpret_cast<EquityData *>(ptr)->currency); });
}

void EquityPage::createTradeDeskLabel(wxPanel *tradePanel, wxBoxSizer *tradeBox)
{
    auto tradeLabel = new wxStaticText(tradePanel, wxID_ANY, "TRADING DESK", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    tradeLabel->SetFont(font);
    tradeLabel->SetFont(tradeLabel->GetFont().Scale(1.6));
    tradeLabel->SetForegroundColour(wxColor(255, 255, 255));
    tradeBox->Add(tradeLabel, 0, wxALL | wxEXPAND, 10);
}

void EquityPage::createTradeDeskSearcher(wxPanel *tradePanel, wxFlexGridSizer *grid)
{
    auto searchBox = new wxFlexGridSizer(1, 2, 4, 0);
    auto searchAndResultBox = new wxBoxSizer(wxVERTICAL);
    searchBox->AddGrowableCol(0);
    auto searchLabel = new wxStaticText(tradePanel, wxID_ANY, "SYMBOL", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    searchLabel->SetForegroundColour(wxColor(255, 255, 255));
    searchLabel->SetFont(font);
    searchLabel->SetFont(searchLabel->GetFont().Scale(1.2));
    searchTab = new wxTextCtrl(tradePanel, wxID_ANY, "");

    searchTab->SetFont(font);
    auto searchBtn = new wxButton(tradePanel, wxID_ANY, "SEARCH", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

    searchBtn->SetForegroundColour(wxColor(0, 0, 0));
    searchBtn->SetBackgroundColour(wxColor(255, 255, 0));
    searchBtn->SetFont(font);

    searchBox->Add(searchTab, 1, wxRIGHT | wxBOTTOM | wxLEFT, 10);
    searchBox->Add(searchBtn, 1, wxRIGHT | wxBOTTOM, 10);

    grid->Add(searchLabel, 1, wxALIGN_LEFT);
    grid->Add(searchBox);

    searchBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                    {
        std::string searchText = searchTab->GetValue().ToStdString();
        std::transform(searchText.begin(), searchText.end(), searchText.begin(), [this](auto &ch)
                       { return std::toupper(ch); });
        searchData=searchModel_->findForex(searchText, profileData_.currency); 
        if(searchData.isEmpty())
        {
            searchData = searchModel_->findStock(searchText, profileData_.currency);
        }
        if(searchData.isEmpty())
        {
            wxMessageBox("Stock not found", "Error");
            return;
        }
        updateDetails(searchData.symbol, searchData.currentPrice, searchData.currency);
        selectedEquitySymbol_=searchText; });
}

void EquityPage::createTradeDeskStockDetails(wxPanel *tradePanel, wxFlexGridSizer *grid)
{
    auto stockLabel = new wxStaticText(tradePanel, wxID_ANY, "STOCK");

    stockDetails = new wxStaticText(tradePanel, wxID_ANY, "");
    stockDetails->SetFont(font);
    stockLabel->SetFont(font);
    stockLabel->SetFont(stockLabel->GetFont().Scale(1.2));
    stockLabel->SetForegroundColour(wxColor(255, 255, 255));
    stockDetails->SetForegroundColour(wxColor(255, 255, 255));

    grid->Add(stockLabel, 1, wxALIGN_LEFT);
    grid->Add(stockDetails);
}

void EquityPage::createTradeDeskTradeDetails(wxPanel *tradePanel, wxFlexGridSizer *grid)
{
    double balVal;
    wxFloatingPointValidator<double>
        val(3, &balVal, wxNUM_VAL_ZERO_AS_BLANK);
    val.SetRange(0.0000001, 1000000);
    auto quantityLabel = new wxStaticText(tradePanel, wxID_ANY, "QUANTITY", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    quantityLabel->SetFont(font);
    quantityLabel->SetFont(quantityLabel->GetFont().Scale(1.2));
    quantityLabel->SetForegroundColour(wxColor(255, 255, 255));

    tradeQuantity = new wxTextCtrl(tradePanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, val);
    tradeQuantity->SetFont(font);

    tradeDetails = new wxStaticText(tradePanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    tradeDetails->SetFont(font);
    tradeDetails->SetForegroundColour(wxColor(255, 255, 255));

    auto tradeCenterBox = new wxFlexGridSizer(1, 2, 4, 0);
    tradeCenterBox->AddGrowableCol(0);
    tradeCenterBox->Add(tradeQuantity, 1, wxRIGHT | wxBOTTOM | wxLEFT, 10);
    tradeCenterBox->Add(tradeDetails, 1, wxRIGHT | wxBOTTOM, 10);

    grid->Add(quantityLabel, 1, wxALIGN_LEFT);
    grid->Add(tradeCenterBox);

    tradeQuantity->Bind(wxEVT_TEXT, [this](wxCommandEvent &event)
                        {
                            std::string::size_type sz;
                            wxString value = tradeQuantity->GetValue();
                            value.ToDouble(&quantity_);
                        totalCost_ = quantity_ * cost_;
                        tradeDetails->SetLabel(std::to_string(totalCost_)); });
}

void EquityPage::createTradeDeskButtons(wxPanel *tradePanel, wxFlexGridSizer *grid)
{
    auto actionLabel = new wxStaticText(tradePanel, wxID_ANY, "ACTION");
    actionLabel->SetFont(font);
    actionLabel->SetFont(actionLabel->GetFont().Scale(1.2));
    actionLabel->SetForegroundColour(wxColor(255, 255, 255));

    auto buyBtn = new wxButton(tradePanel, wxID_ANY, "BUY");
    buyBtn->SetFont(font);
    buyBtn->SetForegroundColour(wxColor(0, 0, 0));
    buyBtn->SetBackgroundColour(wxColor(255, 255, 0));
    auto sellBtn = new wxButton(tradePanel, wxID_ANY, "SELL");
    sellBtn->SetFont(font);
    sellBtn->SetForegroundColour(wxColor(0, 0, 0));
    sellBtn->SetBackgroundColour(wxColor(255, 255, 0));
    auto sellAllBtn = new wxButton(tradePanel, wxID_ANY, "SELL BULK", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    sellAllBtn->SetFont(font);
    sellAllBtn->SetForegroundColour(wxColor(0, 0, 0));
    sellAllBtn->SetBackgroundColour(wxColor(255, 255, 0));

    auto tradeBtnBox = new wxBoxSizer(wxHORIZONTAL);
    tradeBtnBox->Add(buyBtn, 0, wxRIGHT, 10);
    tradeBtnBox->Add(sellBtn, 0, wxRIGHT, 10);
    tradeBtnBox->Add(sellAllBtn, 0, wxRIGHT, 10);

    grid->Add(actionLabel, 1, wxALIGN_LEFT);
    grid->Add(tradeBtnBox, 0, wxALIGN_RIGHT);

    buyBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
                 { buyEquity(); });

    sellBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                  { sellEquity(); });

    sellAllBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                     { sellAllEquity(); });
}

void EquityPage::createTradeDesk(wxBoxSizer *mainSizer)
{
    auto tradePanel = new wxPanel(panel);
    auto grid = new wxFlexGridSizer(4, 2, 10, 0);
    auto tradeBox = new wxBoxSizer(wxVERTICAL);

    createTradeDeskLabel(tradePanel, tradeBox);
    createTradeDeskSearcher(tradePanel, grid);
    createTradeDeskStockDetails(tradePanel, grid);
    createTradeDeskTradeDetails(tradePanel, grid);
    createTradeDeskButtons(tradePanel, grid);

    tradeBox->Add(grid, 0, wxBOTTOM, 10);

    wxBoxSizer *tradeBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    tradeBoxSizer->Add(tradeBox, 0, wxLEFT | wxRIGHT | wxEXPAND, 70);
    tradePanel->SetSizerAndFit(tradeBoxSizer);
    tradePanel->SetBackgroundColour(wxColour(0, 0, 0));

    mainSizer->Add(tradePanel, 0, wxALIGN_CENTER);
}

std::string EquityPage::formatValueStr(const double &value)
{
    std::string values("123456789");
    std::string valueStr = std::to_string(value);
    auto valueStrIter = std::find_first_of(valueStr.begin(), valueStr.end(), values.begin(), values.end());
    if (valueStrIter == valueStr.end())
        return "0.00";

    int intPos = valueStr.find(*valueStrIter);
    int pointPos = valueStr.find('.');

    if (intPos < pointPos)
        valueStr.erase(pointPos + 3, valueStr.size() - (pointPos + 3));

    return valueStr;
}

void EquityPage::addEquityDataToTable(const EquityData &equityData)
{
    int index = equityDataTable->GetItemCount();
    wxFont itemFont;
    itemFont.SetWeight(wxFONTWEIGHT_EXTRAHEAVY);
    itemFont = itemFont.Scale(1.45);

    equityDataTable->InsertItem(index, equityData.symbol);
    equityDataTable->SetItem(index, 1, formatValueStr(equityData.currentValue));
    equityDataTable->SetItem(index, 2, formatValueStr(equityData.quantity));
    equityDataTable->SetItem(index, 3, formatValueStr(equityData.totalValue));
    equityDataTable->SetItem(index, 4, equityData.currency);
    equityDataTable->SetItem(index, 5, formatValueStr(equityData.percentageChange) + "%");
    equityDataTable->SetItemFont(index, itemFont);

    if (equityData.percentageChange < 0)
        equityDataTable->SetItemTextColour(index, wxColor(255, 0, 0));
    else if (equityData.percentageChange > 0)
        equityDataTable->SetItemTextColour(index, wxColor(0, 255, 0));
    else
        equityDataTable->SetItemTextColour(index, wxColor(255, 255, 255));

    auto dataPtr = std::make_unique<EquityData>(equityData);
    equityDataTable->SetItemPtrData(index, reinterpret_cast<wxIntPtr>(dataPtr.get()));
    equityDataPtrs.insert(std::move(dataPtr));
}

int EquityPage::compareValues(double a, double b, int direction)
{
    return a == b ? 0 : (a < b) ? -direction
                                : direction;
}

int EquityPage::compareStrings(const std::string &a, const std::string &b, int direction)
{
    return a.compare(b) * direction;
}

void EquityPage::sortByColumn(int column)
{
    switch (column)
    {
    case 0:
        equityDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                   { return compareStrings(reinterpret_cast<EquityData *>(item1)->symbol, reinterpret_cast<EquityData *>(item2)->symbol, static_cast<int>(direction)); }, this->sortDirection[0]);
        break;
    case 1:
        equityDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                   { return compareValues(reinterpret_cast<EquityData *>(item1)->currentValue, reinterpret_cast<EquityData *>(item2)->currentValue, static_cast<int>(direction)); }, this->sortDirection[column]);
        break;
    case 2:
        equityDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                   { return compareValues(reinterpret_cast<EquityData *>(item1)->quantity, reinterpret_cast<EquityData *>(item2)->quantity, static_cast<int>(direction)); }, this->sortDirection[column]);
        break;
    case 3:
        equityDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                   { return compareValues(reinterpret_cast<EquityData *>(item1)->totalValue, reinterpret_cast<EquityData *>(item2)->totalValue, static_cast<int>(direction)); }, this->sortDirection[column]);
        break;
    case 4:
        equityDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                   { return compareStrings(reinterpret_cast<EquityData *>(item1)->currency, reinterpret_cast<EquityData *>(item2)->currency, static_cast<int>(direction)); }, this->sortDirection[column]);
        break;
    case 5:
        equityDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                   { return compareValues(reinterpret_cast<EquityData *>(item1)->percentageChange, reinterpret_cast<EquityData *>(item2)->percentageChange, static_cast<int>(direction)); }, this->sortDirection[column]);
        break;
    default:
        break;
    }

    equityDataTable->Refresh();
    this->sortDirection[column] = -this->sortDirection[column];
}

void EquityPage::updateDetails(const std::string &symbol, const double &price, const std::string &currency)
{
    cost_ = price;
    std::string result = " 1 ";
    result += symbol;
    result += " = " + formatValueStr(price);
    result += " " + currency;
    stockDetails->SetLabel(result.c_str());
    int pos = result.find('=');
    result = result.substr(pos);
    tradeDetails->SetLabel(result.c_str());
    if (quantity_)
        tradeQuantity->Clear();
    tradeQuantity->SetValue("1");
    quantity_ = 1;
    totalCost_ = quantity_ * cost_;
}

void EquityPage::updateEquityTable()
{
    equityDataTable->DeleteAllItems();
    equityDataPtrs.clear();
    std::for_each(equityDatas_.begin(), equityDatas_.end(), [this](auto &equityData)
                  { addEquityDataToTable(equityData); });

    std::string details = " Count: " + std::to_string(profileData_.equitiesCount);
    details += "        Value: " + formatValueStr(profileData_.equitiesValue);
    details += "        Balance: " + formatValueStr(profileData_.balance) + " \n";
    if (profileData_.percentageChange >= 0)
        details += " Profit: ";
    else
        details += " Loss: ";
    double profitLoss = profileData_.balance + profileData_.equitiesValue - databaseClient_->getProfile(profileData_.ID).initialValue;
    details += formatValueStr(profitLoss);
    details += "        % Return: " + formatValueStr(profileData_.percentageChange);
    details += "    " + profileData_.currency + " ";

    detailsLabel->SetLabel(details.c_str());
    if (profileData_.percentageChange < 0)
        detailsLabel->SetForegroundColour(wxColor(255, 0, 0));
    else if (profileData_.percentageChange > 0)
        detailsLabel->SetForegroundColour(wxColor(0, 255, 0));
    else
        detailsLabel->SetForegroundColour(wxColor(255, 255, 255));
    detailsLabel->SetBackgroundColour(wxColor(0, 0, 0));

    frame_->Refresh();
}

void EquityPage::buyEquity()
{
    auto equityDataIter = std::find_if(equityDatas_.begin(), equityDatas_.end(), [this](auto &equityData)
                                       { return equityData.symbol == selectedEquitySymbol_; });
    if (equityDataIter != equityDatas_.end())
    {
        if (!equityModel_->buy(equityDataIter->ID, quantity_))
        {
            wxMessageBox("Failed Transaction", "Error");
        }
    }
    else if (!equityModel_->addEquity(searchData, quantity_))
    {
        wxMessageBox("Failed Transaction", "Error");
    }
    updateEquityTable();
    return;
}

void EquityPage::sellEquity()
{
    auto equityDataIter = std::find_if(equityDatas_.begin(), equityDatas_.end(), [this](auto &equityData)
                                       { return equityData.symbol == selectedEquitySymbol_; });
    if (equityDataIter != equityDatas_.end())
    {
        if (!equityModel_->sell(equityDataIter->ID, quantity_))
        {
            wxMessageBox("Failed Transaction", "Error");
        }
    }
    else
    {
        wxMessageBox("Equity not found", "Error");
    }
    updateEquityTable();
    return;
}

void EquityPage::sellAllEquity()
{
    auto equityDataIter = std::find_if(equityDatas_.begin(), equityDatas_.end(), [this](auto &equityData)
                                       { return equityData.symbol == selectedEquitySymbol_; });
    if (equityDataIter != equityDatas_.end())
    {
        if (!equityModel_->removeEquity(equityDataIter->ID))
        {
            wxMessageBox("Failed Transaction", "Error");
        }
    }
    else
    {
        wxMessageBox("Equity not found", "Error");
    }
    updateEquityTable();
    return;
}

EquityPage::EquityPage(wxFrame *frame, PAGES &currentPage, database::AccountRecord &account, const std::shared_ptr<database::DatabaseClient> &databaseClient, std::shared_ptr<ApiManager> &apiManager)
    : frame_(frame), currentPage_(currentPage), account_(account), databaseClient_(databaseClient), apiManager_(apiManager)
{
    font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
    font.Bold();
    font.Scale(1.5);
}

void EquityPage::create(ProfileData &profileData)
{
    wxSize size(Width, Height);
    frame_->SetMinSize(wxSize(0, 0));
    frame_->SetMaxSize(wxSize(1000, 1000));

    profileData_ = profileData;
    equityModel_ = std::make_shared<EquityModel>(profileData_, equityDatas_, databaseClient_);
    searchModel_ = std::make_shared<SearchModel>(databaseClient_, *apiManager_);

    panel = new wxPanel(frame_);
    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    createHeaders(mainSizer);
    createEquityTable(mainSizer);
    createTradeDesk(mainSizer);

    panel->SetSizerAndFit(mainSizer);
    panel->SetBackgroundColour(wxColour(128, 128, 128));

    frame_->SetSize(wxDefaultCoord, wxDefaultCoord, Width, Height, wxSIZE_USE_EXISTING);
    frame_->SetMinSize(size);
    frame_->SetMaxSize(size);

    updateEquityTable();
}

void EquityPage::close()
{
    delete panel;
}
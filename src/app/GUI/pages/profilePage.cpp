#include "profilePage.hpp"

bool ProfilePage::MenuCreated = false;
unsigned const int ProfilePage::Width = 760;
unsigned const int ProfilePage::Height = 350;

int ProfilePage::compareValues(double a, double b, int direction)
{
    return a == b ? 0 : (a < b) ? -direction
                                : direction;
}

int ProfilePage::compareStrings(const std::string &a, const std::string &b, int direction)
{
    return a.compare(b) * direction;
}

void ProfilePage::sortByColumn(int column)
{

    switch (column)
    {
    case 0:
        profileDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                    { return compareStrings(reinterpret_cast<ProfileData *>(item1)->name, reinterpret_cast<ProfileData *>(item2)->name, static_cast<int>(direction)); }, this->sortDirection[0]);
        break;
    case 1:
        profileDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                    { return compareStrings(reinterpret_cast<ProfileData *>(item1)->currency, reinterpret_cast<ProfileData *>(item2)->currency, static_cast<int>(direction)); }, this->sortDirection[1]);
        break;
    case 2:
        profileDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                    { return compareValues(reinterpret_cast<ProfileData *>(item1)->equitiesCount, reinterpret_cast<ProfileData *>(item2)->equitiesCount, static_cast<int>(direction)); }, this->sortDirection[2]);
        break;
    case 3:
        profileDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                    { return compareValues(reinterpret_cast<ProfileData *>(item1)->equitiesValue, reinterpret_cast<ProfileData *>(item2)->equitiesValue, static_cast<int>(direction)); }, this->sortDirection[3]);
        break;
    case 4:
        profileDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                    { return compareValues(reinterpret_cast<ProfileData *>(item1)->balance, reinterpret_cast<ProfileData *>(item2)->balance, static_cast<int>(direction)); }, this->sortDirection[4]);
        break;
    case 5:
        profileDataTable->SortItems([](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction)
                                    { return compareValues(reinterpret_cast<ProfileData *>(item1)->percentageChange, reinterpret_cast<ProfileData *>(item2)->percentageChange, static_cast<int>(direction)); }, this->sortDirection[5]);
        break;
    default:
        break;
    }

    profileDataTable->Refresh();
    this->sortDirection[column] = -this->sortDirection[column];
}

void ProfilePage::updateProfileTable()
{
    profileDataTable->DeleteAllItems();
    profileDataPtrs.clear();
    std::for_each(profileDatas_.begin(), profileDatas_.end(), [this](auto &profileData)
                  { addProfileDataToTable(profileData); });
    frame_->Refresh();
}

void ProfilePage::addProfileDataToTable(const ProfileData &profileData)
{
    std::string balanceStr = std::to_string(profileData.balance);
    int pos = balanceStr.find('.');
    balanceStr.erase(pos + 3, balanceStr.size() - (pos + 3));

    int index = profileDataTable->GetItemCount();
    wxFont itemFont;
    itemFont.SetWeight(wxFONTWEIGHT_EXTRAHEAVY);
    itemFont = itemFont.Scale(1.45);

    profileDataTable->InsertItem(index, profileData.name);
    profileDataTable->SetItem(index, 1, profileData.currency);
    profileDataTable->SetItem(index, 2, std::to_string(profileData.equitiesCount));
    profileDataTable->SetItem(index, 3, std::to_string(profileData.equitiesValue));
    profileDataTable->SetItem(index, 4, balanceStr);
    profileDataTable->SetItem(index, 5, std::to_string(profileData.percentageChange) + "%");
    profileDataTable->SetItemFont(index, itemFont);

    if (profileData.percentageChange < 0)
        profileDataTable->SetItemTextColour(index, wxColor(255, 0, 0));
    else if (profileData.percentageChange > 0)
        profileDataTable->SetItemTextColour(index, wxColor(0, 255, 0));
    else
        profileDataTable->SetItemTextColour(index, wxColor(255, 255, 255));

    auto dataPtr = std::make_unique<ProfileData>(profileData);

    profileDataTable->SetItemPtrData(index, reinterpret_cast<wxIntPtr>(dataPtr.get()));
    profileDataPtrs.insert(std::move(dataPtr));
}

void ProfilePage::closeNewProfilePage()
{
    newProfilePage.isActive = false;
    newProfilePage.frame->Destroy();
}

void ProfilePage::openNewProfilePage()
{
    if (newProfilePage.isActive)
        return;
    wxFont font;
    font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
    font.Bold();

    const int Width = 250;
    const int Height = 180;

    newProfilePage.frame = new wxFrame(frame_, wxID_ANY, "Details");
    newProfilePage.frame->SetIcon(wxICON(settingsIcon));
    wxPanel *panel = new wxPanel(newProfilePage.frame);
    panel->SetBackgroundColour(wxColor(125, 125, 125));
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer *grid = new wxFlexGridSizer(3, 2, 10, 10);
    grid->AddGrowableCol(1);

    double balVal;
    wxFloatingPointValidator<double>
        val(2, &balVal, wxNUM_VAL_ZERO_AS_BLANK);
    val.SetRange(0.0001, 1000000);

    auto profileNameLabel = new wxStaticText(panel, wxID_ANY, "Name");
    newProfilePage.profileNameInput = new wxTextCtrl(panel, wxID_ANY);
    auto balanceLabel = new wxStaticText(panel, wxID_ANY, "Balance");
    newProfilePage.balanceInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, val);
    auto currencyLabel = new wxStaticText(panel, wxID_ANY, "Currency");
    newProfilePage.currencyInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    auto saveBtn = new wxButton(panel, wxID_ANY, "OK");

    profileNameLabel->SetFont(font);
    balanceLabel->SetFont(font);
    currencyLabel->SetFont(font);

    grid->Add(profileNameLabel, 0, wxALIGN_RIGHT);
    grid->Add(newProfilePage.profileNameInput, 1, wxEXPAND);
    grid->Add(balanceLabel, 0, wxALIGN_RIGHT);
    grid->Add(newProfilePage.balanceInput, 1, wxEXPAND);
    grid->Add(currencyLabel, 0, wxALIGN_RIGHT);
    grid->Add(newProfilePage.currencyInput, 1, wxEXPAND);

    sizer->Add(grid, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 10);
    sizer->Add(saveBtn, 0, wxALIGN_RIGHT | wxALL, 5);

    panel->SetSizerAndFit(sizer);
    newProfilePage.frame->SetSize(wxDefaultCoord, wxDefaultCoord, Width, Height, wxSIZE_USE_EXISTING);
    newProfilePage.frame->SetMaxSize(wxSize(Width, Height));
    newProfilePage.frame->SetMinSize(wxSize(Width, Height));
    newProfilePage.frame->Show(true);

    saveBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
                  {std::string name=newProfilePage.profileNameInput->GetValue().ToStdString();
                    double balance;
                    newProfilePage.balanceInput->GetValue().ToDouble(&balance);
                    std::string currency = newProfilePage.currencyInput->GetValue().ToStdString();
                    std::transform(currency.begin(), currency.end(), currency.begin(), [](auto &ch)
                       { return std::toupper(ch); });
                    if(!profileModel_->validateProfileName(name))
                    {
                        wxMessageBox("Invalid profile name", "Error");
                        return;
                    }
                    if (!profileModel_->addProfile(name, balance, currency))
                    {
                        wxMessageBox("Failed to add profile", "Error");
                        return;
                    }
                    updateProfileTable();
                    wxMessageBox("Profile added", "Success");
                    closeNewProfilePage(); });
    newProfilePage.isActive = true;
    newProfilePage.frame->Bind(wxEVT_CLOSE_WINDOW, [&](wxCloseEvent &event)
                               { closeNewProfilePage(); });
}

void ProfilePage::closeEditProfilePage()
{
    editProfilePage.isActive = false;
    editProfilePage.frame->Destroy();
}

void ProfilePage::openEditProfilePage()
{
    if (editProfilePage.isActive)
        return;

    std::string currentName;
    auto profileDataIter = std::find_if(profileDatas_.begin(), profileDatas_.end(), [this](auto &profileData)
                                        { return profileData.ID == selectedProfileID_; });
    if (profileDataIter == profileDatas_.end())
        return;
    currentName = profileDataIter->name;

    wxFont font;
    font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
    font.Bold();

    editProfilePage.frame = new wxFrame(frame_, wxID_ANY, "Edit");
    editProfilePage.frame->SetIcon(wxICON(settingsIcon));

    wxPanel *panel = new wxPanel(editProfilePage.frame);
    panel->SetBackgroundColour(wxColor(125, 125, 125));
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer *grid = new wxFlexGridSizer(2, 2, 10, 10);
    grid->AddGrowableCol(1);

    auto oldProfileNameLabel = new wxStaticText(panel, wxID_ANY, "Current Name :");
    auto oldProfileName = new wxStaticText(panel, wxID_ANY, currentName.c_str());
    auto profileNameLabel = new wxStaticText(panel, wxID_ANY, "New Name");
    editProfilePage.profileNameInput = new wxTextCtrl(panel, wxID_ANY);
    auto saveBtn = new wxButton(panel, wxID_ANY, "OK");

    profileNameLabel->SetFont(font);
    oldProfileNameLabel->SetFont(font);
    oldProfileName->SetFont(font);

    grid->Add(oldProfileNameLabel, 0, wxALIGN_RIGHT);
    grid->Add(oldProfileName, 1, wxEXPAND);
    grid->Add(profileNameLabel, 0, wxALIGN_RIGHT);
    grid->Add(editProfilePage.profileNameInput, 1, wxEXPAND);

    sizer->Add(grid, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 10);
    sizer->Add(saveBtn, 0, wxALIGN_RIGHT | wxALL, 5);

    panel->SetSizerAndFit(sizer);
    editProfilePage.frame->SetSize(wxDefaultCoord, wxDefaultCoord, 250, 150, wxSIZE_USE_EXISTING);
    editProfilePage.frame->SetMaxSize(wxSize(250, 150));
    editProfilePage.frame->SetMinSize(wxSize(250, 150));
    editProfilePage.frame->Show(true);

    saveBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
                  {std::string name=editProfilePage.profileNameInput->GetValue().ToStdString();
                    if (!profileModel_->modifyProfileName(selectedProfileID_,name))
                    {
                        wxMessageBox("Failed to edit Profile Name", "Error");
                        return;
                    }
                    updateProfileTable();
                    wxMessageBox("Profile Name Changed", "Success");
                    closeEditProfilePage(); });

    editProfilePage.isActive = true;
    editProfilePage.frame->Bind(wxEVT_CLOSE_WINDOW, [&](wxCloseEvent &event)
                                { closeEditProfilePage(); });
}

void ProfilePage::deleteProfile()
{
    if (!selectedProfileID_)
        return;

    auto profileDataIter = std::find_if(profileDatas_.begin(), profileDatas_.end(), [this](auto &profileData)
                                        { return profileData.ID == selectedProfileID_; });
    if (profileDataIter == profileDatas_.end())
        return;

    std::string currentName = profileDataIter->name;
    int reply = wxMessageBox("Are you sure you want to delete " + currentName + " Profile?", "Confirm Delete", wxYES_NO);
    if (reply == wxYES)
    {
        if (!profileModel_->removeProfile(selectedProfileID_))
        {
            wxMessageBox("Failed to delete " + currentName + " Profile", "Error");
            return;
        }
        updateProfileTable();
        wxMessageBox(currentName + " deleted", "Success");
    }
}

void ProfilePage::createHeader(wxBoxSizer *mainSizer)
{
    auto label = new wxStaticText(panel, wxID_ANY, "PROFILE DASHBOARD", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    label->SetFont(font);
    label->SetFont(label->GetFont().Scale(2));
    mainSizer->Add(label, 0, wxEXPAND | wxALL, 5);
}

void ProfilePage::createProfileTable(wxBoxSizer *mainSizer)
{
    wxItemAttr attr;
    attr.SetTextColour(wxColour(0, 0, 0));
    // attr.SetBackgroundColour(wxColor(255, 255, 255));
    attr.SetFont(font);
    profileDataTable = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    profileDataTable->SetHeaderAttr(attr);
    profileDataTable->AppendColumn("Name", wxLIST_FORMAT_LEFT, 120);
    profileDataTable->AppendColumn("Currency", wxLIST_FORMAT_LEFT, 120);
    profileDataTable->AppendColumn("Count", wxLIST_FORMAT_LEFT, 120);
    profileDataTable->AppendColumn("Value", wxLIST_FORMAT_LEFT, 120);
    profileDataTable->AppendColumn("Balance", wxLIST_FORMAT_LEFT, 120);
    profileDataTable->AppendColumn(R"(% Return)", wxLIST_FORMAT_LEFT, 120);
    // profileDataTable->SetForegroundColour(wxColor(255, 255, 0));
    profileDataTable->SetBackgroundColour(wxColor(0, 0, 0));
    profileDataTable->Bind(wxEVT_LIST_ITEM_FOCUSED, [&](wxListEvent &event)
                           {wxIntPtr ptr= event.GetData(); 
                            selectedProfileID_=reinterpret_cast<ProfileData *>(ptr)->ID; });

    profileDataTable->Bind(wxEVT_LIST_COL_CLICK, [&](wxListEvent &event)
                           {int selectedColumn = event.GetColumn();
                            this->sortByColumn(selectedColumn); });

    profileDataTable->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, [&](wxListEvent &event)
                           {wxIntPtr ptr= event.GetData(); 
                            selectedProfileID_=reinterpret_cast<ProfileData *>(ptr)->ID; });

    profileDataTable->Bind(wxEVT_LIST_ITEM_ACTIVATED, [&](wxListEvent &event)
                           {wxIntPtr ptr= event.GetData(); 
                            selectedProfileID_=reinterpret_cast<ProfileData *>(ptr)->ID;
                            if(switchToEquityPage())
                                event.Skip(); });

    mainSizer->Add(profileDataTable, 1, wxEXPAND | wxALL, 10);
}

void ProfilePage::createButtons(wxBoxSizer *mainSizer)
{
    auto bottomPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition);
    auto bottomSizer = new wxFlexGridSizer(1, 3, 4, 0);
    bottomSizer->AddGrowableCol(1);
    auto addBtn = new wxButton(bottomPanel, wxID_ANY, "Add");
    auto editBtn = new wxButton(bottomPanel, wxID_ANY, "Edit");
    auto removeBtn = new wxButton(bottomPanel, wxID_ANY, "Delete");

    addBtn->SetFont(addBtn->GetFont().Scale(1.5));
    addBtn->SetBackgroundColour(wxColor(255, 255, 0));
    addBtn->SetForegroundColour(wxColor(0, 0, 0));

    editBtn->SetFont(editBtn->GetFont().Scale(1.5));
    editBtn->SetBackgroundColour(wxColor(255, 255, 0));
    editBtn->SetForegroundColour(wxColor(0, 0, 0));

    removeBtn->SetFont(removeBtn->GetFont().Scale(1.5));
    removeBtn->SetBackgroundColour(wxColor(255, 255, 0));
    removeBtn->SetForegroundColour(wxColor(0, 0, 0));

    bottomSizer->Add(addBtn, 1, wxLEFT | wxRIGHT | wxALIGN_LEFT, 20);
    bottomSizer->Add(editBtn, 1, wxLEFT | wxRIGHT | wxALIGN_LEFT, 20);
    bottomSizer->Add(removeBtn, 1, wxRIGHT | wxLEFT | wxALIGN_RIGHT, 20);
    bottomPanel->SetSizerAndFit(bottomSizer);

    addBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
                 { openNewProfilePage(); });

    editBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
                  { openEditProfilePage(); });

    removeBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
                    { deleteProfile(); });

    mainSizer->Add(bottomPanel, 0, wxBOTTOM | wxEXPAND, 10);
}

bool ProfilePage::switchToEquityPage()
{
    auto profileDataIter = std::find_if(profileDatas_.begin(), profileDatas_.end(), [this](auto &profileData)
                                        { return profileData.ID == selectedProfileID_; });
    if (profileDataIter == profileDatas_.end())
        return false;

    currentPage_ = PAGES::EQUITIES;
    selectedProfileData_ = *profileDataIter;
    return true;
}

ProfilePage::ProfilePage(wxFrame *frame, PAGES &currentPage, ProfileData &selectedProfileData, database::AccountRecord &account, std::shared_ptr<database::DatabaseClient> databaseClient, std::shared_ptr<ApiManager> &apiManager)
    : frame_(frame), currentPage_(currentPage), selectedProfileData_(selectedProfileData), account_(account), databaseClient_(databaseClient), apiManager_(apiManager)
{
    font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
    font.Bold();
    font.Scale(1.5);
}

void ProfilePage::create()
{
    if (!MenuCreated)
    {
        menuBar_ = std::make_shared<MenuBar>(frame_, currentPage_, account_, databaseClient_);
        menuBar_->create();
        MenuCreated = true;
    }

    /*refreshModel_ = std::make_shared<RefreshModel>(account_.ID, databaseClient_, *apiManager_);
    if (!refreshModel_->refresh())
    {
        wxMessageBox("Failed to load data", "Error");
    }*/
    profileModel_ = std::make_shared<ProfileModel>(account_.ID, profileDatas_, databaseClient_);
    frame_->SetMinSize(wxSize(0, 0));
    frame_->SetMaxSize(wxSize(1000, 1000));

    wxSize size(Width, Height);

    panel = new wxPanel(frame_);
    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    createHeader(mainSizer);
    createProfileTable(mainSizer);
    createButtons(mainSizer);

    mainSizer->SetMinSize(size);

    panel->SetSizerAndFit(mainSizer);
    panel->SetBackgroundColour(wxColour(128, 128, 128));

    frame_->SetSize(wxDefaultCoord, wxDefaultCoord, Width, Height, wxSIZE_USE_EXISTING);
    frame_->SetMinSize(size);
    frame_->SetMaxSize(size);

    updateProfileTable();
    frame_->Show(true);
}

void ProfilePage::close()
{
    delete panel;
}

ProfilePage::~ProfilePage()
{
    MenuCreated = false;
}
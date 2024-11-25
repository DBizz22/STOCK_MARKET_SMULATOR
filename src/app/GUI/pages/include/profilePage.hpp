#ifndef PROFILEPAGE_HPP_
#define PROFILEPAGE_HPP_

#include "coordinator.hpp"
#include "menuBar.hpp"

class MenuBar;

struct NewProfilePage
{
    bool isActive = false;
    wxFrame *frame;
    wxTextCtrl *profileNameInput, *balanceInput, *currencyInput;
};

struct EditProfilePage
{
    bool isActive = false;
    wxFrame *frame;
    wxTextCtrl *profileNameInput;
};

class ProfilePage
{
private:
    static bool MenuCreated;
    static const unsigned int Width, Height;
    PAGES &currentPage_;
    wxFont font;
    wxFrame *frame_;
    wxPanel *panel;
    wxListCtrl *profileDataTable;
    NewProfilePage newProfilePage;
    EditProfilePage editProfilePage;
    database::AccountRecord &account_;
    std::shared_ptr<database::DatabaseClient> databaseClient_;
    std::shared_ptr<ApiManager> &apiManager_;
    std::shared_ptr<MenuBar> menuBar_;
    // std::shared_ptr<RefreshModel> refreshModel_;
    std::shared_ptr<ProfileModel> profileModel_;
    std::vector<ProfileData> profileDatas_;
    unsigned int selectedProfileID_;
    ProfileData &selectedProfileData_;
    void sortByColumn(int column);
    void addProfileDataToTable(const ProfileData &profileData);
    int sortDirection[6] = {-1, -1, -1, -1, -1, -1};

    std::unordered_set<std::unique_ptr<ProfileData>> profileDataPtrs;
    static int compareValues(double a, double b, int direction);
    static int compareStrings(const std::string &a, const std::string &b, int direction);
    std::string formatValueStr(const double &value);
    void openNewProfilePage();
    void closeNewProfilePage();
    void openEditProfilePage();
    void closeEditProfilePage();
    void deleteProfile();
    void createHeader(wxBoxSizer *mainSizer);
    void createProfileTable(wxBoxSizer *mainSizer);
    void createButtons(wxBoxSizer *mainSizer);
    void updateProfileTable();
    bool switchToEquityPage();

public:
    explicit ProfilePage(wxFrame *frame, PAGES &currentPage, ProfileData &selectedProfileData, database::AccountRecord &account, const std::shared_ptr<database::DatabaseClient> &databaseClient, std::shared_ptr<ApiManager> &apiManager);
    void create();
    void close();
    ~ProfilePage();
};

#endif
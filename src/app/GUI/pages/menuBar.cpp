#include "menuBar.hpp"
#include "buildinfo.h"

void MenuBar::goToProfilePage(wxCommandEvent &event)
{
    currentPage_ = PAGES::PROFILE;
    event.Skip();
}

void MenuBar::closeSettingsPage()
{
    settings.isActive = false;
    settings.settingsFrame->Destroy();
}

void MenuBar::openSettingsPage(wxCommandEvent &event)
{
    if (settings.isActive)
        return;
    settingsModel_ = std::make_shared<SettingsModel>(account_, databaseClient_);

    wxFont font;
    font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
    font.Bold();

    settings.settingsFrame = new wxFrame(frame_, wxID_ANY, "Settings");
    settings.settingsFrame->SetIcon(wxICON(settingsIcon));
    wxPanel *settingsPanel = new wxPanel(settings.settingsFrame);
    settingsPanel->SetBackgroundColour(wxColor(125, 125, 125));
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer *grid = new wxFlexGridSizer(4, 2, 10, 10);
    grid->AddGrowableCol(1);

    auto accountNameLabel = new wxStaticText(settingsPanel, wxID_ANY, "Current Name");
    settings.accountName = new wxStaticText(settingsPanel, wxID_ANY, account_.username.c_str());
    auto newNameLabel = new wxStaticText(settingsPanel, wxID_ANY, "New Name");
    settings.newNameInput = new wxTextCtrl(settingsPanel, wxID_ANY);
    auto password1Label = new wxStaticText(settingsPanel, wxID_ANY, "New Password");
    settings.password1Input = new wxTextCtrl(settingsPanel, wxID_ANY);
    auto password2Label = new wxStaticText(settingsPanel, wxID_ANY, "Confirm New Password");
    settings.password2Input = new wxTextCtrl(settingsPanel, wxID_ANY);

    auto saveBtn = new wxButton(settingsPanel, wxID_ANY, "OK");

    accountNameLabel->SetFont(font);
    settings.accountName->SetFont(font);
    newNameLabel->SetFont(font);
    password1Label->SetFont(font);
    password2Label->SetFont(font);

    grid->Add(accountNameLabel, 0, wxALIGN_RIGHT);
    grid->Add(settings.accountName, 1, wxEXPAND);
    grid->Add(newNameLabel, 0, wxALIGN_RIGHT);
    grid->Add(settings.newNameInput, 1, wxEXPAND);
    grid->Add(password1Label, 0, wxALIGN_RIGHT);
    grid->Add(settings.password1Input, 1, wxEXPAND);
    grid->Add(password2Label, 0, wxALIGN_RIGHT);
    grid->Add(settings.password2Input, 1, wxEXPAND);

    sizer->Add(grid, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 10);
    sizer->Add(saveBtn, 0, wxALIGN_RIGHT | wxALL, 5);

    settingsPanel->SetSizerAndFit(sizer);
    settings.settingsFrame->SetSize(wxDefaultCoord, wxDefaultCoord, 400, 200, wxSIZE_USE_EXISTING);
    settings.settingsFrame->SetMaxSize(wxSize(400, 200));
    settings.settingsFrame->SetMinSize(wxSize(400, 200));
    settings.settingsFrame->Show(true);

    saveBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
                  {std::string name=settings.newNameInput->GetValue().ToStdString();
        std::string password1 = settings.password1Input->GetValue().ToStdString();
        std::string password2 = settings.password2Input->GetValue().ToStdString();
        if(name.empty()&&password1.empty()&&password2.empty())
            return;
        if (password1 != password2)
        {
            wxMessageBox("Passwords do not match", "Error");
            return;
                    }
                    if (!name.empty() && !settingsModel_->changeUserName(name))
                    {
                        wxMessageBox("Unable to change Account Username", "Error");
                        return;
                    }
                    if (!password1.empty() && !settingsModel_->changePassword(password1))
                    {
                        wxMessageBox("Unable to change Account Password", "Error");
                        return;
                    }
                    wxMessageBox("Account Change Successful", "Success");
                    closeSettingsPage(); });

    settings.isActive = true;
    settings.settingsFrame->Bind(wxEVT_CLOSE_WINDOW, [&](wxCloseEvent &event)
                                 { closeSettingsPage(); });
}

void MenuBar::openAboutPage(wxCommandEvent &event)
{
    std::string aboutText("Stock Market simulator\n");
    aboutText += "Version: " + BuildInfo::Version + '\n';
    aboutText += "Built: " + BuildInfo::Timestamp + "\n";
    aboutText += "SHA: " + BuildInfo::CommitSHA;
    wxMessageBox(aboutText.c_str(), "About");
}

void MenuBar::logout(wxCommandEvent &event)
{
    frame_->Hide();
    frame_->Destroy();
    currentPage_ = PAGES::LOGIN;
    event.Skip();
}

enum
{
    ID_MENU_DASHBOARD = 1,
    ID_MENU_SETTINGS,
    ID_MENU_ABOUT,
    ID_MENU_LOGOUT
};

MenuBar::MenuBar(wxFrame *frame, PAGES &currentPage, database::AccountRecord &account, const std::shared_ptr<database::DatabaseClient> &databaseClient)
    : frame_(frame), currentPage_(currentPage), account_(account), databaseClient_(databaseClient)
{
}

void MenuBar::create()
{
    if (menuBar != nullptr)
        return;
    menuFile = new wxMenu;
    menuFile->Append(ID_MENU_DASHBOARD, "&DashBoard",
                     "Go to Profile Dashboard");
    menuFile->Append(ID_MENU_SETTINGS, "&Settings");
    menuFile->Append(ID_MENU_ABOUT, "&About");
    menuFile->Append(ID_MENU_LOGOUT, "&Log Out");

    menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&Menu");

    frame_->SetMenuBar(menuBar);
    frame_->CreateStatusBar();
    frame_->SetStatusText("Welcome to Stock Market Simulator!");

    frame_->Bind(wxEVT_MENU, &MenuBar::goToProfilePage, this, ID_MENU_DASHBOARD);
    frame_->Bind(wxEVT_MENU, &MenuBar::openSettingsPage, this, ID_MENU_SETTINGS);
    frame_->Bind(wxEVT_MENU, &MenuBar::openAboutPage, this, ID_MENU_ABOUT);
    frame_->Bind(wxEVT_MENU, &MenuBar::logout, this, ID_MENU_LOGOUT);
}

void MenuBar::close()
{
    delete menuBar;
}
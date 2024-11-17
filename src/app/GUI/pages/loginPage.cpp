#include "loginPage.hpp"

unsigned const int LoginPage::Width = 500;
unsigned const int LoginPage::Height = 400;

void LoginPage::createHeader(wxBoxSizer *sizer)
{
    auto header = new wxStaticText(panel, wxID_ANY, "LOGIN", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    header->SetFont(header->GetFont().Scale(2));
    header->SetFont(font.Bold());
    header->SetForegroundColour(wxColor(255, 255, 0));
    sizer->Add(header, 0, wxTOP | wxEXPAND, 20);
}

void LoginPage::createUsernameInput(wxFlexGridSizer *grid)
{
    auto usernameLabel = new wxStaticText(panel, wxID_ANY, "Username", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    usernameLabel->SetFont(usernameLabel->GetFont().Scale(1.5));
    usernameLabel->SetForegroundColour(wxColor(255, 255, 0));
    grid->Add(usernameLabel, 1, wxLEFT | wxRIGHT | wxTOP, 20);
    usernameInput = new wxTextCtrl(panel, wxID_ANY, "");
    usernameInput->SetFont(usernameInput->GetFont().Scale(1.5));
    usernameInput->SetBackgroundColour(wxColour(0, 0, 0));
    usernameInput->SetForegroundColour(wxColor(255, 255, 0));
    grid->Add(usernameInput, 2, wxLEFT | wxRIGHT | wxEXPAND, 20);
}

void LoginPage::createPasswordInput(wxFlexGridSizer *grid)
{
    auto passwordLabel = new wxStaticText(panel, wxID_ANY, "Password", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    passwordLabel->SetFont(passwordLabel->GetFont().Scale(1.5));
    passwordLabel->SetForegroundColour(wxColor(255, 255, 0));
    grid->Add(passwordLabel, 1, wxLEFT | wxRIGHT | wxTOP, 20);
    passwordInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    passwordInput->SetFont(passwordInput->GetFont().Scale(1.5));
    passwordInput->SetBackgroundColour(wxColour(0, 0, 0));
    passwordInput->SetForegroundColour(wxColor(255, 255, 0));
    grid->Add(passwordInput, 2, wxLEFT | wxRIGHT | wxEXPAND, 20);
}

void LoginPage::createButtons(wxFlexGridSizer *grid)
{
    auto bottomPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition);
    auto bottomSizer = new wxFlexGridSizer(1, 2, 4, 0);
    bottomSizer->AddGrowableCol(1);
    wxButton *signUpBtn = new wxButton(bottomPanel, wxID_ANY, "SIGN UP");
    wxButton *logInBtn = new wxButton(bottomPanel, wxID_ANY, "LOGIN");

    logInBtn->SetFont(logInBtn->GetFont().Scale(1.5));
    logInBtn->SetBackgroundColour(wxColor(255, 255, 0));
    logInBtn->SetForegroundColour(wxColor(0, 0, 0));

    signUpBtn->SetFont(signUpBtn->GetFont().Scale(1.5));
    signUpBtn->SetBackgroundColour(wxColour(255, 255, 0));
    signUpBtn->SetForegroundColour(wxColor(0, 0, 0));

    bottomSizer->Add(signUpBtn, 1, wxRIGHT | wxLEFT, 20);
    bottomSizer->Add(logInBtn, 1, wxLEFT | wxRIGHT | wxALIGN_RIGHT, 20);
    bottomPanel->SetSizerAndFit(bottomSizer);

    grid->Add(bottomPanel, 1, wxTOP | wxEXPAND, 20);
    signUpBtn->Bind(wxEVT_BUTTON, LoginPage::switchToSignUp, this);
    logInBtn->Bind(wxEVT_BUTTON, LoginPage::signIn, this);
}

void LoginPage::switchToSignUp(wxCommandEvent &event)
{
    currentPage_ = PAGES::REGISTER;
    event.Skip();
}

void LoginPage::signIn(wxCommandEvent &event)
{
    std::string username = usernameInput->GetValue().ToStdString();
    std::string password = passwordInput->GetValue().ToStdString();
    if (!signInModel.signIn(username, password))
    {
        wxMessageBox("Invalid username or password", "Error");
        return;
    }
    currentPage_ = PAGES::PROFILE;
    frame_->Hide();
    event.Skip();
}

LoginPage::LoginPage(wxFrame *frame, PAGES &currentPage, database::AccountRecord &account, std::shared_ptr<database::DatabaseClient> databaseClient)
    : frame_(frame), currentPage_(currentPage), account_(account), signInModel(account_, databaseClient)
{
    font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
    font.Scale(2);
}

void LoginPage::create()
{
    frame_->SetMinSize(wxSize(0, 0));
    frame_->SetMaxSize(wxSize(1000, 1000));

    wxSize size(Width, Height);
    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    panel = new wxPanel(frame_, wxID_ANY, wxDefaultPosition);

    wxFlexGridSizer *grid = new wxFlexGridSizer(6, 1, 4, 0);
    grid->AddGrowableCol(0);

    createHeader(mainSizer);
    createUsernameInput(grid);
    createPasswordInput(grid);
    createButtons(grid);

    mainSizer->Add(grid, 1, wxALL | wxEXPAND, 10);

    mainSizer->SetMinSize(size);
    panel->SetSizerAndFit(mainSizer);
    panel->SetBackgroundColour(wxColour(0, 0, 0));

    frame_->SetSize(wxDefaultCoord, wxDefaultCoord, Width, Height, wxSIZE_USE_EXISTING);

    frame_->SetMinSize(size);
    frame_->SetMaxSize(size);
    frame_->SetBackgroundColour(wxColour(0, 0, 0));

    frame_->Show(true);
    frame_->Refresh();
}

void LoginPage::close()
{
    delete panel;
}

LoginPage::~LoginPage()
{
}

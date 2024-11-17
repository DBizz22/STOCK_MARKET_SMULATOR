#ifndef LOGINPAGE_HPP_
#define LOGINPAGE_HPP_

#include "coordinator.hpp"

// enumPAGES;

class LoginPage
{
private:
    static const unsigned int Width, Height;
    PAGES &currentPage_;
    wxFrame *frame_;
    wxPanel *panel;
    wxFont font;
    wxTextCtrl *usernameInput, *passwordInput;
    database::AccountRecord &account_;
    std::shared_ptr<database::DatabaseClient> databaseClient_;
    SignInModel signInModel;
    void createHeader(wxBoxSizer *sizer);
    void createUsernameInput(wxFlexGridSizer *grid);
    void createPasswordInput(wxFlexGridSizer *grid);
    void createButtons(wxFlexGridSizer *grid);
    void signIn(wxCommandEvent &event);
    void switchToSignUp(wxCommandEvent &event);

public:
    explicit LoginPage(wxFrame *frame, PAGES &currentPage, database::AccountRecord &account, std::shared_ptr<database::DatabaseClient> databaseClient);
    void create();
    void close();
    ~LoginPage();
};

#endif
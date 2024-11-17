#ifndef REGISTERPAGE_HPP_
#define REGISTERPAGE_HPP_

#include "coordinator.hpp"

class RegisterPage
{
private:
    static const unsigned int Width, Height;
    wxFont font;
    wxFrame *frame_;
    PAGES &currentPage_;
    wxPanel *panel;
    wxTextCtrl *usernameInput, *passwordInput[2];
    SignUpModel signUpModel;
    std::shared_ptr<database::DatabaseClient> databaseClient_;
    void createHeader(wxBoxSizer *sizer);
    void createUsernameInput(wxFlexGridSizer *grid);
    void createPasswordInput(wxFlexGridSizer *grid);
    void createButtons(wxFlexGridSizer *grid);
    void signUp(wxCommandEvent &event);
    void switchToSignIn(wxCommandEvent &event);

public:
    explicit RegisterPage(wxFrame *frame, PAGES &currentPage, std::shared_ptr<database::DatabaseClient> databaseClient);
    void create();
    void close();
    ~RegisterPage();
};

#endif
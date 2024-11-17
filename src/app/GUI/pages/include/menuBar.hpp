#ifndef MENUBAR_HPP_
#define MENUBAR_HPP_

#include <iostream>
#include <memory>
#include <unordered_set>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#define wxUSE_STATTEXT 1
#define wxUSE_LISTCTRL 1
#define wxUSE_STD_STRING 1

#include "wx/sizer.h"
#include "wx/panel.h"
#include "wx/textctrl.h"
#include "wx/stattext.h"
#include "wx/event.h"
#include "wx/listctrl.h"
#include <wx/valnum.h>
#include <wx/menu.h>

#include "coordinator.hpp"
#include "settingsModel.hpp"
#include <cctype>

struct SettingsInfo
{
    bool isActive = false;
    wxFrame *settingsFrame;
    wxStaticText *accountName;
    wxTextCtrl *newNameInput, *password1Input, *password2Input;
};

class MenuBar
{
private:
    wxFrame *frame_;
    PAGES &currentPage_;
    SettingsInfo settings;
    wxMenuBar *menuBar = nullptr;
    wxMenu *menuFile = nullptr;
    database::AccountRecord &account_;
    std::shared_ptr<database::DatabaseClient> databaseClient_;
    std::shared_ptr<SettingsModel> settingsModel_;
    void goToProfilePage(wxCommandEvent &event);
    void openSettingsPage(wxCommandEvent &event);
    void closeSettingsPage();
    void openAboutPage(wxCommandEvent &event);
    void logout(wxCommandEvent &event);

public:
    MenuBar(wxFrame *frame, PAGES &currentPage, database::AccountRecord &account, const std::shared_ptr<database::DatabaseClient> &databaseClient);
    void create();
    void close();
};

#endif
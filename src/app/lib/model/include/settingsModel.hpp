#ifndef SETTINGSMODEL_HPP_
#define SETTINGSMODEL_HPP_

#include <string>
#include <memory>
#include "databaseClient.hpp"
#include "signUpModel.hpp"

class SettingsModel
{
private:
    database::AccountRecord &account_;
    std::shared_ptr<database::DatabaseClient> databaseClient_;

public:
    SettingsModel(database::AccountRecord &account, const std::shared_ptr<database::DatabaseClient> &databaseClient);
    bool validateInput(const std::string &);
    bool changeUserName(const std::string &newUserName);
    bool changePassword(const std::string &newPassword);
};

#endif
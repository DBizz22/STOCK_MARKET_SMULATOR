#include "settingsModel.hpp"

SettingsModel::SettingsModel(database::AccountRecord &account, const std::shared_ptr<database::DatabaseClient> &databaseClient) : account_(account)
{
    databaseClient_ = databaseClient;
}

bool SettingsModel::validateInput(const std::string &input)
{
    if (input.empty())
        return false;

    if (account_.username == input || account_.password == input)
        return false;

    if (input.size() < MIN_LENGTH)
        return false;

    return true;
}

bool SettingsModel::changeUserName(const std::string &newUserName)
{
    std::string oldUserName = account_.username;
    account_.username = newUserName;
    if (!databaseClient_->update(account_))
    {
        account_.username = oldUserName;
        return false;
    }

    return true;
}

bool SettingsModel::changePassword(const std::string &newPassword)
{
    std::string oldPassword = account_.password;
    account_.password = newPassword;
    if (!databaseClient_->update(account_))
    {
        account_.password = oldPassword;
        return false;
    }

    return true;
}
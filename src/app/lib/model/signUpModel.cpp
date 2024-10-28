#include "signUpModel.hpp"

SignUpModel::SignUpModel(const std::shared_ptr<database::DatabaseClient> &database)
{
    databaseClient_ = database;
}

bool SignUpModel::validateInput(const std::string &username, const std::string &password) const
{
    if (username.empty() || password.empty())
        return false;
    if (username.size() < MIN_LENGTH || password.size() < MIN_LENGTH)
        return false;
    return true;
}

bool SignUpModel::registerAccount(const std::string &username, const std::string &password) const
{
    database::AccountRecord account = {0, username, password};
    return databaseClient_->insert(account);
}
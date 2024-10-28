#include "signInModel.hpp"

SignInModel::SignInModel(database::AccountRecord &account, const std::shared_ptr<database::DatabaseClient> &database) : account_(account)
{
    database_ = database;
    attempts = 0;
}

bool SignInModel::signIn(const std::string &username, const std::string &password)
{
    if (attempts >= MAX_ATTEMPTS)
        return false;

    account_ = database_->getAccount(username, password);

    if (account_.ID == 0)
    {
        attempts++;
        return false;
    }

    attempts = 0;
    return true;
}
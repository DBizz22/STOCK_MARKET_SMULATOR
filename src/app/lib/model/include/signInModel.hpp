#ifndef SIGNINMODEL_HPP_
#define SIGNINMODEL_HPP_

#include <iostream>
#include <string>
#include <memory>
#include "databaseClient.hpp"

const int MAX_ATTEMPTS = 5;

class SignInModel
{
private:
    unsigned int attempts;
    database::AccountRecord &account_;
    std::shared_ptr<database::DatabaseClient> database_;

public:
    explicit SignInModel(database::AccountRecord &account, const std::shared_ptr<database::DatabaseClient> &database);
    bool signIn(const std::string &username, const std::string &password);
};

#endif
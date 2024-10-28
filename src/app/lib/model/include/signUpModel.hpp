#ifndef SIGNUPMODEL_HPP_
#define SIGNUPMODEL_HPP_

#include <string>
#include <memory>
#include "databaseClient.hpp"
#define MIN_LENGTH 10

class SignUpModel
{
private:
    std::shared_ptr<database::DatabaseClient> databaseClient_;

public:
    explicit SignUpModel(const std::shared_ptr<database::DatabaseClient> &databaseClient);
    bool validateInput(const std::string &username, const std::string &password) const;
    bool registerAccount(const std::string &username, const std::string &password) const;
};

#endif
#ifndef REFRESHMODEL_HPP_
#define REFRESHMODEL_HPP_

#include <vector>
#include "searchModel.hpp"
#include "databaseClient.hpp"
#include "stockApi.hpp"

class RefreshModel : private SearchModel
{
private:
    unsigned int accountID_;
    std::shared_ptr<database::DatabaseClient> databaseClient_;
    std::vector<database::ProfileRecord> profileRecords_;

public:
    RefreshModel(const unsigned int &accountID, const std::shared_ptr<database::DatabaseClient> &databaseClient, ApiManager &apiManager);
    bool refresh();
};

#endif
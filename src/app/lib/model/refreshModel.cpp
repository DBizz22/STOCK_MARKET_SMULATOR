#include "refreshModel.hpp"

RefreshModel::RefreshModel(const unsigned int &accountID, const std::shared_ptr<database::DatabaseClient> &databaseClient, ApiManager &apiManager)
    : SearchModel(databaseClient, apiManager)
{
    accountID_ = accountID;
    databaseClient_ = databaseClient;
    profileRecords_ = databaseClient_->getProfiles(accountID);
}

bool RefreshModel::refresh()
{
    for (const database::ProfileRecord &profileRecord : profileRecords_)
    {
        std::vector<database::EquityRecord> equityRecords = databaseClient_->getEquities(profileRecord.ID);
        for (const database::EquityRecord &equityRecord : equityRecords)
        {
            database::StockRecord stockRecord = databaseClient_->getStock(equityRecord.stockID);
            if (!SearchModel::findForex(stockRecord.symbol, stockRecord.currency).isEmpty())
                continue;
            if (!SearchModel::findStock(stockRecord.symbol, stockRecord.currency).isEmpty())
                return false;
        }
    }
    return true;
}

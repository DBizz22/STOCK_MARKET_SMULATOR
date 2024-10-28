#include "equityModel.hpp"
#include <numeric>

void EquityData::convert(const database::EquityRecord &equityRecord, const database::StockRecord &stockRecord)
{
    ID = equityRecord.ID;
    symbol = stockRecord.symbol;
    currentValue = stockRecord.currentPrice;
    quantity = equityRecord.quantity;
    totalValue = stockRecord.currentPrice * equityRecord.quantity;
    currency = stockRecord.currency;
    percentageChange = (stockRecord.currentPrice - equityRecord.initialValue) / equityRecord.initialValue * 100;
}

bool EquityModel::availableBalance(const double &price, const double &quantity)
{
    double cost = price * quantity;
    if (cost > profileData_.balance)
        return false;
    return true;
}

void EquityModel::addBalance(const double &gain)
{
    profileData_.balance += gain;
}

void EquityModel::deductBalance(const double &cost)
{
    profileData_.balance -= cost;
}

unsigned int EquityModel::getEquitiesCount(const unsigned int &profileID)
{
    std::vector<database::EquityRecord> equityRecords = databaseClient_->getEquities(profileID);
    return equityRecords.size();
}

double EquityModel::getEquitiesValue(const unsigned int &profileID)
{
    std::vector<database::EquityRecord> equityRecords = databaseClient_->getEquities(profileID);
    double currentValue = 0;
    for (const auto &equityRecord : equityRecords)
    {
        database::StockRecord stock = databaseClient_->getStock(equityRecord.stockID);
        currentValue += (equityRecord.quantity * stock.currentPrice);
    }
    return currentValue;
}

void EquityModel::loadEquityDatas()
{
    EquityData equityData;
    std::vector<database::EquityRecord> equityRecords = databaseClient_->getEquities(profileData_.ID);
    for (auto &equityRecord : equityRecords)
    {
        database::StockRecord stockRecord = databaseClient_->getStock(equityRecord.stockID);
        equityData.convert(equityRecord, stockRecord);
        equityDatas_.emplace_back(equityData);
    }
}

void EquityModel::addEquityData(const database::EquityRecord &equityRecord, const database::StockRecord &stockRecord)
{
    std::vector<database::EquityRecord> equityRecords = databaseClient_->getEquities(profileData_.ID);
    auto equityRecordIter = std::find_if_not(equityRecords.begin(), equityRecords.end(), [&](const auto &equityRecord)
                                             { return std::any_of(equityDatas_.begin(), equityDatas_.end(), [&](const auto &equityData)
                                                                  { return equityData.ID == equityRecord.ID; }); });
    EquityData equityData;
    if (equityRecordIter == equityRecords.end())
        return;
    equityData.convert(*equityRecordIter, stockRecord);
    equityDatas_.emplace_back(equityData);
}

void EquityModel::updateEquityData(const database::EquityRecord &equityRecord, const database::StockRecord &stockRecord)
{
    EquityData equityData;
    equityData.convert(equityRecord, stockRecord);
    auto equityDataIter = std::find_if(equityDatas_.begin(), equityDatas_.end(),
                                       [&equityRecord](const EquityData &data)
                                       { return data.ID == equityRecord.ID; });
    if (equityDataIter != equityDatas_.end())
        equityDataIter->convert(equityRecord, stockRecord);
}

void EquityModel::removeEquityData(const database::EquityRecord &equityRecord, const database::StockRecord &stockRecord)
{
    EquityData equityData;
    equityData.convert(equityRecord, stockRecord);
    auto equityDataIter = std::find_if(equityDatas_.begin(), equityDatas_.end(),
                                       [&equityRecord](const EquityData &data)
                                       { return data.ID == equityRecord.ID; });
    if (equityDataIter != equityDatas_.end())
        equityDatas_.erase(equityDataIter);
}

bool EquityModel::updateProfile(const Mode &mode, const double &price)
{
    database::ProfileRecord profileRecord = databaseClient_->getProfile(profileData_.ID);
    if (profileRecord.isEmpty())
        return false;
    (mode == Mode::Add) ? addBalance(price) : deductBalance(price);
    profileRecord.balance = profileData_.balance;
    if (!databaseClient_->update(profileRecord))
        return false;

    double equitiesValue = std::accumulate(equityDatas_.begin(), equityDatas_.end(), 0.0, [](double accumulator, const EquityData &equityData)
                                           { return accumulator + (equityData.currentValue * equityData.quantity); });

    profileData_.convert(profileRecord, equityDatas_.size(), equitiesValue);
    return true;
}

EquityModel::EquityModel(ProfileData &profileData, std::vector<EquityData> &equityDatas, const std::shared_ptr<database::DatabaseClient> &database)
    : equityDatas_(equityDatas), profileData_(profileData)
{
    databaseClient_ = database;
    loadEquityDatas();
}

bool EquityModel::addEquity(const SearchData &searchData, const double &quantity)
{
    if (equityDatas_.size() >= MAX_EQUITIES)
        return false;
    database::StockRecord stockRecord = databaseClient_->getStock(searchData.symbol, searchData.symbol);
    if (stockRecord.isEmpty())
        return false;
    if (!availableBalance(stockRecord.currentPrice, quantity))
        return false;
    database::EquityRecord equityRecord{0, profileData_.ID, stockRecord.ID, stockRecord.currentPrice, quantity};
    if (!databaseClient_->insert(equityRecord))
        return false;
    addEquityData(equityRecord, stockRecord);
    return updateProfile(Mode::Deduct, stockRecord.currentPrice * quantity);
}

bool EquityModel::buy(const unsigned int &equityID, const double &quantity)
{
    if (quantity == 0)
        return true;
    database::EquityRecord equityRecord = databaseClient_->getEquity(equityID);
    database::StockRecord stockRecord = databaseClient_->getStock(equityRecord.stockID);
    if (equityRecord.isEmpty() || stockRecord.isEmpty())
        return false;

    if (!availableBalance(stockRecord.currentPrice, quantity))
        return false;

    equityRecord.quantity += quantity;
    if (!databaseClient_->update(equityRecord))
        return false;

    updateEquityData(equityRecord, stockRecord);
    return updateProfile(Mode::Deduct, stockRecord.currentPrice * quantity);
}

bool EquityModel::sell(const unsigned int &equityID, const double &quantity)
{
    database::EquityRecord equityRecord = databaseClient_->getEquity(equityID);
    database::StockRecord stockRecord = databaseClient_->getStock(equityRecord.stockID);
    if (equityRecord.isEmpty() || stockRecord.isEmpty() || quantity > equityRecord.quantity)
        return false;

    if (quantity == equityRecord.quantity)
        return removeEquity(equityID);

    equityRecord.quantity -= quantity;
    if (!databaseClient_->update(equityRecord))
        return false;

    updateEquityData(equityRecord, stockRecord);
    return updateProfile(Mode::Add, stockRecord.currentPrice * quantity);
}

bool EquityModel::removeEquity(const unsigned int &equityID)
{
    database::EquityRecord equityRecord = databaseClient_->getEquity(equityID);
    database::StockRecord stockRecord = databaseClient_->getStock(equityRecord.stockID);
    if (equityRecord.isEmpty() || stockRecord.isEmpty())
        return false;

    if (!databaseClient_->drop(equityRecord))
        return false;

    removeEquityData(equityRecord, stockRecord);
    return updateProfile(Mode::Add, stockRecord.currentPrice * equityRecord.quantity);
}
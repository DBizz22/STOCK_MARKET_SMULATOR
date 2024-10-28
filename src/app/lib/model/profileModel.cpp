#include "profileModel.hpp"
#include <algorithm>

void ProfileData::convert(const database::ProfileRecord &profileRecord, const unsigned int &equitiesCount_, const double &equitiesValue_)
{
    ID = profileRecord.ID;
    name = profileRecord.name;
    equitiesCount = equitiesCount_;
    equitiesValue = equitiesValue_;
    balance = profileRecord.balance;
    currency = profileRecord.currency;
    percentageChange = (equitiesValue + profileRecord.balance - profileRecord.initialValue) / profileRecord.initialValue * 100;
}

unsigned int ProfileModel::getEquitiesCount(const unsigned int &profileID)
{
    std::vector<database::EquityRecord> equityRecords = databaseClient_->getEquities(profileID);
    return equityRecords.size();
}

double ProfileModel::getEquitiesValue(const unsigned int &profileID)
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

void ProfileModel::loadProfileDatas()
{
    profileDatas_.clear();
    ProfileData profileData;
    std::vector<database::ProfileRecord> profileRecords = databaseClient_->getProfiles(accountID_);
    for (auto &profileRecord : profileRecords)
    {
        profileData.convert(profileRecord, getEquitiesCount(profileRecord.ID), getEquitiesValue(profileRecord.ID));
        profileDatas_.emplace_back(profileData);
    }
}

void ProfileModel::addProfileData(const std::vector<database::ProfileRecord> &profileRecords)
{
    ProfileData profileData;
    auto profileRecordIter = std::find_if_not(profileRecords.begin(), profileRecords.end(),
                                              [&](const auto &profileRecord)
                                              {
                                                  return std::any_of(profileDatas_.begin(), profileDatas_.end(), [&](const auto &profileData)
                                                                     { return profileData.ID == profileRecord.ID; });
                                              });
    profileData.convert(*profileRecordIter, 0, 0);
    profileDatas_.emplace_back(profileData);
}

ProfileModel::ProfileModel(const unsigned int &accountID, std::vector<ProfileData> &profileDatas, const std::shared_ptr<database::DatabaseClient> &database) : profileDatas_(profileDatas)
{
    accountID_ = accountID;
    databaseClient_ = database;
    loadProfileDatas();
}

bool ProfileModel::validateProfileName(const std::string &name)
{
    auto iter = std::find_if(profileDatas_.begin(), profileDatas_.end(), [&name](const ProfileData &profileData)
                             { return profileData.name == name; });
    if (iter != profileDatas_.end() || name.size() < MIN_NAME_LENGTH)
        return false;
    return true;
}

bool ProfileModel::addProfile(const std::string &name, const double &value, const std::string &currency)
{
    if (profileDatas_.size() >= MAX_PROFILES)
        return false;
    database::ProfileRecord profileRecord{0, accountID_, name, currency, value, value};
    if (!databaseClient_->insert(profileRecord))
        return false;

    std::vector<database::ProfileRecord> profileRecords = databaseClient_->getProfiles(accountID_);
    addProfileData(profileRecords);
    return true;
}

bool ProfileModel::modifyProfileName(const unsigned int &profileID, const std::string &newName)
{
    database::ProfileRecord profileRecord = databaseClient_->getProfile(profileID);
    if (profileRecord.isEmpty())
        return false;
    profileRecord.name = newName;
    if (!databaseClient_->update(profileRecord))
        return false;
    auto profileDataIter = std::find_if(profileDatas_.begin(), profileDatas_.end(), [&profileID](const auto &profileData)
                                        { return profileData.ID == profileID; });
    profileDataIter->name = newName;
    return true;
}

bool ProfileModel::removeProfile(const unsigned int &profileID)
{
    database::ProfileRecord profileRecord = databaseClient_->getProfile(profileID);
    if (profileRecord.isEmpty())
        return false;
    if (!databaseClient_->drop(profileRecord))
        return false;
    auto profileDataIter = std::find_if(profileDatas_.begin(), profileDatas_.end(), [&profileID](const auto &profileData)
                                        { return profileData.ID == profileID; });
    profileDatas_.erase(profileDataIter);
    return true;
}
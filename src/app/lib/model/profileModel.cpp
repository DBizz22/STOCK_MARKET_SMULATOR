#include "profileModel.hpp"
#include <algorithm>

const std::vector<std::string> CurrencyCodes{
    "AED", "AFN", "ALL", "AMD", "ANG", "AOA", "ARS", "AUD", "AWG", "AZN",
    "BAM", "BBD", "BDT", "BGN", "BHD", "BIF", "BMD", "BND", "BOB", "BRL", "BSD", "BTN", "BWP", "BZD",
    "CAD", "CDF", "CHF", "CLF", "CLP", "CNH", "CNY", "COP", "CUP", "CVE", "CZK",
    "DJF", "DKK", "DOP", "DZD",
    "EGP", "ERN", "ETB", "EUR", "FJD", "FKP", "GBP", "GEL", "GHS", "GIP", "GMD", "GNF", "GTQ", "GYD",
    "HKD", "HNL", "HRK", "HTG", "HUF", "ICP", "IDR", "ILS", "INR", "IQD", "IRR", "ISK", "JEP", "JMD", "JOD", "JPY",
    "KES", "KGS", "KHR", "KMF", "KPW", "KRW", "KWD", "KYD", "KZT", "LAK", "LBP", "LKR", "LRD", "LSL", "LYD",
    "MAD", "MDL", "MGA", "MKD", "MMK", "MNT", "MOP", "MRO", "MRU", "MUR", "MVR", "MWK", "MXN", "MYR", "MZN",
    "NAD", "NGN", "NOK", "NPR", "NZD", "OMR", "PAB", "PEN", "PGK", "PHP", "PKR", "PLN", "PYG",
    "QAR", "RON", "RSD", "RUB", "RUR", "RWF", "SAR", "SBDF", "SCR", "SDG", "SDR", "SEK", "SGD", "SHP", "SLL", "SOS", "SRD", "SYP", "SZL",
    "THB", "TJS", "TMT", "TND", "TOP", "TRY", "TTD", "TWD", "TZS", "UAH", "UGX", "USD", "UYU", "UZS",
    "VND", "VUV", "WST", "XAF", "XCD", "XDR", "XOF", "XPF", "YER", "ZAR", "ZMW", "ZWL"};

void ProfileData::convert(const database::ProfileRecord &profileRecord, const unsigned int &equitiesCount_, const double &equitiesValue_)
{
    ID = profileRecord.ID;
    name = profileRecord.name;
    equitiesCount = equitiesCount_;
    equitiesValue = equitiesValue_;
    balance = profileRecord.balance;
    currency = profileRecord.currency;
    profitLoss = equitiesValue + profileRecord.balance - profileRecord.initialValue;
    percentageChange = (equitiesValue + profileRecord.balance - profileRecord.initialValue) / profileRecord.initialValue * 100;
}

double ProfileModel::getEquitiesValue(const std::vector<database::EquityRecord> &equityRecords)
{
    double currentValue = 0;
    for (const database::EquityRecord &equityRecord : equityRecords)
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
    for (const auto &profileRecord : profileRecords)
    {
        std::vector<database::EquityRecord> equityRecords = databaseClient_->getEquities(profileRecord.ID);
        double equitiesValue = getEquitiesValue(equityRecords);
        profileData.convert(profileRecord, equityRecords.size(), equitiesValue);
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

bool ProfileModel::validateCurrency(const std::string &currencyCode)
{
    auto iter = std::find(CurrencyCodes.begin(), CurrencyCodes.end(), currencyCode);
    if (iter == CurrencyCodes.end())
        return false;
    return true;
}

bool ProfileModel::addProfile(const std::string &name, const double &value, const std::string &currencyCode)
{
    if (profileDatas_.size() >= MAX_PROFILES)
        return false;
    database::ProfileRecord profileRecord{0, accountID_, name, currencyCode, value, value};
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
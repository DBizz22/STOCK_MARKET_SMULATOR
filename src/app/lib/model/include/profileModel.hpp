#ifndef PROFILEMODEL_HPP_
#define PROFILEMODEL_HPP_

#include <string>
#include <vector>
#include <memory>
#include "databaseClient.hpp"

const unsigned int MAX_PROFILES = 3;
const unsigned int MIN_NAME_LENGTH = 5;
const double DEFAULT_VALUE = 1000;
const std::string DEFAULT_CURRENCY = "USD";

struct ProfileData
{
    unsigned int ID{0};
    std::string name;
    unsigned int equitiesCount{0};
    double equitiesValue{0};
    double balance{0};
    std::string currency;
    double percentageChange{0};
    void convert(const database::ProfileRecord &profileRecord, const unsigned int &equitiesCount_, const double &equitiesValue_);
};

class ProfileModel
{
private:
    unsigned int accountID_;
    std::shared_ptr<database::DatabaseClient> databaseClient_;
    std::vector<ProfileData> &profileDatas_;
    unsigned int getEquitiesCount(const unsigned int &profileID);
    double getEquitiesValue(const unsigned int &profileID);
    ProfileData getProfileData(const database::ProfileRecord &profileRecord);
    void loadProfileDatas();
    void addProfileData(const std::vector<database::ProfileRecord> &profileRecords);

public:
    explicit ProfileModel(const unsigned int &accountID, std::vector<ProfileData> &profileDatas, const std::shared_ptr<database::DatabaseClient> &databaseClient);
    bool validateProfileName(const std::string &name);
    bool addProfile(const std::string &name, const double &value = DEFAULT_VALUE, const std::string &currency = DEFAULT_CURRENCY);
    bool modifyProfileName(const unsigned int &profileID, const std::string &newName);
    bool removeProfile(const unsigned int &profileID);
};

#endif
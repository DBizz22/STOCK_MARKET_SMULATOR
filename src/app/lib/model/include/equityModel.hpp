#ifndef EQUITYMODEL_HPP_
#define EQUITYMODEL_HPP_

#include <string>
#include <memory>
#include "databaseClient.hpp"
#include "stockApi.hpp"
#include "searchModel.hpp"
#include "profileModel.hpp"

const int MAX_EQUITIES = 10;

struct EquityData
{
    unsigned int ID{0};
    std::string symbol;
    double currentValue{0};
    double quantity{0};
    double totalValue{0};
    std::string currency;
    double percentageChange{0};
    void convert(const database::EquityRecord &equityRecord, const database::StockRecord &stockRecord);
};

class EquityModel
{
private:
    enum class Mode
    {
        Add,
        Deduct
    };
    std::shared_ptr<database::DatabaseClient> databaseClient_;
    std::vector<EquityData> &equityDatas_;
    ProfileData &profileData_;
    unsigned int getEquitiesCount(const unsigned int &profileID);
    double getEquitiesValue(const unsigned int &profileID);
    bool availableBalance(const double &price, const double &quantity);
    void addBalance(const double &);
    void deductBalance(const double &);
    void loadEquityDatas();
    void addEquityData(const database::EquityRecord &equityRecord, const database::StockRecord &stockRecord);
    void updateEquityData(const database::EquityRecord &equityRecord, const database::StockRecord &stockRecord);
    void removeEquityData(const database::EquityRecord &equityRecord, const database::StockRecord &stockRecord);
    bool updateProfile(const Mode &, const double &);

public:
    EquityModel(ProfileData &profileData, std::vector<EquityData> &equityDatas, const std::shared_ptr<database::DatabaseClient> &databaseClient);
    bool addEquity(const SearchData &searchData, const double &quantity);
    bool buy(const unsigned int &equityID, const double &quantity);
    bool sell(const unsigned int &equityID, const double &quantity);
    bool removeEquity(const unsigned int &equityID);
};

#endif
#include "gtest/gtest.h"
#include "equityModel.hpp"
#include "databaseClientMock.cpp"
#include "algorithm"

// TODO: Initilization test
class EquityModelTest : public testing::Test
{
protected:
    double balance;
    static const double ExcessQuantity, Cost;
    static const double Quantity;
    static const double Price;
    static const unsigned int profileID;
    SearchData validSearchedData;
    database::StockRecord stockRecord;
    database::EquityRecord equityRecord;
    database::ProfileRecord profileRecord;
    EquityData equityData;
    std::vector<database::EquityRecord> equityRecords;
    std::vector<EquityData> equityDatas, prevEquityDatas;
    ProfileData profileData, prevProfileData;
    std::shared_ptr<DatabaseClientMock> databaseClientMock;
    std::shared_ptr<EquityModel> equityModel;

public:
    EquityModelTest() {}
    ~EquityModelTest() {}
    static void SetUpTestSuite()
    {
    }
    static void TearDownTestSuite()
    {
    }
    void SetUp() override
    {
        databaseClientMock = std::make_shared<DatabaseClientMock>();
        equityModel = std::make_shared<EquityModel>(profileData, equityDatas, databaseClientMock);
        balance = 100;
        validSearchedData = {"SYM", Price, "USD"};
        stockRecord = {1, "SYM", "USD", Price, "2024-10-01"};
        equityRecord = {MAX_EQUITIES, profileID, 1, Price, Quantity};
        equityData = {MAX_EQUITIES, "SYM", Price, Quantity, Quantity * Price, "USD", 0};
        double equitiesValue = 0;
        for (unsigned int i = 1; i < MAX_EQUITIES; i++)
        {
            equityDatas.emplace_back(EquityData{i, "SYM", Price, Quantity, Price * Quantity, "USD", 0});
            equityRecords.emplace_back(database::EquityRecord{i, profileID, i, Price, Quantity});
            equitiesValue += Price * Quantity;
        }
        prevEquityDatas = equityDatas;
        // EXPECT_CALL(*databaseClientMock, getEquities(profileID)).Times(1).WillOnce(testing::Return(equityRecords));
        profileData = {profileID, "Profile1", MAX_EQUITIES - 1, equitiesValue, balance, "USD", 0};
        prevProfileData = profileData;
        profileRecord = {profileID, 1, "Profile1", "USD", equitiesValue, balance};
    }
    void TearDown() override
    {
    }
};

const unsigned int EquityModelTest::profileID = 1;
const double EquityModelTest::ExcessQuantity = 20;
const double EquityModelTest::Quantity = 5;
const double EquityModelTest::Price = 10;
const double EquityModelTest::Cost = Price * Quantity;

static bool EqualProfileData(const ProfileData &lhs, const ProfileData &rhs)
{
    return lhs.ID == rhs.ID && lhs.name == rhs.name && lhs.equitiesCount == rhs.equitiesCount && lhs.equitiesValue == rhs.equitiesValue && lhs.balance == rhs.balance && lhs.currency == rhs.currency && lhs.percentageChange == rhs.percentageChange;
}

static bool EqualEquityData(const EquityData &lhs, const EquityData &rhs)
{
    return lhs.ID == rhs.ID && lhs.symbol == rhs.symbol && lhs.currentValue == rhs.currentValue && lhs.quantity == rhs.quantity && lhs.totalValue == rhs.totalValue && lhs.currency == rhs.currency && lhs.percentageChange == rhs.percentageChange;
}

static bool EqualEquitiesDatas(const std::vector<EquityData> &lhs, const std::vector<EquityData> &rhs)
{
    if (lhs.size() != rhs.size())
        return false;
    for (size_t i = 0; i < lhs.size(); i++)
    {
        if (!EqualEquityData(lhs[i], rhs[i]))
            return false;
    }
    return true;
}

TEST_F(EquityModelTest, AddEquityInvalidStockRecordFailure)
{
    SearchData emptySearchedData;
    EXPECT_CALL(*databaseClientMock, getStock(testing::_, testing::_)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_FALSE(equityModel->addEquity(emptySearchedData, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, AddEquityInsufficientBalanceFailure)
{
    EXPECT_CALL(*databaseClientMock, getStock(testing::_, testing::_)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_FALSE(equityModel->addEquity(validSearchedData, ExcessQuantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, AddEquityInsertEquityRecordFailure)
{
    database::EquityRecord newEquityRecord = equityRecord;
    newEquityRecord.ID = 0;
    EXPECT_CALL(*databaseClientMock, getStock(testing::_, testing::_)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, insert(newEquityRecord)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(equityModel->addEquity(validSearchedData, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, AddEquityGetEquitiesFailure)
{
    std::vector<EquityData> emptyEquityDatas(MAX_EQUITIES - 1);
    database::EquityRecord newEquityRecord = equityRecord;
    newEquityRecord.ID = 0;
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    modifiedProfileRecord.balance -= Cost;
    EXPECT_CALL(*databaseClientMock, getStock(testing::_, testing::_)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, insert(newEquityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getEquities(profileID)).Times(1).WillOnce(testing::Return(std::vector<database::EquityRecord>()));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(database::ProfileRecord{}));
    EXPECT_FALSE(equityModel->addEquity(validSearchedData, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, AddEquityGetProfileFailure)
{
    std::vector<EquityData> emptyEquityDatas(MAX_EQUITIES - 1);
    database::EquityRecord newEquityRecord = equityRecord;
    newEquityRecord.ID = 0;
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    prevEquityDatas.emplace_back(equityData);
    modifiedProfileRecord.balance -= Cost;
    EXPECT_CALL(*databaseClientMock, getStock(testing::_, testing::_)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, insert(newEquityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getEquities(profileID)).Times(1).WillOnce(testing::InvokeWithoutArgs([&]
                                                                                                          { equityRecords.emplace_back(equityRecord); 
                                                                                                          return equityRecords; }));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(database::ProfileRecord{}));
    EXPECT_FALSE(equityModel->addEquity(validSearchedData, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, AddEquityUpdateProfileFailure)
{
    std::vector<EquityData> emptyEquityDatas(MAX_EQUITIES - 1);
    database::EquityRecord newEquityRecord = equityRecord;
    newEquityRecord.ID = 0;
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    modifiedProfileRecord.balance -= Cost;
    prevProfileData.balance = modifiedProfileRecord.balance;
    prevEquityDatas.emplace_back(equityData);
    EXPECT_CALL(*databaseClientMock, getStock(testing::_, testing::_)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, insert(newEquityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getEquities(profileID)).Times(1).WillOnce(testing::InvokeWithoutArgs([&]
                                                                                                          { equityRecords.emplace_back(equityRecord); 
                                                                                                          return equityRecords; }));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(profileRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(equityModel->addEquity(validSearchedData, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, AddEquitySuccess)
{
    database::EquityRecord newEquityRecord = equityRecord;
    newEquityRecord.ID = 0;
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    modifiedProfileRecord.balance -= Cost;
    prevProfileData.balance = modifiedProfileRecord.balance;
    prevEquityDatas.emplace_back(equityData);
    prevProfileData.equitiesValue += Cost;
    prevProfileData.equitiesCount++;
    prevProfileData.percentageChange = (prevProfileData.balance + profileRecord.initialValue + (Price * Quantity) - profileRecord.initialValue) / profileRecord.initialValue * 100;
    EXPECT_CALL(*databaseClientMock, getStock(testing::_, testing::_)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, insert(newEquityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getEquities(profileID)).Times(1).WillOnce(testing::InvokeWithoutArgs([&]
                                                                                                          { equityRecords.emplace_back(equityRecord); 
                                                                                                          return equityRecords; }));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(profileRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(equityModel->addEquity(validSearchedData, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, CanNotAddAfterMaxEquities)
{
    database::EquityRecord newEquityRecord = equityRecord;
    newEquityRecord.ID = 0;
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    modifiedProfileRecord.balance -= Cost;
    prevProfileData.balance = modifiedProfileRecord.balance;
    prevEquityDatas.emplace_back(equityData);
    prevProfileData.equitiesValue += Cost;
    prevProfileData.equitiesCount++;
    prevProfileData.percentageChange = (prevProfileData.balance + profileRecord.initialValue + (Price * Quantity) - profileRecord.initialValue) / profileRecord.initialValue * 100;
    EXPECT_CALL(*databaseClientMock, getStock(testing::_, testing::_)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, insert(newEquityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getEquities(profileID)).Times(1).WillOnce(testing::InvokeWithoutArgs([&]
                                                                                                          { equityRecords.emplace_back(equityRecord); 
                                                                                                          return equityRecords; }));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(profileRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(equityModel->addEquity(validSearchedData, Quantity));
    for (int i = 0; i < 3; i++)
        EXPECT_FALSE(equityModel->addEquity(SearchData{"EXTRA", 1, "NGN"}, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, BuyQuantityZeroQuantityFailure)
{
    EXPECT_TRUE(equityModel->buy(1, 0));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}
TEST_F(EquityModelTest, BuyQuantityGetEquityRecordFailure)
{
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(database::EquityRecord{}));
    EXPECT_CALL(*databaseClientMock, getStock(0)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_FALSE(equityModel->buy(1, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, BuyQuantityGetStockRecordFailure)
{
    equityRecord.ID = 1;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_FALSE(equityModel->buy(1, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, BuyQuantityInsufficientBalanceFailure)
{
    equityRecord.ID = 1;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_FALSE(equityModel->buy(1, ExcessQuantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, BuyQuantityUpdateEquityRecordFailure)
{
    equityRecord.ID = 1;
    database::EquityRecord modifiedEquityRecord = equityRecord;
    modifiedEquityRecord.quantity += Quantity;
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedEquityRecord)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(equityModel->buy(1, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, BuyQuantityGetProfileRecordFailure)
{
    equityRecord.ID = 1;
    database::EquityRecord modifiedEquityRecord = equityRecord;
    modifiedEquityRecord.quantity += Quantity;
    prevEquityDatas[0].quantity += Quantity;
    prevEquityDatas[0].totalValue = prevEquityDatas[0].quantity * prevEquityDatas[0].currentValue;
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedEquityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(database::ProfileRecord{}));
    EXPECT_FALSE(equityModel->buy(1, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, BuyQuantityUpdateProfileRecordFailure)
{
    equityRecord.ID = 1;
    database::EquityRecord modifiedEquityRecord = equityRecord;
    modifiedEquityRecord.quantity += Quantity;
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    prevProfileData.balance = modifiedProfileRecord.balance -= Cost;
    prevEquityDatas[0].quantity += Quantity;
    prevEquityDatas[0].totalValue = prevEquityDatas[0].quantity * prevEquityDatas[0].currentValue;
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedEquityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(profileRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(equityModel->buy(1, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, BuyQuantitySuccess)
{
    equityRecord.ID = 1;
    database::EquityRecord modifiedEquityRecord = equityRecord;
    modifiedEquityRecord.quantity += Quantity;
    prevEquityDatas[0].quantity += Quantity;
    prevEquityDatas[0].totalValue = prevEquityDatas[0].quantity * prevEquityDatas[0].currentValue;
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    prevProfileData.balance = modifiedProfileRecord.balance -= Cost;
    prevProfileData.equitiesValue += Cost;
    prevProfileData.percentageChange = (prevProfileData.balance + prevProfileData.equitiesValue - profileRecord.initialValue) / profileRecord.initialValue * 100;
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedEquityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(profileRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(equityModel->buy(1, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, SellQuantityGetEquitiesFailure)
{
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(database::EquityRecord{}));
    EXPECT_CALL(*databaseClientMock, getStock(0)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_FALSE(equityModel->sell(1, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, SellQuantityGetStockFailure)
{
    equityRecord.ID = 1;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_FALSE(equityModel->sell(1, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, SellQuantityExcessQuantityFailure)
{
    equityRecord.ID = 1;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_FALSE(equityModel->sell(1, ExcessQuantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, SellQuantityUpdateEquityRecordFailure)
{
    equityRecord.ID = 1;
    double soldQuantity = Quantity - 2;
    database::EquityRecord modifiedEquityRecord = equityRecord;
    modifiedEquityRecord.quantity -= soldQuantity;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedEquityRecord)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(equityModel->sell(1, soldQuantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, SellQuantityGetProfileRecordFailure)
{
    equityRecord.ID = 1;
    double soldQuantity = Quantity - 2;
    database::EquityRecord modifiedEquityRecord = equityRecord;
    prevEquityDatas[0].quantity = modifiedEquityRecord.quantity -= soldQuantity;
    prevEquityDatas[0].totalValue = prevEquityDatas[0].quantity * prevEquityDatas[0].currentValue;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedEquityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(database::ProfileRecord{}));
    EXPECT_FALSE(equityModel->sell(1, soldQuantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, SellQuantityUpdateProfileRecordFailure)
{
    equityRecord.ID = 1;
    double soldQuantity = Quantity - 2;
    database::EquityRecord modifiedEquityRecord = equityRecord;
    prevEquityDatas[0].quantity = modifiedEquityRecord.quantity -= soldQuantity;
    prevEquityDatas[0].totalValue = prevEquityDatas[0].quantity * prevEquityDatas[0].currentValue;
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    prevProfileData.balance = modifiedProfileRecord.balance += (soldQuantity * Price);
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedEquityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(profileRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(equityModel->sell(1, soldQuantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, SellQuantitySuccess)
{
    equityRecord.ID = 1;
    double soldQuantity = Quantity - 2;
    database::EquityRecord modifiedEquityRecord = equityRecord;
    prevEquityDatas[0].quantity = modifiedEquityRecord.quantity -= soldQuantity;
    prevEquityDatas[0].totalValue = prevEquityDatas[0].quantity * prevEquityDatas[0].currentValue;
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    prevProfileData.balance = modifiedProfileRecord.balance += (soldQuantity * Price);
    prevProfileData.equitiesValue = prevProfileData.equitiesValue - (soldQuantity * Price);
    prevProfileData.percentageChange = (prevProfileData.balance + prevProfileData.equitiesValue - profileRecord.initialValue) / profileRecord.initialValue * 100;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedEquityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(profileRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(equityModel->sell(1, soldQuantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, RemoveQuantityGetEquitiesFailure)
{
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(database::EquityRecord{}));
    EXPECT_CALL(*databaseClientMock, getStock(0)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_FALSE(equityModel->removeEquity(1));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, RemoveQuantityGetStockFailure)
{
    equityRecord.ID = 1;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(database::StockRecord{}));
    EXPECT_FALSE(equityModel->removeEquity(1));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, RemoveQuantityDropEquityRecordFailure)
{
    equityRecord.ID = 1;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, drop(equityRecord)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(equityModel->removeEquity(1));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, RemoveQuantityGetProfileRecordFailure)
{
    equityRecord.ID = 1;
    prevEquityDatas.erase(prevEquityDatas.begin());
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, drop(equityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(database::ProfileRecord{}));
    EXPECT_FALSE(equityModel->removeEquity(1));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, RemoveQuantityUpdateProfileRecordFailure)
{
    equityRecord.ID = 1;
    prevEquityDatas.erase(prevEquityDatas.begin());
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    prevProfileData.balance = modifiedProfileRecord.balance += Cost;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, drop(equityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(profileRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(equityModel->removeEquity(1));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, RemoveQuantitySuccess)
{
    equityRecord.ID = 1;
    prevEquityDatas.erase(prevEquityDatas.begin());
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    prevProfileData.balance = modifiedProfileRecord.balance += Cost;
    prevProfileData.equitiesCount--;
    prevProfileData.equitiesValue = prevProfileData.equitiesValue - Cost;
    prevProfileData.percentageChange = (prevProfileData.balance + prevProfileData.equitiesValue - profileRecord.initialValue) / profileRecord.initialValue * 100;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(1).WillOnce(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(1).WillOnce(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, drop(equityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(profileRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(equityModel->removeEquity(1));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

TEST_F(EquityModelTest, SellQuantityAllQuantitySuccess)
{
    equityRecord.ID = 1;
    prevEquityDatas.erase(prevEquityDatas.begin());
    database::ProfileRecord modifiedProfileRecord = profileRecord;
    prevProfileData.balance = modifiedProfileRecord.balance += Cost;
    prevProfileData.equitiesCount--;
    prevProfileData.equitiesValue = prevProfileData.equitiesValue - Cost;
    prevProfileData.percentageChange = (prevProfileData.balance + prevProfileData.equitiesValue - profileRecord.initialValue) / profileRecord.initialValue * 100;
    EXPECT_CALL(*databaseClientMock, getEquity(1)).Times(2).WillRepeatedly(testing::Return(equityRecord));
    EXPECT_CALL(*databaseClientMock, getStock(1)).Times(2).WillRepeatedly(testing::Return(stockRecord));
    EXPECT_CALL(*databaseClientMock, drop(equityRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfile(profileID)).Times(1).WillOnce(testing::Return(profileRecord));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(equityModel->sell(1, Quantity));
    EXPECT_TRUE(EqualEquitiesDatas(prevEquityDatas, equityDatas));
    EXPECT_TRUE(EqualProfileData(prevProfileData, profileData));
}

#include "gtest/gtest.h"
#include "profileModel.hpp"
#include "databaseClientMock.cpp"
#include "algorithm"

// TODO: Initilization test
class ProfileModelTest : public testing::Test
{
protected:
    static const unsigned int accountID;
    static const unsigned int EquitiesCount;
    static const double Quantity, Value;
    static const std::string validProfileName;
    static const database::StockRecord stockRecord, OGstockRecord;
    static const database::ProfileRecord profileRecord;
    static const ProfileData profileData;
    std::vector<database::EquityRecord> equityRecords;
    std::vector<database::ProfileRecord> profileRecords;
    std::vector<ProfileData> profileDatas, prevProfileDatas;
    std::shared_ptr<DatabaseClientMock> databaseClientMock;
    std::shared_ptr<ProfileModel> profileModel;

public:
    ProfileModelTest()
    {
    }
    ~ProfileModelTest()
    {
    }
    static void SetUpTestSuite()
    {
    }
    static void TearDownTestSuite()
    {
    }
    void SetUp() override
    {
        databaseClientMock = std::make_shared<DatabaseClientMock>();
        profileModel = std::make_shared<ProfileModel>(accountID, profileDatas, databaseClientMock);
        for (unsigned int i = 1; i < MAX_PROFILES; i++)
        {
            profileDatas.emplace_back(ProfileData{i, "Profile " + std::to_string(i), i, 1000, 1000, "USD", 0});
            profileRecords.emplace_back(database::ProfileRecord{i, accountID, "Profile " + std::to_string(i), "USD", 1000, 1000});
            equityRecords.emplace_back(database::EquityRecord{i, MAX_PROFILES, 1, Value, Quantity});
        }
        prevProfileDatas = profileDatas;
    }
    void TearDown() override
    {
    }
};

const unsigned int ProfileModelTest::accountID = 1;
const unsigned int ProfileModelTest::EquitiesCount = MAX_PROFILES;
const double ProfileModelTest::Quantity = 10;
const double ProfileModelTest::Value = 60;
const std::string ProfileModelTest::validProfileName = "Profile 3";
const database::StockRecord ProfileModelTest::stockRecord{1, "SYM", "USD", Value};
const database::ProfileRecord ProfileModelTest::profileRecord{MAX_PROFILES, accountID, validProfileName, "USD", 1000, 1000};
const ProfileData ProfileModelTest::profileData{MAX_PROFILES, validProfileName, EquitiesCount, Value *Quantity *(MAX_PROFILES - 1), Value *Quantity *(MAX_PROFILES - 1), "USD", 0};

static bool FindProfileData(const std::vector<ProfileData> &profileDatas, const ProfileData &profileData)
{
    for (auto &profile : profileDatas)
    {
        if (profile.ID == profileData.ID && profile.name == profileData.name && profile.equitiesCount == profileData.equitiesCount && profile.equitiesValue == profileData.equitiesValue && profile.balance == profileData.balance && profile.currency == profileData.currency && profile.percentageChange == profileData.percentageChange)
            return true;
    }
    return false;
}

static bool EqualProfileData(const ProfileData &lhs, const ProfileData &rhs)
{
    return lhs.ID == rhs.ID && lhs.name == rhs.name && lhs.equitiesCount == rhs.equitiesCount && lhs.equitiesValue == rhs.equitiesValue && lhs.balance == rhs.balance && lhs.currency == rhs.currency && lhs.percentageChange == rhs.percentageChange;
}

static bool EqualProfileDatas(const std::vector<ProfileData> &prevProfileDatas, const std::vector<ProfileData> &profileDatas)
{
    if (prevProfileDatas.size() != profileDatas.size())
        return false;
    for (int i = 0; i < profileDatas.size(); i++)
    {
        if (!EqualProfileData(prevProfileDatas[i], profileDatas[i]))
            return false;
    }
    return true;
}

TEST_F(ProfileModelTest, InvalidateDuplicateProfileName)
{
    const std::string duplicateProfileName = "Profile 1";
    EXPECT_FALSE(profileModel->validateProfileName(duplicateProfileName));
}

TEST_F(ProfileModelTest, InvalidateEmptyProfileName)
{
    const std::string emptyProfileName;
    EXPECT_FALSE(profileModel->validateProfileName(emptyProfileName));
}

TEST_F(ProfileModelTest, InvalidateInadequateProfileNameSize)
{
    const std::string invalidProfileName(MIN_NAME_LENGTH - 1, 'a');
    EXPECT_FALSE(profileModel->validateProfileName(invalidProfileName));
}

TEST_F(ProfileModelTest, ValidateProfileName)
{
    EXPECT_TRUE(profileModel->validateProfileName(validProfileName));
}

TEST_F(ProfileModelTest, AddProfileInsertProfileRecordFailure)
{
    database::ProfileRecord newProfileRecord = profileRecord;
    newProfileRecord.ID = 0;
    EXPECT_CALL(*databaseClientMock, insert(newProfileRecord)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(profileModel->addProfile(validProfileName));
    EXPECT_TRUE(EqualProfileDatas(prevProfileDatas, profileDatas));
}

TEST_F(ProfileModelTest, AddProfileRecordSuccess)
{
    database::ProfileRecord newProfileRecord = profileRecord;
    newProfileRecord.ID = 0;
    EXPECT_CALL(*databaseClientMock, insert(newProfileRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfiles(accountID)).Times(1).WillOnce(testing::InvokeWithoutArgs([&]()
                                                                                                          {profileRecords.emplace_back(profileRecord);
                                                                                                            return profileRecords; }));
    EXPECT_TRUE(profileModel->addProfile(validProfileName));
    EXPECT_TRUE(FindProfileData(profileDatas, ProfileData{MAX_PROFILES, validProfileName, 0, 0, 1000, "USD", 0}));
    EXPECT_EQ(prevProfileDatas.size() + 1, profileDatas.size());
}

TEST_F(ProfileModelTest, ModifyProfileNameGetProfileRecordFailure)
{
    EXPECT_CALL(*databaseClientMock, getProfile(1)).Times(1).WillOnce(testing::Return(database::ProfileRecord{}));
    EXPECT_FALSE(profileModel->modifyProfileName(1, "New_Name"));
    EXPECT_TRUE(EqualProfileDatas(prevProfileDatas, profileDatas));
}

TEST_F(ProfileModelTest, ModifyProfileNameUpdateProfileRecordsFailure)
{
    database::ProfileRecord modifiedProfileRecord = profileRecords[0];
    modifiedProfileRecord.name = "New_Name";
    EXPECT_CALL(*databaseClientMock, getProfile(1)).Times(1).WillOnce(testing::Return(profileRecords[0]));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(profileModel->modifyProfileName(1, "New_Name"));
    EXPECT_TRUE(EqualProfileDatas(prevProfileDatas, profileDatas));
}

TEST_F(ProfileModelTest, ModifyProfileName)
{
    std::string newName("New_Name");
    database::ProfileRecord modifiedProfileRecord = profileRecords[0];
    modifiedProfileRecord.name = newName;
    EXPECT_CALL(*databaseClientMock, getProfile(1)).Times(1).WillOnce(testing::Return(profileRecords[0]));
    EXPECT_CALL(*databaseClientMock, update(modifiedProfileRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(profileModel->modifyProfileName(1, newName));
    EXPECT_EQ(profileDatas[0].name, newName);
    EXPECT_EQ(prevProfileDatas.size(), profileDatas.size());
}

TEST_F(ProfileModelTest, RemoveProfileGetProfileRecordFailure)
{
    database::ProfileRecord invalidProfileRecord;
    EXPECT_CALL(*databaseClientMock, getProfile(1)).Times(1).WillOnce(testing::Return(database::ProfileRecord{}));
    EXPECT_FALSE(profileModel->removeProfile(1));
    EXPECT_TRUE(EqualProfileDatas(prevProfileDatas, profileDatas));
}

TEST_F(ProfileModelTest, RemoveProfileDropProfileRecordFailure)
{
    database::ProfileRecord invalidProfileRecord;
    EXPECT_CALL(*databaseClientMock, getProfile(1)).Times(1).WillOnce(testing::Return(profileRecords[0]));
    EXPECT_CALL(*databaseClientMock, drop(profileRecords[0])).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(profileModel->removeProfile(1));
    EXPECT_TRUE(EqualProfileDatas(prevProfileDatas, profileDatas));
}

TEST_F(ProfileModelTest, RemoveProfileSuccess)
{
    prevProfileDatas.erase(prevProfileDatas.begin());
    EXPECT_CALL(*databaseClientMock, getProfile(1)).Times(1).WillOnce(testing::Return(profileRecords[0]));
    EXPECT_CALL(*databaseClientMock, drop(profileRecords[0])).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(profileModel->removeProfile(1));
    EXPECT_TRUE(EqualProfileDatas(prevProfileDatas, profileDatas));
}

TEST_F(ProfileModelTest, CanNotAddAfterMaxProfiles)
{
    database::ProfileRecord newProfileRecord = profileRecord;
    newProfileRecord.ID = 0;
    EXPECT_CALL(*databaseClientMock, insert(newProfileRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(*databaseClientMock, getProfiles(accountID)).Times(1).WillOnce(testing::InvokeWithoutArgs([&]()
                                                                                                          {profileRecords.emplace_back(profileRecord);
                                                                                                            return profileRecords; }));
    EXPECT_TRUE(profileModel->addProfile(validProfileName));
    EXPECT_TRUE(FindProfileData(profileDatas, ProfileData{MAX_PROFILES, validProfileName, 0, 0, 1000, "USD", 0}));
    EXPECT_EQ(prevProfileDatas.size() + 1, profileDatas.size());
    for (int i = MAX_PROFILES; i < MAX_PROFILES + 3; i++)
        EXPECT_FALSE(profileModel->addProfile("Profile " + std::to_string(i)));
}
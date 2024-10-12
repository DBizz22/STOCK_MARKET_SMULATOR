#include <iostream>
#include "gtest/gtest.h"
#include "mySqlClient.hpp"

class MySqlClientTest : public testing::Test
{
protected:
    static database::mysql::connectionParams validCredentials;
    static database::mysql::connectionParams invalidCredentials;
    database::mysql::MySQLClient *mySqlSample;

public:
    MySqlClientTest()
    {
    }
    ~MySqlClientTest()
    {
    }

    static void SetUpTestSuite()
    {
        validCredentials = {"testdb", "root", "enyiudo12real"};
        invalidCredentials = {"invalid", "invalid", "invalid"};
    }
    static void TearDownTestSuite()
    {
    }
    void SetUp() override
    {
        mySqlSample = new database::mysql::MySQLClient(validCredentials);
    }
    void TearDown() override
    {
        delete mySqlSample;
    }
};

database::mysql::connectionParams MySqlClientTest::validCredentials;
database::mysql::connectionParams MySqlClientTest::invalidCredentials;

TEST_F(MySqlClientTest, InvalidDatabaseConnectionCredential)
{
    const database::mysql::connectionParams invalidDatabaseCredential = {"invalid", "root", "enyiudo12real"};
    database::mysql::MySQLClient invalidClient(invalidDatabaseCredential);
    EXPECT_FALSE(invalidClient.is_connected());
}

TEST_F(MySqlClientTest, InvalidUserConnectionCredentials)
{
    const database::mysql::connectionParams invalidUserCredential = {"testdb", "invalid", "enyiudo12real"};
    database::mysql::MySQLClient invalidClient(invalidUserCredential);
    EXPECT_FALSE(invalidClient.is_connected());
}

TEST_F(MySqlClientTest, InvalidPasswordConnectionCredentials)
{
    const database::mysql::connectionParams invalidPasswordCredential = {"testdb", "root", "invalid"};
    database::mysql::MySQLClient invalidClient(invalidPasswordCredential);
    EXPECT_FALSE(invalidClient.is_connected());
}

TEST_F(MySqlClientTest, InvalidAllConnectionCredentials)
{
    database::mysql::MySQLClient invalidClient(invalidCredentials);
    EXPECT_FALSE(invalidClient.is_connected());
}

TEST_F(MySqlClientTest, ValidConnectionCredentials)
{
    EXPECT_TRUE(mySqlSample->is_connected());
}

TEST_F(MySqlClientTest, InvalidConnectionCredentialReset)
{
    mySqlSample->reset(invalidCredentials);
    EXPECT_FALSE(mySqlSample->is_connected());
}

TEST_F(MySqlClientTest, ValidConnectionCredentialReset)
{
    mySqlSample->reset(validCredentials);
    EXPECT_TRUE(mySqlSample->is_connected());
}

TEST_F(MySqlClientTest, ResetAfterNoConnection)
{
    database::mysql::MySQLClient invalidClient(invalidCredentials);
    invalidClient.reset(validCredentials);
    EXPECT_TRUE(invalidClient.is_connected());
}

TEST_F(MySqlClientTest, Disconnect)
{
    mySqlSample->disconnect();
    EXPECT_FALSE(mySqlSample->is_connected());
}

TEST_F(MySqlClientTest, Reconnect)
{
    mySqlSample->disconnect();
    mySqlSample->reconnect();
    EXPECT_TRUE(mySqlSample->is_connected());
}

TEST_F(MySqlClientTest, InsertUniqueAccountRecord)
{
    database::AccountRecord account = {0, "username", "password"};
    EXPECT_TRUE(mySqlSample->insert(account));
}

TEST_F(MySqlClientTest, GetInvalidAccountRecord)
{
    database::AccountRecord account = mySqlSample->getAccount("invalid_username", "invalid_password");
    EXPECT_FALSE(account.ID > 0);
}

TEST_F(MySqlClientTest, GetValidAccountRecord)
{
    database::AccountRecord account = mySqlSample->getAccount("username", "password");
    EXPECT_TRUE(account.ID > 0);
}

TEST_F(MySqlClientTest, UpdateInvalidAccountRecord)
{
    database::AccountRecord account = {0, "username", "password"};
    EXPECT_TRUE(mySqlSample->update(account));
}

TEST_F(MySqlClientTest, UpdateValidAccountRecord)
{
    database::AccountRecord account = mySqlSample->getAccount("username", "password");
    account.username = "new_username";
    EXPECT_TRUE(mySqlSample->update(account));
    database::AccountRecord modifiedAccount = mySqlSample->getAccount("new_username", "password");
    EXPECT_TRUE(account == modifiedAccount);
}

TEST_F(MySqlClientTest, InsertProfileRecord)
{
    database::AccountRecord account = mySqlSample->getAccount("new_username", "password");
    database::ProfileRecord profile = {0, account.ID, "Profile1", "USD", 1000};
    EXPECT_TRUE(mySqlSample->insert(profile));
}

TEST_F(MySqlClientTest, GetInvalidProfileRecords)
{
    database::AccountRecord account = {0, "invalid_username", "invalid_password"};
    std::vector<database::ProfileRecord> profiles = mySqlSample->getProfiles(account.ID);
    EXPECT_TRUE(profiles.empty());
}

TEST_F(MySqlClientTest, GetValidProfileRecords)
{
    database::AccountRecord account = mySqlSample->getAccount("new_username", "password");
    std::vector<database::ProfileRecord> profiles = mySqlSample->getProfiles(account.ID);
    EXPECT_FALSE(profiles.empty());
}

TEST_F(MySqlClientTest, UpdateInvalidProfileRecord)
{
    database::ProfileRecord invalidProfile = {0, 0, "invalid_profile", "USD", 1000};
    EXPECT_TRUE(mySqlSample->update(invalidProfile));
}

TEST_F(MySqlClientTest, UpdateValidProfileRecord)
{
    database::AccountRecord account = mySqlSample->getAccount("new_username", "password");
    std::vector<database::ProfileRecord> profiles = mySqlSample->getProfiles(account.ID);
    profiles[0].name = "new_profile_name";
    EXPECT_TRUE(mySqlSample->update(profiles[0]));
    std::vector<database::ProfileRecord> modifiedProfiles = mySqlSample->getProfiles(account.ID);
    EXPECT_TRUE(profiles[0] == modifiedProfiles[0]);
}

TEST_F(MySqlClientTest, InsertStockRecord)
{
    database::StockRecord stock = {0, "new_stock", "STC", "USD", 1000, "2024-09-16"};
    EXPECT_TRUE(mySqlSample->insert(stock));
}

TEST_F(MySqlClientTest, GetInvalidStockRecord)
{
    database::StockRecord stock = mySqlSample->getStock("invalid_stock", "invalid_currency");
    EXPECT_TRUE(stock.isEmpty());
}

TEST_F(MySqlClientTest, GetValidStockRecord)
{
    database::StockRecord stock = mySqlSample->getStock("STC", "USD");
    EXPECT_FALSE(stock.isEmpty());
}

TEST_F(MySqlClientTest, UpdateInvalidStockRecord)
{
    database::StockRecord invalidStock;
    EXPECT_TRUE(mySqlSample->update(invalidStock));
}

TEST_F(MySqlClientTest, UpdateValidStockRecord)
{
    database::StockRecord stock = mySqlSample->getStock("STC", "USD");
    stock.currentPrice = 100;
    EXPECT_TRUE(mySqlSample->update(stock));
    database::StockRecord modifiedStock = mySqlSample->getStock("STC", "USD");
    EXPECT_TRUE(stock == modifiedStock);
}

TEST_F(MySqlClientTest, InsertInvalidEquitiesRecord)
{
    database::EquityRecord record;
    EXPECT_FALSE(mySqlSample->insert(record));
}

TEST_F(MySqlClientTest, InsertValidEquitiesRecord)
{
    database::AccountRecord account = mySqlSample->getAccount("new_username", "password");
    std::vector<database::ProfileRecord> profiles = mySqlSample->getProfiles(account.ID);
    database::StockRecord stock = mySqlSample->getStock("STC", "USD");
    database::EquityRecord equity = {0, profiles[0].ID, stock.ID, 100, 20};
    EXPECT_TRUE(mySqlSample->insert(equity));
}

TEST_F(MySqlClientTest, GetInvalidEquitiesRecords)
{
    database::ProfileRecord profile;
    std::vector<database::EquityRecord> equities = mySqlSample->getEquities(profile.ID);
    EXPECT_TRUE(equities.empty());
}

TEST_F(MySqlClientTest, GetValidEquitiesRecords)
{
    database::AccountRecord account = mySqlSample->getAccount("new_username", "password");
    std::vector<database::ProfileRecord> profiles = mySqlSample->getProfiles(account.ID);
    std::vector<database::EquityRecord> equities = mySqlSample->getEquities(profiles[0].ID);
    EXPECT_FALSE(equities.empty());
}

TEST_F(MySqlClientTest, UpdateInvalidEquitiesRecord)
{
    database::EquityRecord invalidEquity;
    EXPECT_TRUE(mySqlSample->update(invalidEquity));
}

TEST_F(MySqlClientTest, UpdateValidEquitiesRecord)
{
    database::AccountRecord account = mySqlSample->getAccount("new_username", "password");
    std::vector<database::ProfileRecord> profiles = mySqlSample->getProfiles(account.ID);
    std::vector<database::EquityRecord> equities = mySqlSample->getEquities(profiles[0].ID);
    equities[0].quantity = 50;
    EXPECT_TRUE(mySqlSample->update(equities[0]));
    std::vector<database::EquityRecord> modifiedEquities = mySqlSample->getEquities(profiles[0].ID);
    EXPECT_TRUE(equities[0] == modifiedEquities[0]);
}

TEST_F(MySqlClientTest, DeleteInvalidEquitiesRecord)
{
    database::EquityRecord invalidEquity;
    EXPECT_TRUE(mySqlSample->drop(invalidEquity));
}

TEST_F(MySqlClientTest, DeleteValidEquitiesRecord)
{
    database::AccountRecord account = mySqlSample->getAccount("new_username", "password");
    std::vector<database::ProfileRecord> profiles = mySqlSample->getProfiles(account.ID);
    std::vector<database::EquityRecord> equities = mySqlSample->getEquities(profiles[0].ID);
    EXPECT_TRUE(mySqlSample->drop(equities[0]));
    std::vector<database::EquityRecord> deletedEquities = mySqlSample->getEquities(profiles[0].ID);
    EXPECT_TRUE(deletedEquities.empty());
}

TEST_F(MySqlClientTest, DeleteInvalidStockRecord)
{
    database::StockRecord invalidStock;
    EXPECT_TRUE(mySqlSample->drop(invalidStock));
}

TEST_F(MySqlClientTest, DeleteValidStockRecord)
{
    database::StockRecord stock = mySqlSample->getStock("STC", "USD");
    EXPECT_FALSE(stock.isEmpty());
    EXPECT_TRUE(mySqlSample->drop(stock));
    stock = mySqlSample->getStock("STC", "USD");
    EXPECT_TRUE(stock.isEmpty());
}

TEST_F(MySqlClientTest, DeleteInvalidProfileRecord)
{
    database::ProfileRecord invalidProfile;
    EXPECT_TRUE(mySqlSample->drop(invalidProfile));
}

TEST_F(MySqlClientTest, DeleteValidProfileRecord)
{
    database::AccountRecord account = mySqlSample->getAccount("new_username", "password");
    std::vector<database::ProfileRecord> profiles = mySqlSample->getProfiles(account.ID);
    EXPECT_TRUE(mySqlSample->drop(profiles[0]));
    std::vector<database::ProfileRecord> deletedProfiles = mySqlSample->getProfiles(account.ID);
    EXPECT_TRUE(deletedProfiles.empty());
}

TEST_F(MySqlClientTest, DeleteInvalidAccountRecord)
{
    database::AccountRecord account = {0, "invalid_username", "invalid_password"};
    EXPECT_TRUE(mySqlSample->drop(account));
}

TEST_F(MySqlClientTest, DeleteValidAccountRecord)
{
    database::AccountRecord account = mySqlSample->getAccount("new_username", "password");
    EXPECT_TRUE(mySqlSample->drop(account));
    account = mySqlSample->getAccount("new_username", "password");
    EXPECT_TRUE(account.isEmpty());
}

TEST_F(MySqlClientTest, InsertDuplicateAccountUsernameRecordFailure)
{
    database::AccountRecord account = {0, "Duplicate username", "Unique password 1"};
    mySqlSample->insert(account);
    account.password = "Unique password 2";
    EXPECT_FALSE(mySqlSample->insert(account));
    EXPECT_TRUE(mySqlSample->drop(mySqlSample->getAccount("Duplicate username", "Unique password 1")));
}

TEST_F(MySqlClientTest, InsertDuplicateAccountPasswordRecordFailure)
{
    database::AccountRecord account = {0, "Unique username 1", "Duplicate password"};
    mySqlSample->insert(account);
    account.username = "Unique username 2";
    EXPECT_FALSE(mySqlSample->insert(account));
    EXPECT_TRUE(mySqlSample->drop(mySqlSample->getAccount("Unique username 1", "Duplicate password")));
}

TEST_F(MySqlClientTest, DeleteRelatedProfilesAndEquitiesTriggerCheck)
{
    database::AccountRecord account = {0, "username", "password"};
    mySqlSample->insert(account);
    account = mySqlSample->getAccount("username", "password");
    database::ProfileRecord profile = {0, account.ID, "profile", "USD", 1000};
    mySqlSample->insert(profile);
    std::vector<database::ProfileRecord> profiles = mySqlSample->getProfiles(account.ID);
    database::StockRecord stock = {0, "stock", "symbol", "USD", 1000, "2024-09-16"};
    mySqlSample->insert(stock);
    stock = mySqlSample->getStock(stock.symbol, stock.currency);
    database::EquityRecord equity = {0, profiles[0].ID, stock.ID, 100, 20};
    mySqlSample->insert(equity);
    std::vector<database::EquityRecord> equities = mySqlSample->getEquities(profiles[0].ID);
    EXPECT_TRUE(mySqlSample->drop(account));
    EXPECT_TRUE(mySqlSample->getAccount("username", "password").isEmpty());
    EXPECT_TRUE(mySqlSample->getProfiles(account.ID).empty());
    EXPECT_TRUE(mySqlSample->getEquities(profiles[0].ID).empty());
    mySqlSample->drop(stock);
}

TEST_F(MySqlClientTest, DeleteRelatedEquitiesTriggerCheck)
{
    database::AccountRecord account = {0, "username", "password"};
    mySqlSample->insert(account);
    account = mySqlSample->getAccount("username", "password");
    database::ProfileRecord profile = {0, account.ID, "profile", "USD", 1000};
    mySqlSample->insert(profile);
    std::vector<database::ProfileRecord> profiles = mySqlSample->getProfiles(account.ID);
    database::StockRecord stock = {0, "stock", "symbol", "USD", 1000, "2024-09-16"};
    mySqlSample->insert(stock);
    stock = mySqlSample->getStock(stock.symbol, stock.currency);
    database::EquityRecord equity = {0, profiles[0].ID, stock.ID, 100, 20};
    mySqlSample->insert(equity);
    std::vector<database::EquityRecord> equities = mySqlSample->getEquities(profiles[0].ID);
    EXPECT_TRUE(mySqlSample->drop(profiles[0]));
    EXPECT_TRUE(mySqlSample->getProfiles(account.ID).empty());
    EXPECT_TRUE(mySqlSample->getEquities(profiles[0].ID).empty());
    mySqlSample->drop(account);
    mySqlSample->drop(stock);
}
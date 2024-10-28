#include "gtest/gtest.h"
#include "settingsModel.hpp"
#include "databaseClientMock.cpp"
#include "algorithm"

class SettingsModelTest : public testing::Test
{
protected:
    database::AccountRecord account, previousAccount;
    std::shared_ptr<DatabaseClientMock> databaseClientMock;
    std::shared_ptr<SettingsModel> settingsModel;

public:
    SettingsModelTest() {}
    ~SettingsModelTest() {}

    static void SetUpTestSuite()
    {
    }
    static void TearDownTestSuite()
    {
    }
    void SetUp() override
    {
        account = {1, "username", "password"};
        databaseClientMock = std::make_shared<DatabaseClientMock>();
        settingsModel = std::make_shared<SettingsModel>(account, databaseClientMock);
        previousAccount = account;
    }
    void TearDown() override
    {
    }
};

TEST_F(SettingsModelTest, InvalidateInputEmptyString)
{
    EXPECT_FALSE(settingsModel->validateInput(""));
}

TEST_F(SettingsModelTest, InvalidateOldInput)
{
    EXPECT_FALSE(settingsModel->changeUserName("username"));
}

TEST_F(SettingsModelTest, InvalidateLessThanRequiredStringSize)
{
    std::string invalidUserName(MIN_LENGTH - 1, 'a');
    EXPECT_FALSE(settingsModel->changeUserName(invalidUserName));
}

TEST_F(SettingsModelTest, ChangeUserNameUpdateDatabaseFailure)
{
    std::string newUsername("new_username");
    database::AccountRecord updatedAccount = account;
    updatedAccount.username = newUsername;
    EXPECT_CALL(*databaseClientMock, update(updatedAccount)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(settingsModel->changeUserName(newUsername));
    EXPECT_EQ(account, previousAccount);
}

TEST_F(SettingsModelTest, ChangeUserNameUpdateDatabaseSuccess)
{
    std::string newUsername("new_username");
    database::AccountRecord updatedAccount = account;
    updatedAccount.username = newUsername;
    EXPECT_CALL(*databaseClientMock, update(updatedAccount)).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(settingsModel->changeUserName(newUsername));
    EXPECT_EQ(account, updatedAccount);
}

TEST_F(SettingsModelTest, ChangePasswordUpdateDatabaseFailure)
{
    std::string newPassword("new_password");
    database::AccountRecord updatedAccount = account;
    updatedAccount.password = newPassword;
    EXPECT_CALL(*databaseClientMock, update(updatedAccount)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(settingsModel->changePassword(newPassword));
    EXPECT_EQ(account, previousAccount);
}

TEST_F(SettingsModelTest, ChangePasswordUpdateDatabaseSuccess)
{
    std::string newPassword("new_password");
    database::AccountRecord updatedAccount = account;
    updatedAccount.password = newPassword;
    EXPECT_CALL(*databaseClientMock, update(updatedAccount)).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(settingsModel->changePassword(newPassword));
    EXPECT_EQ(account, updatedAccount);
}
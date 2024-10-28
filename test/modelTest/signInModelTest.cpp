#include "gtest/gtest.h"
#include "signInModel.hpp"
#include "databaseClientMock.cpp"

class SignInModelTest : public testing::Test
{
protected:
    static const std::string invalidUsername;
    static const std::string invalidPassword;
    static const std::string validUsername;
    static const std::string validPassword;
    database::AccountRecord accountRecord;
    std::shared_ptr<DatabaseClientMock> databaseClientMock;
    std::shared_ptr<SignInModel> signInModel;

public:
    SignInModelTest()
    {
    }
    ~SignInModelTest()
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
        signInModel = std::make_shared<SignInModel>(accountRecord, databaseClientMock);
    }
    void TearDown() override
    {
    }
};

const std::string SignInModelTest::invalidUsername = "bad";
const std::string SignInModelTest::invalidPassword = "bad";
const std::string SignInModelTest::validUsername = "good_username";
const std::string SignInModelTest::validPassword = "good_password";

TEST_F(SignInModelTest, SignInFailure)
{
    database::AccountRecord invalidAccountRecord;
    EXPECT_CALL(*databaseClientMock, getAccount(invalidUsername, invalidPassword)).Times(1).WillOnce(testing::Return(invalidAccountRecord));
    EXPECT_FALSE(signInModel->signIn(invalidUsername, invalidPassword));
    EXPECT_EQ(accountRecord, invalidAccountRecord);
}

TEST_F(SignInModelTest, SignInSuccess)
{
    database::AccountRecord validAccountRecord = {1, validUsername, validPassword};
    EXPECT_CALL(*databaseClientMock, getAccount(validUsername, validPassword)).Times(1).WillOnce(testing::Return(validAccountRecord));
    EXPECT_TRUE(signInModel->signIn(validUsername, validPassword));
    EXPECT_EQ(accountRecord, validAccountRecord);
}

TEST_F(SignInModelTest, CanSignInBefore5Attempts)
{
    database::AccountRecord invalidAccountRecord;
    database::AccountRecord validAccountRecord = {1, validUsername, validPassword};
    testing::Sequence sequence;
    EXPECT_CALL(*databaseClientMock, getAccount(invalidUsername, invalidPassword)).Times(4).WillRepeatedly(testing::Return(invalidAccountRecord));
    EXPECT_CALL(*databaseClientMock, getAccount(validUsername, validPassword)).Times(1).WillOnce(testing::Return(validAccountRecord));
    for (int i = 0; i < MAX_ATTEMPTS - 1; ++i)
        EXPECT_FALSE(signInModel->signIn(invalidUsername, invalidPassword));
    EXPECT_TRUE(signInModel->signIn(validUsername, validPassword));
}

TEST_F(SignInModelTest, CanNotSignInAfter5Attempts)
{
    database::AccountRecord invalidAccountRecord;
    database::AccountRecord validAccountRecord = {1, validUsername, validPassword};
    EXPECT_CALL(*databaseClientMock, getAccount(testing::StrEq(invalidUsername), testing::StrEq(invalidPassword))).Times(5).WillRepeatedly(testing::Return(invalidAccountRecord));
    for (int i = 0; i < MAX_ATTEMPTS; ++i)
        EXPECT_FALSE(signInModel->signIn(invalidUsername, invalidPassword));
    for (int i = 0; i < MAX_ATTEMPTS; ++i)
        EXPECT_FALSE(signInModel->signIn(validUsername, validPassword));
}
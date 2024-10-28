#include "gtest/gtest.h"
#include "signUpModel.hpp"
#include "databaseClientMock.cpp"

class SignUpModelTest : public testing::Test
{
protected:
    static const std::string invalidUsername;
    static const std::string invalidPassword;
    static const std::string validUsername;
    static const std::string validPassword;
    std::shared_ptr<DatabaseClientMock> databaseClientMock;
    std::shared_ptr<SignUpModel> signUpModel;

public:
    SignUpModelTest()
    {
    }
    ~SignUpModelTest()
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
        signUpModel = std::make_shared<SignUpModel>(databaseClientMock);
    }
    void TearDown() override
    {
    }
};

const std::string SignUpModelTest::invalidUsername{MIN_LENGTH - 1, 'a'};
const std::string SignUpModelTest::invalidPassword{MIN_LENGTH - 1, 'a'};
const std::string SignUpModelTest::validUsername = "good_username";
const std::string SignUpModelTest::validPassword = "good_password";

TEST_F(SignUpModelTest, InvalidateBadUserName)
{
    EXPECT_FALSE(signUpModel->validateInput(invalidUsername, validPassword));
}

TEST_F(SignUpModelTest, InvalidateBadPassword)
{
    EXPECT_FALSE(signUpModel->validateInput(validUsername, invalidPassword));
}

TEST_F(SignUpModelTest, InvalidateBadUserNameAndPassword)
{
    EXPECT_FALSE(signUpModel->validateInput(invalidUsername, invalidPassword));
}

TEST_F(SignUpModelTest, ValidateGoodUserNameAndPassword)
{
    EXPECT_TRUE(signUpModel->validateInput(validUsername, validPassword));
}

TEST_F(SignUpModelTest, RegisterAccountFailure)
{
    database::AccountRecord accountRecord = {0, invalidUsername, invalidPassword};
    EXPECT_CALL(*databaseClientMock, insert(accountRecord)).Times(1).WillOnce(testing::Return(false));
    EXPECT_FALSE(signUpModel->registerAccount(invalidUsername, invalidPassword));
}

TEST_F(SignUpModelTest, RegisterAccountSuccess)
{
    database::AccountRecord accountRecord = {0, validUsername, validPassword};
    EXPECT_CALL(*databaseClientMock, insert(accountRecord)).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(signUpModel->registerAccount(validUsername, validPassword));
}
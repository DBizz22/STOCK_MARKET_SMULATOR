#include <iostream>
#include "gtest/gtest.h"
#include "timeApi.hpp"
#include "httpClientMock.cpp"

class UTCTimeApiTest : public testing::Test
{
protected:
    std::shared_ptr<HttpClientMock> mock;
    UTCTimeApi *UTCTimeApiSample;
    static std ::string urlRequest;
    static std::string urlResponse;

public:
    UTCTimeApiTest()
    {
    }
    ~UTCTimeApiTest()
    {
    }
    static void SetUpTestSuite()
    {
        urlRequest = "https://timeapi.io/api/time/current/zone?timeZone=Europe%2FLondon";
        urlResponse = R"({
                "year": 2024,
                "month": 11,
                "day": 11,
                "hour": 13,
                "minute": 0,
                "seconds": 12,
                "milliSeconds": 903,
                "dateTime": "2024-11-11T13:00:12.9030271",
                "date": "11/11/2024",
                "time": "13:00",
                "timeZone": "Europe/London",
                "dayOfWeek": "Monday",
                "dstActive": false
        })";
    }
    static void TearDownTestSuite()
    {
    }
    void SetUp() override
    {
        mock = std::make_shared<HttpClientMock>();
        UTCTimeApiSample = new UTCTimeApi(mock);
    }
    void TearDown() override
    {
        delete UTCTimeApiSample;
    }
};

std::string UTCTimeApiTest::urlRequest = "";
std::string UTCTimeApiTest::urlResponse = "";

TEST_F(UTCTimeApiTest, SendRequest)
{
    EXPECT_CALL(*mock, sendRequest(urlRequest)).Times(1).WillOnce(testing::Return(true));
    EXPECT_TRUE(UTCTimeApiSample->sendRequest());
}

TEST_F(UTCTimeApiTest, GetEmptyResponse)
{
    std::string emptyUrlResponse;
    EXPECT_CALL(*mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(emptyUrlResponse));
    EXPECT_EQ(UTCTimeApiSample->getResponse(), emptyUrlResponse);
}

TEST_F(UTCTimeApiTest, GetValidResponse)
{
    EXPECT_CALL(*mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(urlResponse));
    EXPECT_EQ(UTCTimeApiSample->getResponse(), urlResponse);
}

TEST_F(UTCTimeApiTest, GetInvalidTime)
{
    std::string invalidResponse = R"({
        "invalid_key" : "invalid_value"
        })";
    DateTime emptyTime;
    EXPECT_CALL(*mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(invalidResponse));
    UTCTimeApiSample->sendRequest();
    EXPECT_EQ(UTCTimeApiSample->getTime(), emptyTime);
}

TEST_F(UTCTimeApiTest, GetValidTime)
{
    DateTime validUTCDateTime = {"2024-11-11", "13:00:12"};
    EXPECT_CALL(*mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(urlResponse));
    UTCTimeApiSample->sendRequest();
    EXPECT_EQ(UTCTimeApiSample->getTime(), validUTCDateTime);
}

TEST_F(UTCTimeApiTest, UTCTimeComparators)
{
    DateTime time1 = {"2024-10-17", "19:41:17"};
    DateTime time2 = {"2024-10-17", "20:41:17"};
    DateTime time3 = {"2024-10-18", "19:41:17"};
    DateTime time4 = {"2024-10-18", "20:41:17"};
    EXPECT_TRUE(time1 < time2);
    EXPECT_TRUE(time2 < time3);
    EXPECT_TRUE(time3 < time4);
    EXPECT_TRUE(time4 > time3);
    EXPECT_TRUE(time3 > time2);
    EXPECT_TRUE(time2 > time1);
}

TEST_F(UTCTimeApiTest, GetTimeDIfferenceInSeconds)
{
    DateTime time1 = {"2024-10-17", "19:41:17"};
    DateTime time2 = {"2024-10-17", "20:41:17"};
    DateTime time3 = {"2024-10-18", "00:00:00"};
    DateTime time4 = {"2024-10-20", "00:00:00"};
    EXPECT_EQ(time2 - time1, 3600);
    EXPECT_EQ(time4 - time3, 172800);
}
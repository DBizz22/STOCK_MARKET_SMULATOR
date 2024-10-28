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
        urlRequest = "http://worldtimeapi.org/api/timezone/Europe/London";
        urlResponse = R"({
                      "utc_offset" : "+01:00",
        "timezone" : "Europe/London",
        "day_of_week" : 4,
        "day_of_year" : 291,
        "datetime" : "2024-10-17T20:41:17.190200+01:00",
        "utc_datetime" : "2024-10-17T19:41:17.190200+00:00",
        "unixtime" : 1729194077,
        "raw_offset" : 0,
        "week_number" : 42,
        "dst" : true,
        "abbreviation" : "BST",
        "dst_offset" : 3600,
        "dst_from" : "2024-03-31T01:00:00+00:00",
        "dst_until" : "2024-10-27T01:00:00+00:00",
        "client_ip" : "91.243.81.186"
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
    Time emptyTime;
    EXPECT_CALL(*mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(invalidResponse));
    UTCTimeApiSample->sendRequest();
    EXPECT_EQ(UTCTimeApiSample->getTime(), emptyTime);
}

TEST_F(UTCTimeApiTest, GetValidTime)
{
    Time validUTCDateTime = {"2024-10-17", "19:41:17"};
    EXPECT_CALL(*mock, getResponse()).Times(1).WillOnce(testing::ReturnRef(urlResponse));
    UTCTimeApiSample->sendRequest();
    EXPECT_EQ(UTCTimeApiSample->getTime(), validUTCDateTime);
}

TEST_F(UTCTimeApiTest, UTCTimeComparators)
{
    Time time1 = {"2024-10-17", "19:41:17"};
    Time time2 = {"2024-10-17", "20:41:17"};
    Time time3 = {"2024-10-18", "19:41:17"};
    Time time4 = {"2024-10-18", "20:41:17"};
    EXPECT_TRUE(time1 < time2);
    EXPECT_TRUE(time2 < time3);
    EXPECT_TRUE(time3 < time4);
    EXPECT_TRUE(time4 > time3);
    EXPECT_TRUE(time3 > time2);
    EXPECT_TRUE(time2 > time1);
}

TEST_F(UTCTimeApiTest, GetTimeDIfferenceInSeconds)
{
    Time time1 = {"2024-10-17", "19:41:17"};
    Time time2 = {"2024-10-17", "20:41:17"};
    Time time3 = {"2024-10-18", "00:00:00"};
    Time time4 = {"2024-10-20", "00:00:00"};
    EXPECT_EQ(time2 - time1, 3600);
    EXPECT_EQ(time4 - time3, 172800);
}
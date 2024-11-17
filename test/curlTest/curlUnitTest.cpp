#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>
#include "curlClient.hpp"

TEST_GROUP(CurlClientTest)
{
    std::string request;
    CurlClient *sample;
    void setup()
    {
        sample = new CurlClient();
        request = "https://httpstat.us/200";
    }
    void teardown()
    {
        delete sample;
    }
};

TEST(CurlClientTest, SendRequestFailure)
{
    bool result = sample->sendRequest("");
    CHECK(result == false);
}

TEST(CurlClientTest, SendRequestSuccess)
{
    bool result = sample->sendRequest(request);
    CHECK(result == true);
}

TEST(CurlClientTest, GetResponseEmptyString)
{
    CHECK_EQUAL(sample->getResponse(), "");
}

TEST(CurlClientTest, GetResponseString)
{
    sample->sendRequest(request);
    CHECK_EQUAL(sample->getResponse(), "200 OK");
}

TEST(CurlClientTest, ResponseAfterGoodThenBadRequest)
{
    sample->sendRequest(request);
    bool result = sample->sendRequest("");
    CHECK(result == false);
    CHECK_EQUAL(sample->getResponse(), "");
}

int main(int argc, char **argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
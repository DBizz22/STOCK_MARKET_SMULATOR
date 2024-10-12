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

IGNORE_TEST(CurlClientTest, SendRequestFailure)
{
    bool result = sample->sendRequest("");
    CHECK(result == false);
}

IGNORE_TEST(CurlClientTest, SendRequestSuccess)
{
    bool result = sample->sendRequest(request);
    CHECK(result == true);
}

IGNORE_TEST(CurlClientTest, GetResponseEmptyString)
{
    CHECK_EQUAL(sample->getResponse(), "");
}

IGNORE_TEST(CurlClientTest, GetResponseString)
{
    sample->sendRequest(request);
    CHECK_EQUAL(sample->getResponse(), "200 OK");
}

IGNORE_TEST(CurlClientTest, ResponseAfterGoodThenBadRequest)
{
    sample->sendRequest(request);
    bool result = sample->sendRequest("");
    CHECK(result == false);
    CHECK_EQUAL(sample->getResponse(), "");
}

int main(int argc, char **argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
    // return RUN_ALL_TESTS(argc, argv);
}
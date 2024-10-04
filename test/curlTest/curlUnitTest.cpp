#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>
#include "curlClient.hpp"

TEST_GROUP(curlClientTest)
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

IGNORE_TEST(curlClientTest, sendRequestFailure)
{
    bool result = sample->sendRequest("");
    CHECK(result == false);
}

IGNORE_TEST(curlClientTest, sendRequestSuccess)
{
    bool result = sample->sendRequest(request);
    CHECK(result == true);
}

IGNORE_TEST(curlClientTest, getResponseEmptyString)
{
    CHECK_EQUAL(sample->getResponse(), "");
}

IGNORE_TEST(curlClientTest, getResponseString)
{
    sample->sendRequest(request);
    CHECK_EQUAL(sample->getResponse(), "200 OK");
}

IGNORE_TEST(curlClientTest, responseAfterGoodThenBadRequest)
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
add_library(httpClientMock STATIC httpClientMock.cpp)
target_link_libraries(httpClientMock httpClientInterface gmock)
target_include_directories(httpClientMock PUBLIC .)
include(Testing)
add_subdirectory(curlTest)
add_subdirectory(alphavantageApiSuiteTest)

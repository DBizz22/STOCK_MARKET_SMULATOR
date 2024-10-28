add_library(httpClientMock STATIC httpClientMock.cpp)
target_link_libraries(httpClientMock httpClientInterface gmock)
target_include_directories(httpClientMock PUBLIC .)

add_library(databaseClientMock STATIC databaseClientMock.cpp)
target_link_libraries(databaseClientMock databaseClientInterface gmock)
target_include_directories(databaseClientMock PUBLIC .)

add_library(stockApiMock STATIC stockApiMock.cpp)
target_link_libraries(stockApiMock stockApiInterface gmock)
target_include_directories(stockApiMock PUBLIC .)

add_library(timeApiMock STATIC timeApiMock.cpp)
target_link_libraries(timeApiMock timeApiSTATIC gmock)
target_include_directories(timeApiMock PUBLIC .)

include(Testing)

# add_subdirectory(curlTest)
# add_subdirectory(alphavantageApiSuiteTest)
# add_subdirectory(mySqlClientTest)

# add_subdirectory(timeApiTest)
add_subdirectory(modelTest)
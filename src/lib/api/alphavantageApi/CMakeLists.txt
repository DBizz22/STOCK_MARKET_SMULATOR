add_library(alphavantageStatic STATIC alphavantage.cpp)
target_include_directories(alphavantageStatic PUBLIC include)
target_link_libraries(alphavantageStatic stockApi httpClientInterface)

add_executable(tester exe.cpp)
target_link_libraries(tester PRIVATE alphavantageStatic curlStatic)

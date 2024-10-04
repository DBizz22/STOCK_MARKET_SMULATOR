add_library(stockApi STATIC stockApi.cpp)
target_include_directories(stockApi PUBLIC include)
target_link_libraries(stockApi httpClientInterface Boost::json)
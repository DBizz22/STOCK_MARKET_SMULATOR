add_library(stockApiInterface INTERFACE stockApi.hpp)
target_include_directories(httpClientInterface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
target_link_libraries(stockApiInterface INTERFACE httpClientInterface Boost::json)
add_library(httpClientInterface INTERFACE httpClient.hpp)
target_include_directories(httpClientInterface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
add_subdirectory(curl)

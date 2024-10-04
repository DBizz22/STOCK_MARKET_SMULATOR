add_library(databaseClientInterface INTERFACE databaseClient.hpp)
target_include_directories(databaseClientInterface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

# add_subdirectory(curl)
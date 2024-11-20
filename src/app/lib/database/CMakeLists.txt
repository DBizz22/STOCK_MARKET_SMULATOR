# include(GetMariaDB)
include(GetSoci)
add_library(databaseClientInterface INTERFACE databaseClient.hpp)
target_link_libraries(databaseClientInterface INTERFACE soci_core_static) # soci_core)
target_include_directories(databaseClientInterface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

add_subdirectory(mySqlClient)
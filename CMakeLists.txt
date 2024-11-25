cmake_minimum_required(VERSION 3.20.0)

# cmake_policy(VERSION 3.20)
project(Stock_Market_Simulator VERSION 1.0.1 LANGUAGES C CXX)

set(CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(NoInSourceBuilds)

add_subdirectory(src bin)
include(CTest)
enable_testing()

add_subdirectory(test)
install(TARGETS ${PROJECT_NAME}
    RUNTIME
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "Samuel Divine")
set(CPACK_PACKAGE_CONTACT "divinesamuel1515@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION "Simple Stock Market Simulator Project")

include(CPack)

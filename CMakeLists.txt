cmake_minimum_required(VERSION 3.20.0)

# cmake_policy(VERSION 3.20)
project(Stock_Market_Simulator VERSION 1.0.0 LANGUAGES C CXX)

set(CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(EXTERNAL_HEADERS "C:/msys64/ucrt64/include/mysql")
include(NoInSourceBuilds)

add_subdirectory(src bin)
include(CTest)
enable_testing()
add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# include(Install)
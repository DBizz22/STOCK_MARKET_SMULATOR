set(BUILD_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)
set(SOCI_SHARED OFF)
set(SOCI_STATIC ON)
set(WITH_MYSQL ON)
set(WITH_BOOST OFF)

#[=[include(FetchContent)
FetchContent_Declare(
    soci
    GIT_REPOSITORY https://github.com/SOCI/soci.git
    GIT_TAG v4.0.3
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    EXCLUDE_FROM_ALL
)]=]
message(STATUS "Downloading and extracting soci library sources. This will take some time...")
include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE) # Needed to print downloading progress
FetchContent_Declare(
    soci
    URL https://github.com/SOCI/soci/archive/refs/tags/v4.0.3.zip # downloading a zip release speeds up the download
    USES_TERMINAL_DOWNLOAD TRUE
    GIT_PROGRESS TRUE

    # DOWNLOAD_NO_EXTRACT FALSE
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(soci)
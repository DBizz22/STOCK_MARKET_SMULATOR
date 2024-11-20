set(BUILD_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)
set(CURL_USE_SCHANNEL ON)

set(CURL_USE_OPENSSL OFF)
set(USE_WINDOWS_SSPI ON)
set(USE_LIBIDN2 OFF)
set(BUILD_CURL_EXE OFF)

#[=[include(FetchContent)
FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_9_1
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    EXCLUDE_FROM_ALL
)]=]
message(STATUS "Downloading and extracting curl library sources. This will take some time...")
include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE) # Needed to print downloading progress
FetchContent_Declare(
    curl
    URL https://github.com/curl/curl/releases/download/curl-8_9_1/curl-8.9.1.zip # downloading a zip release speeds up the download
    USES_TERMINAL_DOWNLOAD TRUE
    GIT_PROGRESS TRUE

    # DOWNLOAD_NO_EXTRACT FALSE
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(curl)

# find_package(CURL REQUIRED)
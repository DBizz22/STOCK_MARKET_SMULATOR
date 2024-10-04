set(BUILD_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)
set(CURL_USE_SCHANNEL ON)

set(CURL_USE_OPENSSL OFF)
set(USE_WINDOWS_SSPI ON)
set(BUILD_CURL_EXE OFF)
include(FetchContent)
FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_9_1
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(curl)

# find_package(CURL REQUIRED)
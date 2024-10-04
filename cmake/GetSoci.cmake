set(BUILD_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)
include(FetchContent)
FetchContent_Declare(
    soci
    GIT_REPOSITORY https://github.com/SOCI/soci.git
    GIT_TAG v4.0.3
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(soci)
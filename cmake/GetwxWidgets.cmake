include(FetchContent)
set(wxBUILD_SHARED OFF)
set(wxUSE_GUI ON)
set(wxBUILD_PRECOMP ON)
set(wxBUILD_COMPATIBILITY "3.1")

#[=[FetchContent_Declare(wxWidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
    GIT_TAG v3.2.5 # Important: Specify a version or a commit hash here.
    GIT_PROGRESS True
    GIT_SHALLOW TRUE
)]=]
FetchContent_Declare(
    wxWidgets
    URL https://github.com/wxWidgets/wxWidgets/releases/download/v3.2.5/wxWidgets-3.2.5.zip # downloading a zip release speeds up the download
    USES_TERMINAL_DOWNLOAD TRUE
    GIT_PROGRESS TRUE

    # DOWNLOAD_NO_EXTRACT FALSE
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(wxWidgets)
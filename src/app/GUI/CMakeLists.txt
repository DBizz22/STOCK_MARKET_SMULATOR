# include(GetFTXUI)
include(GetwxWidgets)
add_subdirectory(pages)

set(APP_NAME ${PROJECT_NAME})
add_executable(${APP_NAME} bootstrap.cpp resource.rc)
link_directories(${APP_NAME} "C:\\msys64\\ucrt64\\include")

# target_include_directories(${APP_NAME} " C:\\msys64\\ucrt64\\include\\ ")

# target_compile_options(${APP_NAME} PRIVATE -g)
target_link_options(${APP_NAME} PUBLIC -mwindows)

target_link_libraries(${APP_NAME} -static wxWidgetsPages)

add_executable(CLItest CLItest.cpp)
target_link_libraries(CLItest wxWidgetsPages)

add_executable(GUItest GUItest.cpp)
target_link_libraries(GUItest wxWidgetsPages)

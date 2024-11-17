# include(GetFTXUI)
include(GetwxWidgets)
add_subdirectory(pages)
set(APP_NAME ${PROJECT_NAME})
add_executable(${APP_NAME} bootstrap.cpp resource.rc)

# target_compile_options(${APP_NAME} PRIVATE -g)
target_link_options(${APP_NAME} PRIVATE -mwindows)

target_link_libraries(${APP_NAME} wxWidgetsPages)

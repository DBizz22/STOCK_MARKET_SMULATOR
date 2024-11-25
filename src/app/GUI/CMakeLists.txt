include(GetwxWidgets)
add_subdirectory(pages)

set(APP_NAME ${PROJECT_NAME})
add_executable(${APP_NAME} bootstrap.cpp resource.rc)
target_link_options(${APP_NAME} PUBLIC -mwindows)
target_link_libraries(${APP_NAME} wxWidgetsPages)
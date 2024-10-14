include(GetLibcurl)
add_library(curlStatic STATIC curlClient.cpp)
target_link_libraries(curlStatic httpClientInterface CURL::libcurl_static)
target_include_directories(curlStatic PUBLIC include)
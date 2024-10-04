add_executable(curlTest curlUnitTest.cpp)
target_link_libraries(curlTest
    curlStatic CppUTest::CppUTest
    CppUTest::CppUTestExt)

# EnableCoverage(calc_console_static)
# target_include_directories(testrunner PRIVATE )
cpputest_discover_tests(curlTest)
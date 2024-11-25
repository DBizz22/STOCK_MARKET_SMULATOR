add_executable(curlTest curlUnitTest.cpp)
target_link_libraries(curlTest
    curlStatic CppUTest::CppUTest
    CppUTest::CppUTestExt)

cpputest_discover_tests(curlTest)
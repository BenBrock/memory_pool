enable_testing()

add_executable(mp-tests
               free_test.cpp
               basic_compaction.cpp)

target_link_libraries(mp-tests memory_pool fmt GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(mp-tests)

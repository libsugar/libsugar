enable_testing()

set(TARGET_NAME tests)

add_executable("${TARGET_NAME}" test_result.cc)

add_dependencies("${TARGET_NAME}" sugar.effect sugar.result)
target_link_libraries("${TARGET_NAME}" sugar.effect sugar.result)

target_link_libraries("${TARGET_NAME}" GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests("${TARGET_NAME}")

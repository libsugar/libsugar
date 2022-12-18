set(TARGET_NAME sugar.result)

set(TARGET_SRC Result.cc Result.h)

add_library("${TARGET_NAME}" STATIC "${TARGET_SRC}")
target_include_directories("${TARGET_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

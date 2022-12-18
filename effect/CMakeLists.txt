set(TARGET_NAME sugar.effect)

set(TARGET_SRC effect.cc effect.h)

add_library("${TARGET_NAME}" STATIC "${TARGET_SRC}")
target_include_directories("${TARGET_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

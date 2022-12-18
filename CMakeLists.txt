cmake_minimum_required(VERSION 3.23)
project(sugar)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(effect)
add_subdirectory(result)

add_subdirectory(tests/googletest)
add_subdirectory(tests)

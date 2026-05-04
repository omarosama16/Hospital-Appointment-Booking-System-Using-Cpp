cmake_minimum_required(VERSION 3.14)
project(HospitalBookingSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB CORE_SOURCES src/core/*.cpp)

add_library(core_lib STATIC ${CORE_SOURCES})
target_include_directories(core_lib PUBLIC include)

file(GLOB APP_SOURCES src/app/*.cpp)

add_executable(app ${APP_SOURCES})
target_link_libraries(app PRIVATE core_lib)

file(GLOB TEST_SOURCES tests/*.cpp)

add_executable(tests_runner ${TEST_SOURCES})
target_link_libraries(tests_runner PRIVATE core_lib gtest gtest_main pthread)

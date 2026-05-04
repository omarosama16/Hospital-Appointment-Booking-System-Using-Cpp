cmake_minimum_required(VERSION 3.16)
project(HospitalSystem)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES
    src/core/*.cpp
    src/app/*.cpp
)

add_executable(app ${SOURCES})

enable_testing()

# GoogleTest (if tests exist)
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

if(TEST_SOURCES)
    add_executable(tests ${TEST_SOURCES} ${SOURCES})
    target_link_libraries(tests gtest gtest_main pthread)
endif()

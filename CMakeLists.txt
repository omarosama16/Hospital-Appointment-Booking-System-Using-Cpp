cmake_minimum_required(VERSION 3.16)
project(HospitalApp)

set(CMAKE_CXX_STANDARD 17)

# ====== SOURCES ======
file(GLOB_RECURSE SRC_FILES
    src/core/*.cpp
    src/app/*.cpp
)

include_directories(include)
include_directories(src)

add_executable(app ${SRC_FILES})

# ====== TESTS (GoogleTest) ======
enable_testing()

file(GLOB_RECURSE TEST_FILES
    tests/*.cpp
)

add_executable(unit_tests ${SRC_FILES} ${TEST_FILES})

target_link_libraries(unit_tests gtest gtest_main pthread)

add_test(NAME AllTests COMMAND unit_tests)

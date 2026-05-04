cmake_minimum_required(VERSION 3.10)

project(HospitalAppointmentSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# include headers
include_directories(include)

# core sources only (business logic)
file(GLOB_RECURSE CORE_SOURCES
    src/core/*.cpp
)

# -------------------------
# MAIN APPLICATION
# -------------------------
add_executable(app
    src/app/Main.cpp
    ${CORE_SOURCES}
)

# -------------------------
# TESTS (GoogleTest)
# -------------------------
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB_RECURSE TEST_SOURCES
    tests/*.cpp
)

add_executable(runTests
    ${TEST_SOURCES}
    ${CORE_SOURCES}
)

target_link_libraries(runTests GTest::GTest GTest::Main pthread)

add_test(NAME AllTests COMMAND runTests)

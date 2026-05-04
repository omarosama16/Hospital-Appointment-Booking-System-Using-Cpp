cmake_minimum_required(VERSION 3.16)

project(HospitalAppointmentSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ======================
# Include directories
# ======================
include_directories(include)
include_directories(src)

# ======================
# Source files
# ======================
file(GLOB_RECURSE APP_SOURCES
    src/core/*.cpp
    src/app/*.cpp
)

add_executable(app ${APP_SOURCES})

# ======================
# TESTS (GoogleTest)
# ======================
enable_testing()

file(GLOB_RECURSE TEST_SOURCES
    tests/*.cpp
)

add_executable(unit_tests ${APP_SOURCES} ${TEST_SOURCES})

target_link_libraries(unit_tests gtest gtest_main pthread)

add_test(NAME AllTests COMMAND unit_tests)

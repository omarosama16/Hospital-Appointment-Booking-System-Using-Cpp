cmake_minimum_required(VERSION 3.10)
project(HospitalApp)

set(CMAKE_CXX_STANDARD 17)

# Enable compile database (CRITICAL FOR SONAR)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

# Main app
file(GLOB SOURCES "src/*.cpp")
add_executable(app ${SOURCES})

# Tests
enable_testing()

file(GLOB TEST_SOURCES "tests/*.cpp" "src/*.cpp")
add_executable(tests ${TEST_SOURCES})

# GoogleTest (system installed in CI)
target_link_libraries(tests gtest gtest_main pthread)

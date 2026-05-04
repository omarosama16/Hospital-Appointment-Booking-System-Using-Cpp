cmake_minimum_required(VERSION 3.10)

project(HospitalAppointmentSystem)

# =========================
# C++ STANDARD
# =========================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# مهم لـ Sonar + static analysis
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================
# INCLUDE DIRECTORIES
# =========================
include_directories(include)
include_directories(src)

# =========================
# CORE SOURCES (Business Logic)
# =========================
file(GLOB_RECURSE CORE_SOURCES
    src/core/*.cpp
)

# =========================
# MAIN APPLICATION
# =========================
add_executable(app
    src/app/Main.cpp
    ${CORE_SOURCES}
)

# =========================
# TESTS (GoogleTest)
# =========================
enable_testing()

find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SOURCES
    tests/*.cpp
)

add_executable(runTests
    ${TEST_SOURCES}
    ${CORE_SOURCES}
)

# correct modern GTest linking
target_link_libraries(runTests
    GTest::gtest
    GTest::gtest_main
    pthread
)

# include paths for tests
target_include_directories(runTests PRIVATE
    include
    src
)

# =========================
# CTEST
# =========================
add_test(NAME AllTests COMMAND runTests)

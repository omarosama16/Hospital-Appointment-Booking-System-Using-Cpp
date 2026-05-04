cmake_minimum_required(VERSION 3.10)
project(HospitalAppointmentSystem)

# =========================
# C++ STANDARD
# =========================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================
# INCLUDE DIRECTORIES
# =========================
include_directories(include)

# =========================
# CORE SOURCES (Business Logic)
# Fixed: sources live in src/core/*.cpp, not a deeper subdir
# =========================
file(GLOB_RECURSE CORE_SOURCES
    src/core/*.cpp
)

# =========================
# MAIN APPLICATION
# Fixed: Main.cpp lives at src/app/Main.cpp
# =========================
add_executable(app
    src/app/Main.cpp
    ${CORE_SOURCES}
)

target_include_directories(app PRIVATE include)

# =========================
# TESTS (GoogleTest)
# =========================
enable_testing()
find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SOURCES
    tests/*.cpp
)

if(TEST_SOURCES)
    add_executable(runTests
        ${TEST_SOURCES}
        ${CORE_SOURCES}
    )

    target_link_libraries(runTests
        GTest::gtest
        GTest::gtest_main
        pthread
    )

    target_include_directories(runTests PRIVATE
        include
    )

    add_test(NAME AllTests COMMAND runTests)
endif()

cmake_minimum_required(VERSION 3.16)
project(HospitalAppointmentBookingSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# =========================
# GoogleTest
# =========================
find_package(GTest REQUIRED)

# =========================
# Source files
# =========================
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
list(FILTER SRC_FILES EXCLUDE REGEX ".*/Main\\.cpp$")

# =========================
# Core Library (BEST PRACTICE)
# =========================
add_library(hospital_core ${SRC_FILES})

target_include_directories(hospital_core
    PUBLIC ${CMAKE_SOURCE_DIR}/src
)

# =========================
# Main Application
# =========================
add_executable(hospital_app src/Main.cpp)

target_link_libraries(hospital_app hospital_core)

# =========================
# Tests
# =========================
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS tests/*.cpp)

add_executable(run_tests ${TEST_FILES})

target_link_libraries(run_tests
    hospital_core
    GTest::GTest
    GTest::Main
)

# Include headers for tests
target_include_directories(run_tests
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

# =========================
# Enable CTest
# =========================
add_test(NAME unit_tests COMMAND run_tests)
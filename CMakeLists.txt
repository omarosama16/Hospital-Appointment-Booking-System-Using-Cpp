cmake_minimum_required(VERSION 3.16)
project(HospitalAppointmentBookingSystem)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

find_package(GTest REQUIRED)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
list(FILTER SRC_FILES EXCLUDE REGEX ".*/Main\\.cpp$")

add_executable(hospital_app
    ${SRC_FILES}
    src/Main.cpp
)

target_include_directories(hospital_app PRIVATE src)

file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS tests/*.cpp)

add_executable(run_tests
    ${TEST_FILES}
    ${SRC_FILES}
)

target_include_directories(run_tests PRIVATE src)

target_link_libraries(run_tests
    GTest::GTest
    GTest::Main
)

add_test(NAME unit_tests COMMAND run_tests)
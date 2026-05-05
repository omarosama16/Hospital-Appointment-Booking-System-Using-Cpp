cmake_minimum_required(VERSION 3.16)
project(HospitalAppointmentBookingSystem)
 
set(CMAKE_CXX_STANDARD 17)
 
# Auto-collect all .cpp files in src/ except main.cpp
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
list(FILTER SRC_FILES EXCLUDE REGEX ".*main\\.cpp$")
 
# Main executable (all src files including main.cpp)
file(GLOB_RECURSE ALL_SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
add_executable(hospital_app ${ALL_SRC_FILES})
 
# Only enable testing if test files exist
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS tests/*.cpp)
 
if(TEST_FILES)
    enable_testing()
    find_package(GTest REQUIRED)
    add_executable(run_tests ${TEST_FILES} ${SRC_FILES})
    target_include_directories(run_tests PRIVATE src)
    target_link_libraries(run_tests GTest::GTest GTest::Main)
    add_test(NAME unit_tests COMMAND run_tests)
endif()
 

cmake_minimum_required(VERSION 3.10)
project(HospitalAppointmentSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Force -std=c++17 explicitly into compile commands for SonarCloud
add_compile_options(-std=c++17)

include_directories(include)

file(GLOB_RECURSE CORE_SOURCES src/core/*.cpp)

add_executable(app
    src/app/Main.cpp
    ${CORE_SOURCES}
)
target_include_directories(app PRIVATE include)

enable_testing()

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

if(TEST_SOURCES)
    find_package(GTest REQUIRED)

    add_executable(runTests
        ${TEST_SOURCES}
        ${CORE_SOURCES}
    )

    target_link_libraries(runTests
        GTest::gtest
        GTest::gtest_main
        pthread
    )

    target_include_directories(runTests PRIVATE include)

    add_test(NAME AllTests COMMAND runTests)
endif()

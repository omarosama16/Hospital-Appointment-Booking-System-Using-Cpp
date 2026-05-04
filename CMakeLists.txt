cmake_minimum_required(VERSION 3.10)
project(HospitalSystem)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# ======================
# Collect source files
# ======================
file(GLOB_RECURSE CORE_SOURCES src/core/*.cpp)
file(GLOB_RECURSE APP_SOURCES  src/app/*.cpp)

set(SOURCES
    ${CORE_SOURCES}
    ${APP_SOURCES}
)

# ======================
# Build executable
# ======================
add_executable(app ${SOURCES})

# ======================
# GoogleTest (optional)
# ======================
enable_testing()

find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

add_executable(tests ${TEST_SOURCES} ${CORE_SOURCES})

target_link_libraries(tests GTest::GTest GTest::Main pthread)

add_test(NAME AllTests COMMAND tests)

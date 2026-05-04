cmake_minimum_required(VERSION 3.10)
project(HospitalSystem)

set(CMAKE_CXX_STANDARD 17)

# Include headers
include_directories(include)

# Source files (IMPORTANT: auto include everything)
file(GLOB_RECURSE SOURCES
    "src/**/*.cpp"
)

add_executable(app ${SOURCES})

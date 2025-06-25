cmake_minimum_required(VERSION 3.10)
project(KV_PROJECT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable release flags by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set highly optimized flags for Release build
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -DNDEBUG")
endif()

# Globally enable Link Time Optimization (LTO)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Include directories
include_directories(shared include)

# Client target
add_executable(client_exec
    client/client.cpp
    shared/helpers.cpp
)

# Server target (grabs all .cpp files from src/ and shared/)
file(GLOB SERVER_SRC "src/*.cpp" "shared/*.c*" "shared/exception/*.cpp")
add_executable(server_exec ${SERVER_SRC})

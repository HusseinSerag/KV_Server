cmake_minimum_required(VERSION 3.10)
project(KV_PROJECT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -DNDEBUG")
endif()

include_directories(shared include)

add_executable(client_exec
    client/client.cpp
    shared/helpers.cpp
)

file(GLOB SERVER_SRC "src/*.cpp" "shared/*.cpp" "shared/exception/*.cpp")

add_executable(server_exec ${SERVER_SRC}
)
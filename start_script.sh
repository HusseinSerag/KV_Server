#!/bin/bash


#!/bin/bash
set -e

# Create build dir if missing
mkdir -p build

# Configure and build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -- -j$(nproc)

# Run the server
./build/server_exec --port=$1 --persistence=$2 --log_enabled=0


//cd build && cmake  .. && make && cd .. && ./build/server_exec --port=$1 --persistence=$2 --log_enabled=0
#cd build && cmake  .. && make && cd .. && ./valgrind_test.sh $1

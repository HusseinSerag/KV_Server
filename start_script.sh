#!/bin/bash
cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j && cd .. && ./build/server_exec --port=$1 --persistence=$2 --log_enabled=0
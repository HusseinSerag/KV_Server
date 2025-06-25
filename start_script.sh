#!/bin/bash
cd build && cmake  .. && make && cd .. && ./build/server_exec --port=$1 --persistence=$2 --log_enabled=0
#cd build && cmake  .. && make && cd .. && ./valgrind_test.sh $1

#!/bin/bash
cd build && cmake .. && make && cd .. && ./build/server_exec --port=$1
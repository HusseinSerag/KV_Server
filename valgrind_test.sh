#!/bin/bash
valgrind --tool=callgrind ./build/server_exec  --persistence=0 --log_enabled=0
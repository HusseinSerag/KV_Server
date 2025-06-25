#!/bin/bash
valgrind --track-origins=yes --leak-check=full ./build/server_exec  --persistence=0 --log_enabled=0 --port=${1:-3000} 
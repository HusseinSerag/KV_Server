#!/bin/bash
valgrind --leak-check=full --track-origins=yes ./build/server_exec  --persistence=0 --log_enabled=0
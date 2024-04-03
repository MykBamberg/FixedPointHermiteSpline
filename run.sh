#!/bin/bash
cd "$(dirname "$0")"

gcc -o ./bin/application ./src/main.c -lSDL2 -Wall -O2
./bin/application

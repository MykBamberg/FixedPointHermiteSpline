#!/bin/bash
cd "$(dirname "$0")"

gcc -o ./application ./src/main.c ./src/display.c -lSDL2 -Wall -O2
./application

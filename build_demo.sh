#!/bin/bash
# For Linux/Mac: Builds the "demo.cpp" program to demostrate MT's ability.

g++ -o build/MTcpp.exe -std=c++17 ./src/demo.cpp
start ./build/MTcpp.exe 
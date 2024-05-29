REM For Windows: Builds the "demo.cpp" program to demostrate MT's ability.

@echo off
if not exist build (
    mkdir build
)

g++ -o build/MTcpp.exe -std=c++17 ./src/demo.cpp
start ./build/MTcpp.exe 
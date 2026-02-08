cmake_minimum_required(VERSION 3.29)
project(ADS2)

set(CMAKE_CXX_STANDARD 20)

add_executable(ADS2 main.cpp
        RandomStreamGen.cpp
        HashFuncGen.cpp
        HyperLogLog.cpp
)

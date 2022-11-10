cmake_minimum_required(VERSION 3.20)
project(AI_Checkers)

set(CMAKE_CXX_STANDARD 14)

add_executable(AI_Checkers
        main.cpp initialize.cpp initialize.h backend.cpp backend.h)

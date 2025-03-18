cmake_minimum_required(VERSION 3.30)
project(MST)

set(CMAKE_CXX_STANDARD 20)

add_executable(MST main.cpp
        header.h
        Graph.cpp
        TSProblem.cpp
        AstarSearch.cpp)

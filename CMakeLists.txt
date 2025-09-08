cmake_minimum_required(VERSION 3.10)

project(ThermalProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(thermal src/main.cpp src/ConfigReader.cpp src/Grid.cpp)

target_include_directories(thermal PRIVATE include)

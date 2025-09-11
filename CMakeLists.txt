cmake_minimum_required(VERSION 3.10)

project(ThermalProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
    src/ConfigReader.cpp
    src/DirectSolver.cpp
    src/Grid.cpp
    src/BoundaryConditions.cpp
    src/ParserConst.cpp
    src/VTKSaver.cpp
    src/SourceFunction.cpp
    src/ThermalConductivity.cpp
    src/Solver.cpp
    src/TempSaver.cpp)

add_executable(thermal ${SOURCES})

target_include_directories(thermal PRIVATE include)

set(MUPARSER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/parsers/muparser/include)
set(MUPARSER_LIB_DIR ${CMAKE_SOURCE_DIR}/parsers/muparser/lib)

target_include_directories(thermal PRIVATE ${MUPARSER_INCLUDE_DIR})

target_link_libraries(thermal PRIVATE ${MUPARSER_LIB_DIR}/libmuparser.so)
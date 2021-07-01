cmake_minimum_required(VERSION 3.19)
project(UM_3)

set(CMAKE_CXX_STANDARD 14)

add_executable(UM-3 UM3.cpp
                   Exception.cpp
                   Exception.h
                   Memory.cpp
                   Memory.h
                   parser.cpp
                   parser.h
                   Processor.cpp
                   Processor.h
              )

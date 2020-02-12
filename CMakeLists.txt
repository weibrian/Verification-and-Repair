cmake_minimum_required(VERSION 3.14)
project(Verif CXX C)

set(CMAKE_CXX_STANDARD 11)

add_executable(Verif main.cpp DFA.c inc/DFA.h inc/array_util.h inc/array_util.h
        array_util.c examples.c inc/examples.h Property.cpp inc/Property.h)
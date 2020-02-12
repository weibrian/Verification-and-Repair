cmake_minimum_required(VERSION 3.14)
project(Verif CXX C)

set(Boost_INCLUDE_DIR /usr/local/Cellar/boost/1.72.0/include/)
set(Boost_LIBRARY_DIR /usr/local/Cellar/boost/1.72.0/include/)
include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 11)


add_executable(Verif main.cpp DFA.c inc/DFA.h inc/array_util.h inc/array_util.h
        array_util.c examples.c inc/examples.h Property.cpp inc/Property.h modify.cpp inc/modify.h)
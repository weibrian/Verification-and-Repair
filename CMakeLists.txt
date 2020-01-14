cmake_minimum_required(VERSION 3.14)
project(Verif C)

set(CMAKE_C_STANDARD 99)

add_executable(Verif main.c DFA.c inc/DFA.h inc/array_util.h inc/array_util.h array_util.c examples.c inc/examples.h)
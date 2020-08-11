cmake_minimum_required(VERSION 3.10)
project(c_mmn_14 C)

set(CMAKE_C_STANDARD 11)

add_executable(c_mmn_14 main.c code_func.c code_func.h instruction_validation.c instruction_validation.h first_transition.c first_transition.h second_transition.c second_transition.h)
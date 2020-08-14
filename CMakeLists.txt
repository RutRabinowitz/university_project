cmake_minimum_required(VERSION 3.10)
project(c_mmn_14 C)

set(CMAKE_C_STANDARD 11)

add_executable(c_mmn_14 main.c code_func.c code_func.h instruction_validation.c instruction_validation.h first_transition.c first_transition.h second_transition.c second_transition.h directive_sen.c directive_sen.h guidance_sen.c guidance_sen.h symbol_table.c symbol_table.h first_operand.c first_operand.h second_operand.c second_operand.h read_files.c read_files.h)
cmake_minimum_required(VERSION 3.10)
project(c_mmn_14 C)

set(CMAKE_C_STANDARD 11)

add_executable(c_mmn_14 main.c directives_table.c directives_table.h instruction_validation.c instruction_validation.h first_transition.c first_transition.h second_transition.c second_transition.h directive_sen.c directive_sen.h guidance_sen.c guidance_sen.h symbol_table.c symbol_table.h src_operand.c src_operand.h dst_operand.c dst_operand.h read_files.c read_write_files.h)
cmake_minimum_required(VERSION 3.17)
project(pasm_comp C)

set(CMAKE_C_STANDARD 99)

add_executable(pasm main.c pasm.h pasm_lexer.h utils.h pasm_parser.h)
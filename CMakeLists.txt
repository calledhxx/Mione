cmake_minimum_required(VERSION 3.30)
project(Mione C)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/HeadFile)

add_executable(Mione
        main.c
        FILE_TO_CASE.c
        CASE_TO_MIONE.c
        HeadFile/SET.c
        HeadFile/SVV.c
        COUNT.c MIONE.c
        SYMBOL_DEF.c
        HeadFile/AllHeads.c
        REQUEST.c
        PROMPT_DEF.c
        IMPLEMENT.c
        NUMBER.h
        NUMBER.c
        main.c
        COMPUTATION.h
        COMPUTATION.c
        STDMIO.h
)
cmake_minimum_required(VERSION 3.30)
project(Mione C)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/HeadFile)

add_executable(Mione
        main.c
        FILE_TO_CASE.c
        CASE_TO_MIONE.c
        HeadFile/RETURN.c
        HeadFile/SET.c
        HeadFile/SVV.c
        HeadFile/IF.c
        COUNT.c MIONE.c
        ERR.c
        SYMBOL_DEF.c
        HeadFile/AllHeads.c
        HeadFile/FOR.c
        REQUEST.c
        PROMPT_DEF.c
        IMPLEMENT.c
)
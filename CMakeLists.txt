cmake_minimum_required(VERSION 3.28)
project(Mione C)
set(CMAKE_C_STANDARD 11)
add_executable(Mione main.c
        main.h
        SYMBOL_DEF.h
        FILE_TO_CASE.c
        CASE_TO_MIONE.c
        HeadFile/RETURN.c
        HeadFile/SET.c
        HeadFile/SVV.c
        HeadFile/IF.c
        COUNT.c
        MIONE.c
        ERR.c
        SYMBOL_DEF.c
        HeadFile/AllHeads.c

)
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
        main.c
        COMPUTATION.h
        COMPUTATION.c
        STDMIO.h
        VARIABLE_PROCESSING.c
        VARIABLE_PROCESSING.h
        HeadFile/RETURN.c
        HeadFile/GET.h
        HeadFile/GET.c
        HeadFile/LIBRARY.h
        HeadFile/LIBRARY.c
        HeadFile/ENTER.h
        HeadFile/ENTER.c
        HeadFile/INCLUDE.h
        HeadFile/INCLUDE.c
)
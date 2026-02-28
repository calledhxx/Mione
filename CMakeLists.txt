cmake_minimum_required(VERSION 3.31)
project(New_Mione C)

set(CMAKE_C_STANDARD 11)

add_executable(New_Mione main.c
        memory.c
        word.c
        object.c
        head.c
        head.h
        prompt.c
        prompt.h
        symbol.c
        symbol.h
        train.c
        train.h
        value.c
        value.h
        variable.c
        variable.h
        weld.c
        weld.h
        word.h
        instruct.c
        instruct.h
        run.h
        run.c
        calculate.c
        calculate.h
)

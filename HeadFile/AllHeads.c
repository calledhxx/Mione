//
// Created by calle on 24-12-28.
//
#include <stdlib.h>

#include "../OBJECTS.h"

#include "SET.h"
#include "RETURN.h"
#include "IF.h"
#include "FOR.h"

HeadObj Heads[]  = {
    (HeadObj){
        .Fuc = SET,
        .Name = "set",
        .CurNumber = 1
    },
    (HeadObj){
        .Fuc = RETURN,
        .Name = "return",
        .CurNumber = 2
    },
    (HeadObj){
        .Fuc = IF,
        .Name = "if",
        .CurNumber = 3
    },
    (HeadObj){
        .Fuc = FOR,
        .Name = "for",
        .CurNumber = 4
    },

    //記得到MIONE.c新增可執行CurNumber
    (HeadObj){
        .CurNumber = -1
    }
};
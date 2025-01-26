//
// Created by calle on 24-12-28.
//
#include <stdlib.h>

#include "../OBJECTS.h"

#include "SET.h"
#include "RETURN.h"
#include "IF.h"

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
        .CurNumber = 2
    },


    (HeadObj){
        .CurNumber = -1
    }
};
#include "../OBJECTS.h"

#include "SET.h"
#include "RETURN.h"
#include "IF.h"
#include "FOR.h"


HeadObj Heads[]  = {
    (HeadObj){
        .Fuc = SET,
        .Name = L"set",
        .CurNumber = 1,
    },
    (HeadObj){
        .Fuc = RETURN,
        .Name = L"return",
        .CurNumber = 2
    },
    (HeadObj){
        .Fuc = IF,
        .Name = L"if",
        .CurNumber = 3
    },
    (HeadObj){
        .Fuc = FOR,
        .Name = L"for",
        .CurNumber = 4
    },

    (HeadObj){
        .CurNumber = -1
    }
};
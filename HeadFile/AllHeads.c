#include "../OBJECTS.h"

#include "SET.h"
#include "RETURN.h"
#include "SVV.h"

const HeadObj Heads[]  = {
    (HeadObj){
        .Fuc = SET,
        .Name = "set",
        .Identification = 1,
    },

    (HeadObj){
        .Fuc = RETURN,
        .Name = "return",
        .Identification = 1,
    },

    (HeadObj){
        .Identification = 0
    }
};
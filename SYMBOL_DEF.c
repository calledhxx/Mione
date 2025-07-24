#include <stdlib.h>

#include "OBJECTS.h"

SymbolObj Symbols[] =
{
    (SymbolObj){
        .Name = "+",
        .Identification = 1,
        
        .AfterConnectVV = 1,
    },

    (SymbolObj){
        .Name = ",",
        .Identification = 2,
        
        .AfterConnectVV = 1,
    },

    (SymbolObj){
        .Name = "*",
        .Identification = 3,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = "/",
        .Identification = 4,
        .AfterConnectVV = 1,
       
    },



    (SymbolObj){
        .Name = "^",
        .Identification = 5,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = "-",
        .Identification = 6,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = ".",
        .Identification = 7,
        .AfterConnectVV = 1,
       
    },
    (SymbolObj){
        .Name = "==",
        .Identification = 8,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = "[",
        .Identification = 10,

        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = "]",
        .Identification = 11,

        .AfterConnectVV = 0,
       
    },
    (SymbolObj){
        .Name = "(",
        .Identification = 12,

        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = ")",
        .Identification = 13,

        .AfterConnectVV = 0,
       
    },
    (SymbolObj){ //衛兵值
        .Identification = 0
    }
};
#include <stdlib.h>

#include "OBJECTS.h"

SymbolObj Symbols[] =
{
    (SymbolObj){
        .Name = L"+",
        .Identification = 1,
        
        .AfterConnectVV = 1,
    },

    (SymbolObj){
        .Name = L",",
        .Identification = 2,
        
        .AfterConnectVV = 1,
    },

    (SymbolObj){
        .Name = L"*",
        .Identification = 3,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = L"/",
        .Identification = 4,
        .AfterConnectVV = 1,
       
    },



    (SymbolObj){
        .Name = L"^",
        .Identification = 5,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = L"-",
        .Identification = 6,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = L".",
        .Identification = 7,
        .AfterConnectVV = 1,
       
    },
    (SymbolObj){
        .Name = L"==",
        .Identification = 8,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = L"[",
        .Identification = 10,

        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = L"]",
        .Identification = 11,

        .AfterConnectVV = 0,
       
    },
    (SymbolObj){
        .Name = L"(",
        .Identification = 12,

        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = L")",
        .Identification = 13,

        .AfterConnectVV = 0,
       
    },
    (SymbolObj){ //衛兵值
        .Identification = -1
    }
};
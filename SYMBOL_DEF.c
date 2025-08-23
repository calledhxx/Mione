#include <stdlib.h>

#include "OBJECTS.h"

SymbolObj Symbols[] =
{
    (SymbolObj){
        .Name = "+",
        .Identification = 1,

        .SymbolCarry =
             SC_AfterVariableOrValue ,

        //1 + 1
        //(1+1) + 1
        // 1 + -2
    },

    (SymbolObj){
        .Name = ",",
        .Identification = 2,

        .SymbolCarry =
             SC_AfterVariableOrValue ,
        //1,1
        //(1),(2)
    },

    (SymbolObj){
        .Name = "*",
        .Identification = 3,
        .SymbolCarry =
             SC_AfterVariableOrValue
    },

    (SymbolObj){
        .Name = "/",
        .Identification = 4,
        .SymbolCarry =
             SC_AfterVariableOrValue ,

    },



    (SymbolObj){
        .Name = "^",
        .Identification = 5,
        .SymbolCarry =
             SC_AfterVariableOrValue ,

    },

    (SymbolObj){
        .Name = "-",
        .Identification = 6,
        .SymbolCarry =
            SC_BeforeVariableOrValue |  SC_AfterVariableOrValue |  SC_AfterSymbol,

    },

    (SymbolObj){
        .Name = ".",
        .Identification = 7,
        .SymbolCarry =
             SC_AfterVariableOrValue ,


    },
    (SymbolObj){
        .Name = "==",
        .Identification = 8,
        .SymbolCarry =
             SC_AfterVariableOrValue ,


    },

    (SymbolObj){
        .Name = "[",
        .Identification = 10,
        .SymbolCarry =
            SC_BeforeVariableOrValue | SC_AfterVariableOrValue ,
    },

    (SymbolObj){
        .Name = "]",
        .Identification = 11,
        .SymbolCarry =
            SC_AfterVariableOrValue ,



    },
    (SymbolObj){
        .Name = "(",
        .Identification = 12,
        .SymbolCarry =
             SC_BeforeVariableOrValue,


    },

    (SymbolObj){
        .Name = ")",
        .Identification = 13,
        .SymbolCarry =
            SC_AfterVariableOrValue,


    },
    (SymbolObj){ //衛兵值
        .Identification = 0
    }
};
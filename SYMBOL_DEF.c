#include <stdlib.h>
#include <string.h>

#include "OBJECTS.h"



SymbolObjCarrier ReturnSymbolList()
{
    const SymbolObj Symbols[] =
{
        (SymbolObj){
            .Name = "+",
            .Identification = SYMBOL_ADD,

            .SymbolCarry =
                 SC_AfterVariableOrValue | SC_BeforeVariableOrValue ,

            //1 + 1
            //(1+1) + 1
            // 1 + -2
        },

        (SymbolObj){
            .Name = ",",
            .Identification = SYMBOL_COMMA,

            .SymbolCarry =
                 SC_AfterVariableOrValue ,
            //1,1
            //(1),(2)
        },

        (SymbolObj){
            .Name = "*",
            .Identification = SYMBOL_STAR,
            .SymbolCarry =
                 SC_AfterVariableOrValue
        },

        (SymbolObj){
            .Name = "/",
            .Identification = SYMBOL_SLASH,
            .SymbolCarry =
                 SC_AfterVariableOrValue ,

        },



        (SymbolObj){
            .Name = "^",
            .Identification = SYMBOL_UP,
            .SymbolCarry =
                 SC_AfterVariableOrValue ,

        },

        (SymbolObj){
            .Name = "-",
            .Identification = SYMBOL_SUB,
            .SymbolCarry =
                SC_BeforeVariableOrValue |  SC_AfterVariableOrValue |  SC_AfterSymbol,

        },

        (SymbolObj){
            .Name = ".",
            .Identification = SYMBOL_DOT,
            .SymbolCarry =
                 SC_AfterVariableOrValue ,


        },
        (SymbolObj){
            .Name = "==",
            .Identification = SYMBOL_IS,
            .SymbolCarry =
                 SC_AfterVariableOrValue ,


        },

        (SymbolObj){
            .Name = "[",
            .Identification = SYMBOL_FRONT_BRACKET,
            .SymbolCarry =
                SC_BeforeVariableOrValue | SC_AfterVariableOrValue ,
        },

        (SymbolObj){
            .Name = "]",
            .Identification = SYMBOL_BACK_BRACKET,
            .SymbolCarry =
                SC_AfterVariableOrValue ,



        },
        (SymbolObj){
            .Name = "(",
            .Identification = SYMBOL_FRONT_PARENTHESES,
            .SymbolCarry =
                 SC_BeforeVariableOrValue,


        },

        (SymbolObj){
            .Name = ")",
            .Identification = SYMBOL_BACK_PARENTHESES,
            .SymbolCarry =
                SC_AfterVariableOrValue,


        }
};

    SymbolObjCarrier result = {0};
    result.CarrierLen =
        sizeof(Symbols)/sizeof(Symbols[0]);
    result.Carrier =
        malloc(sizeof(Symbols));

    memcpy(
        result.Carrier,
        Symbols,
        sizeof(Symbols)
        );

    return result;
}
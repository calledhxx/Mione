#include <string.h>

#include "main.h"

static keyword_t SymbolKeywordArray[] = {
    {
        .name = "+",
        .token = TOKEN_SYMBOL_PLUS,
        .instance = &(symbol_t){
            .connect_condition_flag =
                SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV |
                    SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV |
                        SYMBOL_CONNECT_CONDITION_FLAG_MODESTY,
            .order = 2,
            .calculate_allow_position_flag =
                SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE
        },
    },
{
    .name = "-",
    .token = TOKEN_SYMBOL_MINUS,
    .instance = &(symbol_t){
        .connect_condition_flag =
            SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV |
                SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV |
                    SYMBOL_CONNECT_CONDITION_FLAG_AFTER_SYMBOL |
                        SYMBOL_CONNECT_CONDITION_FLAG_MODESTY,
        .order = 2,
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE
        }
    },
    {0}
};

keyword_t * SymbolKeyword = SymbolKeywordArray;

symbol_t token_to_symbol(const int token)
{
    for (unsigned i = 0; i < sizeof(SymbolKeywordArray) / sizeof(keyword_t); i++)
        if (SymbolKeywordArray[i].token == token)
            return *(symbol_t*)SymbolKeywordArray[i].instance;

    return (symbol_t){0};
}
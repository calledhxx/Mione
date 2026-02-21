#include <string.h>

#include "object.h"
#include "symbol.h"

static keyword_t SymbolKeywordArray[] = {
    {
        .name = "+",
        .token = TOKEN_SYMBOL_PLUS,
        .instance = &(symbol_t){
            .connect_condition_flag =
                SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV |
                    SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV |
                        SYMBOL_CONNECT_CONDITION_FLAG_MODESTY
        }
    },
{
    .name = "-",
    .token = TOKEN_SYMBOL_MINUS,
    .instance = &(symbol_t){
        .connect_condition_flag =
            SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV |
                SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV |
                    SYMBOL_CONNECT_CONDITION_FLAG_AFTER_SYMBOL |
                        SYMBOL_CONNECT_CONDITION_FLAG_MODESTY
        }
    },
    {0}
};

keyword_t * SymbolKeyword = SymbolKeywordArray;

symbol_t token_to_symbol(const enum token_e token)
{
    for (unsigned i = 0; i < sizeof(SymbolKeywordArray) / sizeof(keyword_t); i++)
        if (SymbolKeywordArray[i].token == token)
            return *(symbol_t*)SymbolKeywordArray[i].instance;

    return {0};
}
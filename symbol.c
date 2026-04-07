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
            .calculate_allow_position_flag =
                SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE,
            .instruct_information = (information_t[3]){
                {
                    .after_count = 2,
                    .instruct = INSTRUCT_ADD,
                    .order = 2
                },
            }
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
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE |
                SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_BEFORE,
        .instruct_information = (information_t[3]){
                    {
                        .after_count = 2,
                        .instruct = INSTRUCT_SUB,
                        .order = 2
                    },
                {
                        .after_count = 1,
                        .instruct = INSTRUCT_NEGATIVE,
                        .order = 0
                    },
                }
    },
},
{
    .name = "*",
    .token = TOKEN_SYMBOL_MUL,
    .instance = &(symbol_t){
        .connect_condition_flag =
            SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV |
                    SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV |
                        SYMBOL_CONNECT_CONDITION_FLAG_MODESTY,
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE,
        .instruct_information = (information_t[3]){
                    {
                        .after_count = 2,
                        .instruct = INSTRUCT_MUL,
                        .order = 1
                    },
        }
    },
},
{
    .name = "/",
    .token = TOKEN_SYMBOL_DIV,
    .instance = &(symbol_t){
        .connect_condition_flag =
            SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV |
                SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV |
                        SYMBOL_CONNECT_CONDITION_FLAG_MODESTY,
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE,
        .instruct_information = (information_t[3]){
            {
                .after_count = 2,
                .instruct = INSTRUCT_DIV,
                .order = 1
            },
        }
        }
    },
{
    .name = "(",
    .token = TOKEN_SYMBOL_OPENING_PARENTHESIS,
    .instance = &(symbol_t){
        .connect_condition_flag =
            SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV |
                SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV |
                    SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_SYMBOL |
                        SYMBOL_CONNECT_CONDITION_FLAG_AFTER_SYMBOL |
                            SYMBOL_CONNECT_CONDITION_FLAG_MODESTY,
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_SPECIAL,
        }
    },
{
    .name = ")",
    .token = TOKEN_SYMBOL_CLOSING_PARENTHESIS,
    .instance = &(symbol_t){
        .connect_condition_flag =
            SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV |
                SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV |
                    SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_SYMBOL |
                        SYMBOL_CONNECT_CONDITION_FLAG_AFTER_SYMBOL |
                            SYMBOL_CONNECT_CONDITION_FLAG_MODESTY,
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_SPECIAL,
        }
},{
    .name = "[",
    .token = TOKEN_SYMBOL_OPENING_BRACKET,
    .instance = &(symbol_t){
        .connect_condition_flag =
            SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV |
                SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV |
                    SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_SYMBOL |
                        SYMBOL_CONNECT_CONDITION_FLAG_AFTER_SYMBOL |
                            SYMBOL_CONNECT_CONDITION_FLAG_MODESTY,
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_SPECIAL,
    }
},
{
    .name = "]",
    .token = TOKEN_SYMBOL_CLOSING_BRACKET,
    .instance = &(symbol_t){
        .connect_condition_flag =
            SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV |
                SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV |
                    SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_SYMBOL |
                        SYMBOL_CONNECT_CONDITION_FLAG_AFTER_SYMBOL |
                            SYMBOL_CONNECT_CONDITION_FLAG_MODESTY,
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_SPECIAL,
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
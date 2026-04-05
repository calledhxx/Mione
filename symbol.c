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
                SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE,
            .instruct_information = (instruct_information_t[3]){
                {
                    .after_count = 2,
                    .instruct = INSTRUCT_ADD
                },
                {
                    .after_count = 0,
                    .instruct = INSTRUCT_NONE
                },
                {
                    .after_count = 0,
                    .instruct = INSTRUCT_NONE
                }
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
        .order = 1,
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE,
        .instruct_information = (instruct_information_t[3]){
                    {
                        .after_count = 2,
                        .instruct = INSTRUCT_MUL
                    },
                    {
                        .after_count = 0,
                        .instruct = INSTRUCT_NONE
                    },
                    {
                        .after_count = 0,
                        .instruct = INSTRUCT_NONE
                    }
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
        .order = 1,
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE,
        .instruct_information = (instruct_information_t[3]){
            {
                .after_count = 2,
                .instruct = INSTRUCT_DIV
            },
            {
                .after_count = 0,
                .instruct = INSTRUCT_NONE
            },
            {
                .after_count = 0,
                .instruct = INSTRUCT_NONE
            }
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
                    SYMBOL_CONNECT_CONDITION_FLAG_AFTER_SYMBOL |
                        SYMBOL_CONNECT_CONDITION_FLAG_MODESTY,
        .order = 0,
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_SPECIAL,
        .instruct_information = (instruct_information_t[3]){
                {
                    .after_count = 1,
                    .instruct = INSTRUCT_NONE
                },
                {
                    .after_count = 1,
                    .instruct = INSTRUCT_NONE
                },
                {
                    .after_count = 1,
                    .instruct = INSTRUCT_NONE
                }
            }
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
                        SYMBOL_CONNECT_CONDITION_FLAG_MODESTY,
        .order = 0,
        .calculate_allow_position_flag =
            SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_SPECIAL,
        .instruct_information = (instruct_information_t[3]){
                    {
                        .after_count = 1,
                        .instruct = INSTRUCT_NONE
                    },
                    {
                        .after_count = 1,
                        .instruct = INSTRUCT_NONE
                    },
                    {
                        .after_count = 1,
                        .instruct = INSTRUCT_NONE
                    }
            }
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
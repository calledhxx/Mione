#ifndef SYMBOL_H
#define SYMBOL_H

extern keyword_t * SymbolKeyword;

enum symbol_connect_condition_flag_e
{
    SYMBOL_CONNECT_CONDITION_FLAG_NONE = 0,
    SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV = 1,
    SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV = 2,
    SYMBOL_CONNECT_CONDITION_FLAG_AFTER_SYMBOL = 4,
    SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_SYMBOL = 8,
    SYMBOL_CONNECT_CONDITION_FLAG_MODESTY = 16,
};

enum symbol_calculate_allow_position_flag_e
{
    SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_NONE = 0,
    SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_AFTER = 1,
    SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE = 2,
    SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_BEFORE = 4,
};

typedef struct symbol_s
{
    enum symbol_connect_condition_flag_e connect_condition_flag;
    enum symbol_calculate_allow_position_flag_e calculate_allow_position_flag;

    unsigned order;
} symbol_t;

extern symbol_t token_to_symbol(const int token);


#endif

#ifndef SYMBOL_H
#define SYMBOL_H

extern keyword_t * SymbolKeyword;

enum symbol_connect_condition_flag_e
{
    SYMBOL_CONNECT_CONDITION_FLAG_NONE = 0,
    SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV = 1<<0,
    SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV = 1<<1,
    SYMBOL_CONNECT_CONDITION_FLAG_AFTER_SYMBOL = 1<<2,
    SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_SYMBOL = 1<<3,
    SYMBOL_CONNECT_CONDITION_FLAG_MODESTY = 1<<4,
};

enum symbol_calculate_allow_position_flag_e
{
    SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_NONE = 0,
    SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_AFTER = 1<<0,
    SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE = 1<<1,
    SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_BEFORE = 1<<2,
};
typedef struct instruct_information_s
{
    unsigned char after_count;
    unsigned int instruct;
} instruct_information_t;

typedef struct instruct_information_carrier_s
{
    instruct_information_t* instruct_information;
    unsigned instruct_information_length;
} instruct_information_carrier_t;


typedef struct symbol_s
{
    enum symbol_connect_condition_flag_e connect_condition_flag;
    enum symbol_calculate_allow_position_flag_e calculate_allow_position_flag;
    instruct_information_t instruct_information[3];

    unsigned order;
} symbol_t;

extern symbol_t token_to_symbol(const int token);


#endif

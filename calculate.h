#ifndef CALCULATOR_H
#define CALCULATOR_H

enum symbol_calculate_state_e
{
    SYMBOL_CALCULATE_STATE_NONE,
    SYMBOL_CALCULATE_STATE_MIDDLE,
    SYMBOL_CALCULATE_STATE_BEFORE,
    SYMBOL_CALCULATE_STATE_AFTER,
};

enum calculate_option_flag_e
{
    CALCULATE_OPTION_FLAG_NONE = 0,
    CALCULATE_OPTION_FLAG_FREE_BRACKET_ALLOWED = 1<<0,
};

instruct_carrier_t calculate(object_carrier_t object_carrier);

#endif

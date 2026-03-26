#ifndef CALCULATOR_H
#define CALCULATOR_H

enum symbol_calculate_state_e
{
    SYMBOL_CALCULATE_STATE_NONE,
    SYMBOL_CALCULATE_STATE_MIDDLE,
    SYMBOL_CALCULATE_STATE_BEFORE,
    SYMBOL_CALCULATE_STATE_AFTER,
};

instruct_carrier_t calculate(object_carrier_t object_carrier);

#endif

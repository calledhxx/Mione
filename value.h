#ifndef VALUE_H
#define VALUE_H

#include "train.h"

enum value_type_e
{
    VALUE_NONE,
    VALUE_STRING,
    VALUE_FUNCTION,
    VALUE_RANGE,
    VALUE_TABLE,
    VALUE_BOOL,
    VALUE_NUMBER,
    VALUE_WINDOWS_LIBRARY,
};

typedef struct value_s
{
    enum value_type_e value_type;
    union
    {
        char* string;
        double number;
        char boolean;
        train_carrier_t train_carrier;
    } value;
} value_t;

#endif

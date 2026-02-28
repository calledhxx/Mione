#include <stdlib.h>

#include "main.h"
#define _max(a,b) a>b?a:b

instruct_carrier_t calculate(object_carrier_t object_carrier)
{
    instruct_carrier_t result = {0};

    object_carrier_t parentheses_layout = {0};
    unsigned parentheses_depth = 0;

    unsigned requested_order = 2;

    for (unsigned order = 0; order < requested_order; order++)
    for (int i = 0; i < object_carrier.objects_length; i++)
    {
        object_t const ThisObject = object_carrier.objects[i];

        if (!order)
        {
            if (ThisObject.token == TOKEN_SYMBOL_OPENING_PARENTHESIS)
            {
                if (parentheses_depth)
                {
                    parentheses_layout = {0};
                }
                parentheses_depth++;
            }else if (ThisObject.token == TOKEN_SYMBOL_CLOSING_PARENTHESIS)
            {
                parentheses_depth--;

                if (parentheses_depth)
                {
                    instruct_carrier_t const carrier = calculate(parentheses_layout);
                    free(parentheses_layout.objects);

                    pushInstructsIntoCarrier(&result,carrier);
                }
            }
        }
        else
        {
            if (parentheses_depth)
            {
                parentheses_layout.objects_length++;
                parentheses_layout.objects = realloc(parentheses_layout.objects,parentheses_layout.objects_length * sizeof(object_t));
                parentheses_layout.objects[parentheses_layout.objects_length-1] =
                    ThisObject;
            }else switch (ThisObject.object_type)
            {
                case OBJECT_SYMBOL:
                    {
                        symbol_t const symbol = token_to_symbol(ThisObject.token);

                        if (symbol.order > order)
                            requested_order = _max(order,symbol.order);
                        else if (symbol.order < order)
                            continue;

                        enum symbol_calculate_allow_position_flag_e state = SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_NONE;

                        if (symbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE)
                            if (i - 1 >= 0 && i + 1 < object_carrier.objects_length)
                            {
                                state = SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE;
                                goto jump;
                            }

                        if (symbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_BEFORE)
                            if (i + 1 < object_carrier.objects_length)
                            {
                                state = SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_BEFORE;
                                goto jump;
                            }

                        if (symbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_AFTER)
                            if (i - 1 >= 0)
                            {
                                state = SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_AFTER;
                                goto jump;
                            }

                        exit(5);

                        jump:

                        switch (ThisObject.token)
                        {
                        case TOKEN_SYMBOL_PLUS:
                            {
                                instruct_carrier_t const carrier1 = loadVV(&object_carrier.objects[i-1]);
                                pushInstructsIntoCarrier(&result,carrier1);

                                instruct_carrier_t const carrier2 = loadVV(&object_carrier.objects[i+1]);
                                pushInstructsIntoCarrier(&result,carrier2);

                                pushInstructIntoCarrier(&result,(instruct_t){
                                    .instruct = INSTRUCT_ADD,
                                    .object = 0
                                });
                                break;
                            }
                        case TOKEN_SYMBOL_MINUS:
                            {
                                instruct_carrier_t const carrier1 = loadVV(&object_carrier.objects[i-1]);
                                pushInstructsIntoCarrier(&result,carrier1);

                                instruct_carrier_t const carrier2 = loadVV(&object_carrier.objects[i+1]);
                                pushInstructsIntoCarrier(&result,carrier2);

                                pushInstructIntoCarrier(&result,(instruct_t){
                                    .instruct = INSTRUCT_SUB,
                                    .object = 0
                                });
                                break;
                            }
                        default:
                            exit(5);
                        }
                        break;
                    }
                case OBJECT_VARIABLE:
                    {

                        break;
                    }
                case OBJECT_VALUE:
                    {

                        break;
                    }
                default:
                exit(5);
            }


        }
    }

    return result;
}
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "main.h"
#include <crtdbg.h>

#define _max(a,b) ((a) > (b) ? (a) : (b))

instruct_carrier_t cal_ast(
    object_carrier_t carrier
    )
{
    calculate_layout_t layout_stack[32] = {0};
    calculate_layout_t * layout_stack_top = layout_stack + 31;
    
    for (int i = 0; i < carrier.objects_length; i++)
    {
        object_t const object = carrier.objects[i];

        switch (object.object_type)
        {
        case OBJECT_SYMBOL:
            {
                symbol_t const symbol = token_to_symbol(object.token);
                operator_t operator = {0};

                if (symbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_SPECIAL)
                {
                    switch (object.token)
                    {
                    case TOKEN_SYMBOL_OPENING_PARENTHESIS:
                        {
                            --layout_stack_top;

                            if (i-1 >=0 && (carrier.objects[i-1].object_type == OBJECT_VARIABLE || carrier.objects[i-1].object_type == OBJECT_VALUE))
                                layout_stack_top->calculate_layout_type = CALCULATE_LAYOUT_TYPE_INTEND_TO_CALL;

                            break;
                        }
                    case TOKEN_SYMBOL_CLOSING_PARENTHESIS:
                        {
                            for (int j = layout_stack_top->operator_carrier.operators_length - 1; j >= 0; j--){
                                layout_stack_top->operator_carrier.operators_length--;

                                layout_stack_top->instruct_carrier.instructs_length++;
                                layout_stack_top->instruct_carrier.instructs = alc(
                                    layout_stack_top->instruct_carrier.instructs,
                                    layout_stack_top->instruct_carrier.instructs_length * sizeof(instruct_t)
                                    );
                                layout_stack_top->instruct_carrier.instructs[layout_stack_top->instruct_carrier.instructs_length-1] = (instruct_t){
                                    .instruct = layout_stack_top->operator_carrier.operators[j].instruct,
                                    .object = 0
                                };
                            }

                            if (layout_stack_top->calculate_layout_type == CALCULATE_LAYOUT_TYPE_INTEND_TO_CALL)
                            {
                                (layout_stack_top+1)->instruct_carrier.instructs_length++;
                                (layout_stack_top+1)->instruct_carrier.instructs = alc(
                                    (layout_stack_top+1)->instruct_carrier.instructs,
                                    (layout_stack_top+1)->instruct_carrier.instructs_length * sizeof(instruct_t)
                                    );
                                (layout_stack_top+1)->instruct_carrier.instructs[(layout_stack_top+1)->instruct_carrier.instructs_length-1] = (instruct_t){
                                    .instruct = INSTRUCT_CALL,
                                    .object = layout_stack_top->instruct_carrier.instructs_length
                                };
                            };

                            pushInstructsIntoCarrier(&(layout_stack_top+1)->instruct_carrier,(layout_stack_top)->instruct_carrier);

                            ++layout_stack_top;

                            break;
                        }

                        //todo
                    default:
                        exit(32);
                    }
                }else
                {
                    if (symbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE)
                        if (i - 1 >= 0 && i + 1 < carrier.objects_length)
                        {
                            operator = symbol.operators.MIDDLE;
                            goto capf_end;
                        }

                    if (symbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_AFTER)
                        if (i - 1 >= 0)
                        {
                            operator = symbol.operators.AFTER;
                            goto capf_end;
                        }

                    if (symbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_BEFORE)
                        if (i + 1 < carrier.objects_length)
                        {
                            operator = symbol.operators.BEFORE;
                            goto capf_end;
                        }

                    exit(99);

                    capf_end:

                    for (int j = layout_stack_top->operator_carrier.operators_length - 1; j >= 0; j--)
                        if (layout_stack_top->operator_carrier.operators[j].order >= operator.order)
                        {
                            layout_stack_top->operator_carrier.operators_length--;
                            //dangerous

                            layout_stack_top->instruct_carrier.instructs_length++;
                            layout_stack_top->instruct_carrier.instructs = alc(
                                layout_stack_top->instruct_carrier.instructs,
                                layout_stack_top->instruct_carrier.instructs_length * sizeof(instruct_t)
                                );
                            layout_stack_top->instruct_carrier.instructs[layout_stack_top->instruct_carrier.instructs_length-1] = (instruct_t){
                                .instruct = layout_stack_top->operator_carrier.operators[j].instruct,
                                .object = 0
                            };
                        }else
                            break;

                    layout_stack_top->operator_carrier.operators_length++;
                    layout_stack_top->operator_carrier.operators = alc(
                        layout_stack_top->operator_carrier.operators,
                    layout_stack_top->operator_carrier.operators_length * sizeof(operator_t)
                    );
                    layout_stack_top->operator_carrier.operators[layout_stack_top->operator_carrier.operators_length-1] = operator;
                }

                break;
            }
        case OBJECT_VARIABLE:
            {
                layout_stack_top->instruct_carrier.instructs_length+=2;
                layout_stack_top->instruct_carrier.instructs = alc(
                    layout_stack_top->instruct_carrier.instructs,
                    layout_stack_top->instruct_carrier.instructs_length * sizeof(instruct_t)
                    );
                layout_stack_top->instruct_carrier.instructs[layout_stack_top->instruct_carrier.instructs_length-2] = (instruct_t){
                    .instruct = INSTRUCT_LOAD_VARIABLE,
                    .object = (unsigned long long)(carrier.objects + i)
                };
                layout_stack_top->instruct_carrier.instructs[layout_stack_top->instruct_carrier.instructs_length-1] = (instruct_t){
                    .instruct = INSTRUCT_TO_VALUE,
                };

                break;
            }
        case OBJECT_VALUE:
            {
                layout_stack_top->instruct_carrier.instructs_length++;
                layout_stack_top->instruct_carrier.instructs = alc(
                    layout_stack_top->instruct_carrier.instructs,
                    layout_stack_top->instruct_carrier.instructs_length * sizeof(instruct_t)
                    );
                layout_stack_top->instruct_carrier.instructs[layout_stack_top->instruct_carrier.instructs_length-1] = (instruct_t){
                    .instruct = INSTRUCT_LOAD_VALUE,
                    .object = (unsigned long long)(carrier.objects + i)
                };

                break;
            }
        default:
            exit(123);
        }
    }

    for (int j = layout_stack_top->operator_carrier.operators_length - 1; j >= 0; j--){
        layout_stack_top->operator_carrier.operators_length--;

        layout_stack_top->instruct_carrier.instructs_length++;
        layout_stack_top->instruct_carrier.instructs = alc(
            layout_stack_top->instruct_carrier.instructs,
            layout_stack_top->instruct_carrier.instructs_length * sizeof(instruct_t)
            );
        layout_stack_top->instruct_carrier.instructs[layout_stack_top->instruct_carrier.instructs_length-1] = (instruct_t){
            .instruct = layout_stack_top->operator_carrier.operators[j].instruct,
            .object = 0
        };
    }

    return layout_stack_top->instruct_carrier;
}
instruct_carrier_t calculate(object_carrier_t const object_carrier,char const calculate_as_container)
{
    instruct_carrier_t const result = cal_ast(
        object_carrier);

    printf("result:\n");
    print_instruct_carrier(result);
    printf(":end\n");

    return result;
}

/*
 1 + 2 * 3 ^ 4 - 5

 1 2
 +

 1 2 3
 + *

 1 2 3 4
 + * ^

 1 2 3 4 ^
 + *

 1 2 3 4 ^ *
 +

 1 2 3 4 ^ * +

 1 2 3 4 ^ * + 5
 -

 1 2 3 4 ^ * + 5 -
*/

/*
 1 + 2()

 1

 1
 +

 1 2
 +

 1 2
 + @
*/
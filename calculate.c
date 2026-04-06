#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "main.h"
#include <crtdbg.h>

#define _max(a,b) ((a) > (b) ? (a) : (b))

static void push(const object_carrier_t carrier,object_carrier_container_t * container_ptr)
{
    container_ptr->object_carriers_length++;
    container_ptr->object_carriers = realloc(
        container_ptr->object_carriers,
        container_ptr->object_carriers_length * sizeof(object_carrier_t)
        );
    container_ptr->object_carriers[container_ptr->object_carriers_length - 1] = carrier;
}


instruct_carrier_t cal_ast(object_carrier_t carrier)
{

    instruct_carrier_t result = {0};

    int highest_order = -INT_MAX;

    char preprocess_stack[32] = {0};
    char * preprocess_stack_top = preprocess_stack + 31;

    while (
        carrier.objects[0].token == TOKEN_SYMBOL_OPENING_PARENTHESIS
        &&
        carrier.objects[carrier.objects_length - 1].token == TOKEN_SYMBOL_CLOSING_PARENTHESIS
        )
    {
        for (int i = 1;i<carrier.objects_length - 1; i++)
            if (carrier.objects[i].token == TOKEN_SYMBOL_OPENING_PARENTHESIS)
                goto simple;
            else if(carrier.objects[i].token == TOKEN_SYMBOL_CLOSING_PARENTHESIS)
                goto keep;

        simple:;
        carrier.objects++;
        carrier.objects_length -= 2;
        keep:;
    }

    for (unsigned i = 0; i < carrier.objects_length; i++)
    {

        if (carrier.objects[i].object_type == OBJECT_SYMBOL)
        {
            symbol_t const ThisSymbol = token_to_symbol(carrier.objects[i].token);

            if (carrier.objects[i].token == TOKEN_SYMBOL_OPENING_PARENTHESIS)
            {
                *preprocess_stack_top = TOKEN_SYMBOL_OPENING_PARENTHESIS;
                preprocess_stack_top--;
            }

            highest_order = _max((preprocess_stack - preprocess_stack_top + 31) ? 0 : (int)ThisSymbol.order,highest_order);

            if (carrier.objects[i].token == TOKEN_SYMBOL_CLOSING_PARENTHESIS)
            {

                preprocess_stack_top++;

                if (*preprocess_stack_top != TOKEN_SYMBOL_OPENING_PARENTHESIS)
                    exit(101);
                *preprocess_stack_top = 0;
            }

        }
    }

    if (preprocess_stack - preprocess_stack_top + 31)
        exit(100);

    int preprocess_depth = 0;
    
    object_carrier_container_t object_carrier_container = {0};
    object_carrier_t object_carrier = {0};

    instruct_information_t information_stack[32] = {0};
    instruct_information_t * information_stack_top = information_stack + 31;



    for (int i = 0; i < carrier.objects_length; i++)
    {
        const object_t ThisObj = carrier.objects[i];

        if (ThisObj.object_type == OBJECT_SYMBOL)
        {
            symbol_t const ThisSymbol = token_to_symbol(ThisObj.token);

            if (carrier.objects[i].token == TOKEN_SYMBOL_OPENING_PARENTHESIS)
                preprocess_depth++;

            if (highest_order == (preprocess_depth ? 0 : ThisSymbol.order))
            {
                if (object_carrier.objects_length && object_carrier.objects)
                    push(object_carrier,&object_carrier_container);

                object_carrier = (object_carrier_t){0};

                enum symbol_calculate_allow_position_flag_e final_flag = SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_NONE;

                if ((final_flag = ThisSymbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_SPECIAL))
                {
                    goto capf_end;
                }

                if ((final_flag = ThisSymbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE))
                    if (i - 1 >= 0 && i + 1 < carrier.objects_length)
                        goto capf_end;

                if ((final_flag = ThisSymbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_BEFORE))
                    if (i + 1 < carrier.objects_length)
                        goto capf_end;

                if ((final_flag = ThisSymbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_AFTER))
                    if (i - 1 >= 0)
                        goto capf_end;

                exit(32);

                capf_end:

                instruct_information_t information = (instruct_information_t){0};

                switch (final_flag)
                {
                case SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE:
                    {
                        information = ThisSymbol.instruct_information[0];

                        if (information.instruct == INSTRUCT_NONE)
                            exit(9);

                        break;
                    }
                case SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_BEFORE:
                    {
                        information = ThisSymbol.instruct_information[1];

                        if (information.instruct == INSTRUCT_NONE)
                            exit(9);

                        break;
                    }
                case SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_AFTER:
                    {
                        information = ThisSymbol.instruct_information[2];

                        if (information.instruct == INSTRUCT_NONE)
                            exit(9);

                        break;
                    }
                case SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_SPECIAL:{
                        highest_order = 1;
                        information = (instruct_information_t){
                            .instruct = INSTRUCT_CALL,
                            .after_count = 2
                        };

                        i --;

                        break;
                    }
                default: exit(11);
                }

                *information_stack_top = information;

                information_stack_top--;
            }else
            {
                if (!object_carrier.objects)
                    object_carrier.objects = carrier.objects + i;

                object_carrier.objects_length ++;
            }

            if (carrier.objects[i].token == TOKEN_SYMBOL_CLOSING_PARENTHESIS)
                preprocess_depth--;
        }else
        {
            if (!object_carrier.objects)
                object_carrier.objects = carrier.objects + i;

            object_carrier.objects_length ++;
        }
    }

    if (object_carrier.objects_length && object_carrier.objects)
        push(object_carrier,&object_carrier_container);

    object_carrier = (object_carrier_t){0};

    int const information_stack_length = (int)(information_stack - information_stack_top + 31);
    information_stack_top = information_stack + 31;

    for (unsigned int i = 0;i < object_carrier_container.object_carriers_length; i++)
        {
            const object_carrier_t ThisCarrier = object_carrier_container.object_carriers[i];

            if (information_stack_length)
            {
                information_stack_top->after_count--;
            }


            instruct_carrier_t res = {0};

            if (ThisCarrier.objects_length == 1)
            {
                switch (ThisCarrier.objects[0].object_type)
                {
                case OBJECT_VALUE:
                    {
                        pushInstructIntoCarrier(&result,(instruct_t){
                            .instruct = INSTRUCT_LOAD_VALUE,
                            .object = (unsigned long long)memcpy(malloc(sizeof(object_t)),ThisCarrier.objects, sizeof(object_t))
                        });
                        break;
                    }
                case OBJECT_VARIABLE:
                    {
                        pushInstructIntoCarrier(&result,(instruct_t){
                            .instruct = INSTRUCT_LOAD_VARIABLE,
                            .object = (unsigned long long)memcpy(malloc(sizeof(object_t)),ThisCarrier.objects, sizeof(object_t))
                        });

                        pushInstructIntoCarrier(&result,(instruct_t){
                            .instruct = INSTRUCT_TO_VALUE,
                            .object = 0
                        });
                        break;
                    }
                default: exit(15);
                }
            }else
            {
                res = cal_ast(ThisCarrier);
                pushInstructsIntoCarrier(&result, res);
            }


            if (information_stack_length)
            {
                if (!information_stack_top->after_count)
                {
                    if (information_stack_top->instruct != INSTRUCT_NONE)
                        pushInstructIntoCarrier(&result,(instruct_t){
                            .instruct = information_stack_top->instruct,
                            .object = res.instructs_length //如果不為0 表示至少前n項屬於該指令的控制範圍 僅有少部分instruct會採信 如 CALL
                        });


                    information_stack_top--;
                    information_stack_top->after_count--;
                }
            }

        }

    if (object_carrier_container.object_carriers && object_carrier_container.object_carriers_length)
        free(object_carrier_container.object_carriers);

    return result;
}
instruct_carrier_t calculate(object_carrier_t const object_carrier)
{
    instruct_carrier_t const result = cal_ast(object_carrier);

    for (int i = 0;i < result.instructs_length; i++)
    {
        printf("%d %llu\n",result.instructs[i].instruct,result.instructs[i].object);
    }
    return result;
}
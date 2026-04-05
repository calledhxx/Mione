#include <stdlib.h>
#include <limits.h>

#include "main.h"

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
    char * stack_top = preprocess_stack + 31;

    if (
        carrier.objects[0].token == TOKEN_SYMBOL_OPENING_PARENTHESIS
        &&
        carrier.objects[carrier.objects_length - 1].token == TOKEN_SYMBOL_CLOSING_PARENTHESIS
        )
    {
        carrier.objects++;
        carrier.objects_length -= 2;
    }

    for (unsigned i = 0; i < carrier.objects_length; i++)
        if (carrier.objects[i].object_type == OBJECT_SYMBOL)
        {
            symbol_t const ThisSymbol = token_to_symbol(carrier.objects[i].token);

            if (carrier.objects[i].token == TOKEN_SYMBOL_OPENING_PARENTHESIS)
            {
                *stack_top = TOKEN_SYMBOL_OPENING_PARENTHESIS;
                printf("in\n");
                stack_top--;
            }

            highest_order = _max((preprocess_stack - stack_top + 31) ? 0 : (int)ThisSymbol.order,highest_order);

            if (carrier.objects[i].token == TOKEN_SYMBOL_CLOSING_PARENTHESIS)
            {
                stack_top++;
                printf("out\n");

                if (*stack_top != TOKEN_SYMBOL_OPENING_PARENTHESIS)
                    exit(101);
                *stack_top = 0;
            }

            printf("%d %d\n",(preprocess_stack - stack_top + 31),(preprocess_stack - stack_top + 31) ? 0 : (int)ThisSymbol.order);
        }

    if (preprocess_stack - stack_top + 31)
        exit(100);

    printf("highest: %d\n",highest_order);

    int preprocess_depth = 0;
    
    object_carrier_container_t object_carrier_container = {0};
    object_carrier_t object_carrier = {0};

    instruct_information_carrier_t instruct_information_carrier = {0};


    for (int i = 0; i < carrier.objects_length; i++)
    {
        printf("%d: %d %d %p\n",i,carrier.objects[i].object_type,carrier.objects[i].token,&carrier.objects[i].vv);

        const object_t ThisObj = carrier.objects[i];

        if (ThisObj.object_type == OBJECT_SYMBOL)
        {
            symbol_t const ThisSymbol = token_to_symbol(ThisObj.token);

            printf(":       %d\n",(preprocess_depth ? 0 : ThisSymbol.order));

            if (carrier.objects[i].token == TOKEN_SYMBOL_OPENING_PARENTHESIS)
            {
                preprocess_depth++;
            }

            if (carrier.objects[i].token == TOKEN_SYMBOL_CLOSING_PARENTHESIS)
            {
                preprocess_depth--;
            }

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

                instruct_information_carrier.instruct_information_length++;
                instruct_information_carrier.instruct_information = realloc(
                instruct_information_carrier.instruct_information,
                sizeof (instruct_information_t) *  instruct_information_carrier.instruct_information_length
                    );

                printf("split\n");

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

                        break;
                    }
                default: exit(11);
                }

                instruct_information_carrier.instruct_information[instruct_information_carrier.instruct_information_length - 1]=
                    information;

                continue;
            }
        }

        if (!object_carrier.objects)
            object_carrier.objects = carrier.objects + i;

        object_carrier.objects_length ++;
        printf("add len %d\n",object_carrier.objects_length);
    }



    instruct_information_t * information_ptr = instruct_information_carrier.instruct_information;

    if (object_carrier.objects_length && object_carrier.objects)
    {
        printf("split\n");
        push(object_carrier,&object_carrier_container);
    }

    for (int i =0;i<object_carrier_container.object_carriers_length;i++)
        print_object_carrier(object_carrier_container.object_carriers[i]);


    object_carrier = (object_carrier_t){0};

    for (unsigned int i = 0;i < object_carrier_container.object_carriers_length; i++)
    {
        const object_carrier_t ThisCarrier = object_carrier_container.object_carriers[i];

        information_ptr->after_count--;

        if (ThisCarrier.objects_length == 1)
        {
            switch (ThisCarrier.objects[0].object_type)
            {
            case OBJECT_VALUE:
                {
                    pushInstructIntoCarrier(&result,(instruct_t){
                        .instruct = INSTRUCT_LOAD_VALUE,
                        .object = (long long unsigned)ThisCarrier.objects
                    });
                    break;
                }
            case OBJECT_VARIABLE:
                {
                    pushInstructIntoCarrier(&result,(instruct_t){
                        .instruct = INSTRUCT_LOAD_VARIABLE,
                        .object = (long long unsigned)ThisCarrier.objects
                    });

                    pushInstructIntoCarrier(&result,(instruct_t){
                        .instruct = INSTRUCT_TO_VALUE,
                        .object = 0
                    });
                    break;
                }
            default: exit(11);
            }
        }else
            pushInstructsIntoCarrier(&result,cal_ast(ThisCarrier));

        if (!information_ptr->after_count)
        {
            if (information_ptr->instruct != INSTRUCT_NONE)
                pushInstructIntoCarrier(&result,(instruct_t){
                    .instruct = information_ptr->instruct,
                    .object = 0
                });

            information_ptr++;

            information_ptr->after_count--;
        }
    }

    return result;
}
instruct_carrier_t calculate(object_carrier_t object_carrier)
{
    printf("hello?\n");
    print_object_carrier(object_carrier);

    instruct_carrier_t const result = cal_ast(object_carrier);

    for (int i = 0;i < result.instructs_length; i++)
    {
        printf("%d %llu\n",result.instructs[i].instruct,result.instructs[i].object);
    }
    return result;
}
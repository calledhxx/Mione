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

instruct_carrier_t cal_ast(
    object_carrier_t carrier,
    enum calculate_option_flag_e calculate_option_flag_for_once,
    enum calculate_option_flag_e calculate_option_flag_for_all
    )
{
    instruct_carrier_t result = {0};

    int highest_order = -INT_MAX;

    char preprocess_stack[32] = {0};
    char * preprocess_stack_top = preprocess_stack + 31;

    while (
            carrier.objects[0].token == TOKEN_SYMBOL_OPENING_BRACKET
            &&
            carrier.objects[carrier.objects_length - 1].token == TOKEN_SYMBOL_CLOSING_BRACKET
            )
    {
        if (!(calculate_option_flag_for_once & CALCULATE_OPTION_FLAG_FREE_BRACKET_ALLOWED))
            exit(102);

        for (int i = 1;i<carrier.objects_length - 1; i++)
            if (carrier.objects[i].token == TOKEN_SYMBOL_OPENING_BRACKET)
                goto simple_bracket;
            else if(carrier.objects[i].token == TOKEN_SYMBOL_CLOSING_BRACKET)
                goto keep;

        simple_bracket:;
        carrier.objects++;
        carrier.objects_length -= 2;
    }


    while (
        carrier.objects[0].token == TOKEN_SYMBOL_OPENING_PARENTHESIS
        &&
        carrier.objects[carrier.objects_length - 1].token == TOKEN_SYMBOL_CLOSING_PARENTHESIS
        )
    {

        for (int i = 1;i<carrier.objects_length - 1; i++)
            if (carrier.objects[i].token == TOKEN_SYMBOL_OPENING_PARENTHESIS)
                goto simple_parenthesis;
            else if(carrier.objects[i].token == TOKEN_SYMBOL_CLOSING_PARENTHESIS)
                goto keep;


        simple_parenthesis:;
        carrier.objects++;
        carrier.objects_length -= 2;
    }
    keep:;

    symbol_t symbol_stack[32] = {0};
    symbol_t * symbol_stack_top = symbol_stack + 31;

    for (int i = 0; i < carrier.objects_length; i++)
        if (carrier.objects[i].object_type == OBJECT_SYMBOL)
        {
            symbol_t ThisSymbol = token_to_symbol(carrier.objects[i].token);

            if (
                carrier.objects[i].token == TOKEN_SYMBOL_OPENING_PARENTHESIS
                ||
                carrier.objects[i].token == TOKEN_SYMBOL_OPENING_BRACKET
                )
            {
                *preprocess_stack_top = carrier.objects[i].token;
                preprocess_stack_top--;
            }

            information_t information = (information_t){0};

            {
                if (ThisSymbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_SPECIAL)
                    goto sccapf_end;

                if (ThisSymbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_BEFORE)
                    if (i + 1 < carrier.objects_length)
                    {
                        information = ThisSymbol.instruct_information[1];
                        goto sccapf_end;
                    }

                if (ThisSymbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_AFTER)
                    if (i - 1 >= 0)
                    {
                        information = ThisSymbol.instruct_information[2];
                        goto sccapf_end;
                    }

                if (ThisSymbol.calculate_allow_position_flag & SYMBOL_CALCULATE_ALLOW_POSITION_FLAG_MIDDLE)
                    if (i - 1 >= 0 && i + 1 < carrier.objects_length)
                    {
                        information = ThisSymbol.instruct_information[0];
                        goto sccapf_end;
                    }

                exit(132);
            }
            sccapf_end:

            int order = ThisSymbol.order = preprocess_stack - preprocess_stack_top + 31 ? 0 : information.order;
            ThisSymbol.instruct_information[0] = information;

            highest_order = _max(order,highest_order);

            if (carrier.objects[i].token == TOKEN_SYMBOL_CLOSING_PARENTHESIS)
            {
                preprocess_stack_top++;

                if (*preprocess_stack_top != TOKEN_SYMBOL_OPENING_PARENTHESIS)
                    exit(101);
                *preprocess_stack_top = 0;
            }else if (carrier.objects[i].token == TOKEN_SYMBOL_CLOSING_BRACKET)
            {
                preprocess_stack_top++;

                if (*preprocess_stack_top != TOKEN_SYMBOL_OPENING_BRACKET)
                    exit(102);
                *preprocess_stack_top = 0;
            }

            *symbol_stack_top-- = ThisSymbol;
        }

    symbol_stack_top = symbol_stack + 31;

    if (preprocess_stack - preprocess_stack_top + 31)
        exit(100);

    int preprocess_depth = 0;

    object_carrier_container_t object_carrier_container = {0};
    object_carrier_t object_carrier = {0};

    information_t information_stack[32] = {0};
    information_t * information_stack_top = information_stack + 31;

    for (int i = 0; i < carrier.objects_length; i++)
    {
        const object_t ThisObj = carrier.objects[i];

        if (ThisObj.object_type == OBJECT_SYMBOL)
        {
            symbol_t const ThisSymbol = *symbol_stack_top--;

            if (
                carrier.objects[i].token == TOKEN_SYMBOL_OPENING_PARENTHESIS
                ||
                carrier.objects[i].token == TOKEN_SYMBOL_OPENING_BRACKET
                )
                preprocess_depth++;

            if (highest_order == (preprocess_depth ? 0 : ThisSymbol.order))
            {
                if (object_carrier.objects_length && object_carrier.objects)
                    push(object_carrier,&object_carrier_container);

                object_carrier = (object_carrier_t){0};

                information_t information = ThisSymbol.instruct_information[0];

                if (ThisObj.token == TOKEN_SYMBOL_OPENING_PARENTHESIS)
                {
                    highest_order = 1;
                    information = (information_t){
                        .instruct = INSTRUCT_CALL,
                        .after_count = 2,
                        .option = INFORMATION_OPTION_FLAG_PREPOSITION | INFORMATION_OPTION_FLAG_REQUIRED_LENGTH
                    };

                    i--;
                }else if (ThisObj.token == TOKEN_SYMBOL_OPENING_BRACKET)
                {
                    highest_order = 1;
                    information = (information_t){
                        .instruct = INSTRUCT_LOCATE,
                        .after_count = 2,
                        .option = INFORMATION_OPTION_FLAG_PREPOSITION | INFORMATION_OPTION_FLAG_REQUIRED_LENGTH,
                        .calculate_option_flag = CALCULATE_OPTION_FLAG_FREE_BRACKET_ALLOWED
                    };

                    i--;
                }

                *information_stack_top-- = information;
            }else
            {
                if (!object_carrier.objects)
                    object_carrier.objects = carrier.objects + i;

                object_carrier.objects_length ++;
            }

            if (
                carrier.objects[i].token == TOKEN_SYMBOL_CLOSING_PARENTHESIS
                ||
                carrier.objects[i].token == TOKEN_SYMBOL_CLOSING_BRACKET
                )
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

    for (unsigned int i = 0;i < object_carrier_container.object_carriers_length; i++){

        const object_carrier_t ThisCarrier = object_carrier_container.object_carriers[i];

        if (information_stack_length)
            information_stack_top->after_count--;

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

                    if (!(calculate_option_flag_for_all & CALCULATE_OPTION_FLAG_PROTECT_VARIABLE))
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
            res = cal_ast(
                ThisCarrier,
                information_stack_top->calculate_option_flag,
                calculate_option_flag_for_all
                );

            if (information_stack_length)
                if (!information_stack_top->after_count)
                    if (information_stack_top->instruct != INSTRUCT_NONE)
                        if (information_stack_top->option & INFORMATION_OPTION_FLAG_PREPOSITION)
                        {

                            pushInstructIntoCarrier(&result,(instruct_t){
                                .instruct = information_stack_top->instruct,
                                .object =
                                    (information_stack_top->option & INFORMATION_OPTION_FLAG_REQUIRED_LENGTH) ?
                                    res.instructs_length : 0
                            });
                            pushInstructsIntoCarrier(&result, res);

                            goto cl_end;
                        }

            pushInstructsIntoCarrier(&result, res);
        }

        if (information_stack_length)
        {

            if (!information_stack_top->after_count)
            {
                if (information_stack_top->instruct != INSTRUCT_NONE)
                    pushInstructIntoCarrier(&result,(instruct_t){
                               .instruct = information_stack_top->instruct,
                               .object =
                                   (information_stack_top->option & INFORMATION_OPTION_FLAG_REQUIRED_LENGTH) ?
                                   res.instructs_length : 0
                           });


                (--information_stack_top)->after_count--;
            }
        }

        cl_end:;
    }

    if (object_carrier_container.object_carriers && object_carrier_container.object_carriers_length)
        free(object_carrier_container.object_carriers);

    return result;
}
instruct_carrier_t calculate(object_carrier_t const object_carrier,char const calculate_as_container)
{
    instruct_carrier_t const result = cal_ast(
        object_carrier,
        CALCULATE_OPTION_FLAG_NONE,
        calculate_as_container ? CALCULATE_OPTION_FLAG_PROTECT_VARIABLE : CALCULATE_OPTION_FLAG_NONE
        );

    print_instruct_carrier(result);
    return result;
}
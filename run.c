#include <stdlib.h>

#include "main.h"

#define STACK_SIZE 256

void run(instruct_carrier_t const instruct_carrier)
{
    unsigned long long  stack[STACK_SIZE] = {0};
    unsigned long long  * stack_top = stack + STACK_SIZE - 1;

    for (unsigned i = 0; i < instruct_carrier.instructs_length;i++)
    {
        unsigned long long const object = instruct_carrier.instructs[i].object;

        switch (instruct_carrier.instructs[i].instruct)
        {
        case INSTRUCT_LOAD_VARIABLE:
            {
                *stack_top-- = object;

                break;
            }
        case INSTRUCT_LOAD_VALUE:
            {
                *stack_top-- = object;
                break;
            }
        case INSTRUCT_TO_VALUE:
            {
                long long unsigned const a = *stack_top++;

                break;
            }
        default:
            exit(24);
        }
    }
}
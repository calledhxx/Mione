#include <stdlib.h>

#include "main.h"

#define STACK_SIZE 256

void run(instruct_carrier_t const instruct_carrier)
{
    long long unsigned stack[STACK_SIZE] = {0};
    long long unsigned * stack_top = stack + STACK_SIZE - 1;

    for (unsigned i = 0; i < instruct_carrier.instructs_length;i++)
    {
        switch (instruct_carrier.instructs[i].instruct)
        {
        case INSTRUCT_LOAD_VARIABLE:
            {
                *stack_top-- = instruct_carrier.instructs[i].object;
                break;
            }
        case INSTRUCT_LOAD_VALUE:
            {
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
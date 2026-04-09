#include <stdlib.h>

#include "main.h"

#define STACK_SIZE 256

void run(instruct_carrier_t const instruct_carrier)
{
    long long unsigned stack[STACK_SIZE] = {0};
    long long unsigned * stack_top = stack + STACK_SIZE - 1;

    long long unsigned
        ea = 0,
        eb = 0,
        ec = 0,
        ed = 0;

    for (unsigned i = 0; i < instruct_carrier.instructs_length;i++)
    {
        unsigned long long const object = instruct_carrier.instructs[i].object;

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
        case INSTRUCT_RECORD_STACK_TOP:
            {
                switch (object)
                {
                case 1:
                    {
                        ea = (intptr_t)stack_top;
                        break;
                    }
                case 2:
                    {
                        eb = (intptr_t)stack_top;
                        break;
                    }
                case 3:
                    {
                        ec = (intptr_t)stack_top;
                        break;
                    }
                case 4:
                    {
                        ed = (intptr_t)stack_top;
                        break;
                    }
                default:exit(25);
                }
                break;
            }
        case INSTRUCT_WRITE_INTO_VARIABLE:
            {
                if (!ea)
                    exit(25);

                unsigned const len = (unsigned long long *)ea - stack_top + 1;

                if (len & 1)
                    exit(137);

                for (int j = 0;j<len/2;j++)
                {
                    unsigned long long const * container = (unsigned long long *)(ea + j);
                    unsigned long long const * resource = (unsigned long long *)(ea + len/2 + j);


                }
                break;
            }
        default:
            exit(24);
        }
    }
}
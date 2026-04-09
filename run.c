#include <stdlib.h>

#include "main.h"

#define STACK_SIZE 256

void run(instruct_carrier_t const instruct_carrier)
{
    unsigned long long  stack[STACK_SIZE] = {0};
    unsigned long long  * stack_top = stack + STACK_SIZE - 1;

    printf("bs%llu st%llu\n",stack,stack_top);

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
                printf("%d push variable %llu as %llu\n",i,object,stack_top);


                *stack_top-- = object;

                printf("verify %llu\n",((object_t*)(*(stack_top+1)))->object_type);

                break;
            }
        case INSTRUCT_LOAD_VALUE:
            {
                printf("%d push value %llu as %llu\n",i,object,stack_top);

                *stack_top-- = object;
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

                unsigned const len = (unsigned long long *)ea - stack_top;

                if (len & 1)
                    exit(1);

                for (int j = 0;j<len>>1;j++)
                {
                    unsigned long long const * container_ptr = (unsigned long long *)ea - j;
                    unsigned long long const * resource_ptr = (unsigned long long *)ea - len/2 - j;

                    printf("con %llu\n",container_ptr);
                    printf("res %llu\n",resource_ptr);

                    printf("objtype %d",((object_t*)(*container_ptr))->object_type);

                    variable_t * const ge_var_ptr = variable_chaser(*((object_t*)(*container_ptr))->vv.variable_link_ptr);

                    printf("pos %p\n",ge_var_ptr);

                    ge_var_ptr->variable.genuine_variable.value = ((object_t*)(*resource_ptr))->vv.value;
                }
                break;
            }
        default:
            exit(24);
        }
    }
}
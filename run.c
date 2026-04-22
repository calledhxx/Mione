    #include <stdlib.h>
    #include <string.h>

    #include "main.h"

    #define STACK_SIZE 256

    void run(instruct_carrier_t const instruct_carrier)
    {
        unsigned long long stack[STACK_SIZE] = {0};
        unsigned long long * stack_top = stack + STACK_SIZE - 1;

        unsigned long long registers[4] = {0};

        for (unsigned i = 0; i < instruct_carrier.instructs_length;i++)
        {
            unsigned long long const object = instruct_carrier.instructs[i].object;

            switch (instruct_carrier.instructs[i].instruct)
            {
            case INSTRUCT_LOAD_VARIABLE:
            case INSTRUCT_LOAD_VALUE:
                {
                    *stack_top-- = object;

                    break;
                }
            case INSTRUCT_TO_VALUE:
                {
                    unsigned long long a = *++stack_top;
                    variable_t* variable_ptr = variable_chaser(*((object_t*)a)->vv.variable_link_ptr);

                    if (!variable_ptr)
                        exit(123);

                    *stack_top-- = (long long unsigned)memcpy(alc(0,sizeof(object_t)),&(object_t){
                        .object_type = OBJECT_VALUE,
                        .vv.value = variable_ptr->variable.genuine_variable.value
                    },sizeof(object_t));

                    break;
                }
            case INSTRUCT_RECORD_STACK_TOP:
                {
                    registers[object] = (long long unsigned)stack_top;

                    break;
                }
            case INSTRUCT_WRITE_VARIABLE:
                {
                    if (registers[1] - (long long unsigned)stack_top != registers[0] - registers[1])
                        exit(25);

                    for (int index = 0; index < (long long unsigned*)registers[0] - (long long unsigned*)registers[1]; index++)
                    {
                        variable_t* variable_ptr = variable_chaser(*((object_t*)*((long long unsigned*)registers[0] + index))->vv.variable_link_ptr);
                        if (!variable_ptr)
                            exit(26);
                        variable_ptr->variable.genuine_variable.value = ((object_t*)*((long long unsigned*)registers[1] + index))->vv.value;
                    }

                    break;
                }
            case INSTRUCT_ADD:
                {
                    unsigned long long b = *++stack_top;
                    unsigned long long a = *++stack_top;

                    object_t const obj_b = *((object_t*)b);
                    object_t const obj_a = *((object_t*)a);

                    if (obj_b.vv.value.value_type != VALUE_NUMBER || obj_a.vv.value.value_type != VALUE_NUMBER)
                        exit(221);

                    *stack_top-- = (long long unsigned)memcpy(alc(0,sizeof(object_t)),&(object_t){
                        .object_type = OBJECT_VALUE,
                        .vv.value = (value_t){
                            .value_type = VALUE_NUMBER,
                            .value.number = obj_a.vv.value.value.number + obj_b.vv.value.value.number
                        }
                    },sizeof(object_t));

                    printf("a %f + b %f\n",obj_a.vv.value.value.number,obj_b.vv.value.value.number);

                    break;
                }
            case INSTRUCT_MUL:
                {
                    unsigned long long b = *++stack_top;
                    unsigned long long a = *++stack_top;

                    object_t const obj_b = *((object_t*)b);
                    object_t const obj_a = *((object_t*)a);

                    if (obj_b.vv.value.value_type != VALUE_NUMBER || obj_a.vv.value.value_type != VALUE_NUMBER)
                        exit(221);

                    *stack_top-- = (long long unsigned)memcpy(alc(0,sizeof(object_t)),&(object_t){
                        .object_type = OBJECT_VALUE,
                        .vv.value = (value_t){
                            .value_type = VALUE_NUMBER,
                            .value.number = obj_a.vv.value.value.number * obj_b.vv.value.value.number
                        }
                    },sizeof(object_t));

                    printf("a %f * b %f\n",obj_a.vv.value.value.number,obj_b.vv.value.value.number);

                    break;
                }
            case INSTRUCT_PRINT_NUMBER:
                {
                    variable_t* variable_ptr = variable_chaser(*((object_t*)object)->vv.variable_link_ptr);
                    printf("v: %f\n",variable_ptr->variable.genuine_variable.value.value.number);

                    break;
                }
            default:
                printf("%d\n",instruct_carrier.instructs[i].instruct);
                exit(24);
            }
        }
    }
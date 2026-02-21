#include <stdlib.h>

#include "main.h"

static instruct_carrier_t set(train_t const train)
{
    instruct_carrier_t instruct_carrier = {0};

    for (int i = 0; i < train.carriage_carrier.carriages_length; i++)
    {
        carriage_t const ThisCarriage = train.carriage_carrier.carriages[i];

        switch (ThisCarriage.carriage_type)
        {
        case CARRIAGE_HEAD:
            {
                pushInstructIntoCarrier(
                    &instruct_carrier,
                    (instruct_t){
                        .instruct = INSTRUCT_LOAD_VARIABLE,
                        .object = (intptr_t)ThisCarriage.passengers.objects
                    }
                    );
                break;
            }
        case CARRIAGE_BODY:
            {
                switch (ThisCarriage.conductor)
                {
                case TOKEN_PROMPT_EQUAL:
                    {
                        if (ThisCarriage.passengers.objects[0].object_type == OBJECT_VARIABLE)
                        {
                            pushInstructIntoCarrier(
                                    &instruct_carrier,
                                    (instruct_t){
                                        .instruct = INSTRUCT_LOAD_VARIABLE,
                                        .object = (intptr_t)ThisCarriage.passengers.objects
                                    }
                                    );

                            pushInstructIntoCarrier(
                                    &instruct_carrier,
                                    (instruct_t){
                                        .instruct = INSTRUCT_TO_VALUE,
                                        .object = 0
                                    }
                                    );
                        }else if (ThisCarriage.passengers.objects[0].object_type == OBJECT_VALUE){
                            pushInstructIntoCarrier(
                                    &instruct_carrier,
                                    (instruct_t){
                                        .instruct = INSTRUCT_LOAD_VALUE,
                                        .object = (intptr_t)ThisCarriage.passengers.objects
                                    }
                                    );
                        }else exit(1);

                        break;
                    }
                default:
                    exit(4);
                }


                break;
            }
        default:
            exit(3);
        }
    }
}

static keyword_t HeadKeywordArray[] = {
    {
        .name = "set",
        .token = TOKEN_HEAD_SET,
        .instance = &(head_t){
            .function = set
        }
    },
{0}
};

keyword_t * HeadKeyword = HeadKeywordArray;

head_t token_to_head(const int token)
{
    for (unsigned i = 0; i < sizeof(HeadKeywordArray) / sizeof(keyword_t); i++)
        if (HeadKeywordArray[i].token == token)
            return *(head_t*)HeadKeywordArray[i].instance;

    return (head_t){0};
}
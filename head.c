#include <stdlib.h>

#include "main.h"

static instruct_carrier_t * set(train_t const * const trainPtr)
{
    train_t const train = *trainPtr;

    instruct_carrier_t instruct_carrier = {0};

    pushInstructIntoCarrier(
                    &instruct_carrier,
                    (instruct_t){
                        .instruct = INSTRUCT_RECORD_STACK_TOP,
                        .object = 1,
                    }
        );

    for (int i = 0; i < train.carriage_carrier.carriages_length; i++)
    {
        carriage_t const ThisCarriage = train.carriage_carrier.carriages[i];

        switch (ThisCarriage.carriage_type)
        {
        case CARRIAGE_HEAD:
            {
                pushInstructsIntoCarrier(
                    &instruct_carrier,
                    calculate(ThisCarriage.passengers,1)
                    );
                break;
            }
        case CARRIAGE_BODY:
            {
                switch (ThisCarriage.conductor)
                {
                case TOKEN_PROMPT_EQUAL:
                    {
                        pushInstructsIntoCarrier(
                            &instruct_carrier,
                            calculate(ThisCarriage.passengers,0)
                        );

                        break;
                    }
                default:
                    exit(4);
                }


                break;
            }
        default:
            exit(111);
        }
    }

    pushInstructIntoCarrier(
                    &instruct_carrier,
                    (instruct_t){
                        .instruct = INSTRUCT_WRITE_INTO_VARIABLE,
                        .object = 1,
                    }
        );

    instruct_carrier_t * res = alc(0,sizeof(instruct_carrier_t));
    *res = instruct_carrier;
    return res;
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
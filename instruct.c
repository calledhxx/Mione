#include <stdlib.h>
#include <string.h>

#include "main.h"

void pushInstructIntoCarrier(instruct_carrier_t * const CarrierPtr,instruct_t const instruct)
{
    CarrierPtr->instructs_length++;
    CarrierPtr->instructs = realloc(
    CarrierPtr->instructs,
    CarrierPtr->instructs_length * sizeof(instruct_t)
        );
    CarrierPtr->instructs[CarrierPtr->instructs_length - 1] = instruct;
}

instruct_carrier_t train_to_instruct(train_carrier_t const train_carrier)
{
    instruct_carrier_t instruct_carrier = {0};

    for (unsigned i = 0; i < train_carrier.trains_length;i++)
    {
        train_t const ThisTrain = train_carrier.trains[i];

        switch (ThisTrain.train_type)
        {
        case TRAIN_NORMAL:
            {
                head_t const head = token_to_head(ThisTrain.carriage_carrier.carriages[0].conductor);

                instruct_carrier_t const * const head_instruct_carrier =  head.function((void*)&train_carrier);

                instruct_carrier.instructs_length += head_instruct_carrier->instructs_length;
                instruct_carrier.instructs = realloc(
                    instruct_carrier.instructs,
                    sizeof(instruct_t) * instruct_carrier.instructs_length
                    );

                memcpy(
                instruct_carrier.instructs + instruct_carrier.instructs_length - head_instruct_carrier->instructs_length,
                head_instruct_carrier->instructs,
                sizeof(instruct_t) * head_instruct_carrier->instructs_length
                    );

                break;
            }

        default:
            exit(2);
        }
    }
}
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

void pushInstructsIntoCarrier(instruct_carrier_t * const CarrierPtr,instruct_carrier_t const instructs)
{
    CarrierPtr->instructs_length += instructs.instructs_length;
    CarrierPtr->instructs = realloc(
        CarrierPtr->instructs,
        sizeof(instruct_t) * CarrierPtr->instructs_length
        );

    memcpy(
    CarrierPtr->instructs + CarrierPtr->instructs_length - instructs.instructs_length,
    instructs.instructs,
    sizeof(instruct_t) * instructs.instructs_length
        );
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

                instruct_carrier_t const * const head_instruct_carrier = head.function(&ThisTrain);

                pushInstructsIntoCarrier(&instruct_carrier,*head_instruct_carrier);

                break;
            }

        default:
            exit(2);
        }
    }

    return instruct_carrier;

}
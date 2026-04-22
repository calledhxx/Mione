#include <stdlib.h>
#include <string.h>

#include "main.h"

void pushInstructIntoCarrier(instruct_carrier_t * const CarrierPtr,instruct_t const instruct)
{
    CarrierPtr->instructs_length++;
    CarrierPtr->instructs = alc(
    CarrierPtr->instructs,
    CarrierPtr->instructs_length * sizeof(instruct_t)
        );
    CarrierPtr->instructs[CarrierPtr->instructs_length - 1] = instruct;
}

void pushInstructsIntoCarrier(instruct_carrier_t * const CarrierPtr,instruct_carrier_t const instructs)
{
    CarrierPtr->instructs_length += instructs.instructs_length;
    CarrierPtr->instructs = alc(
        CarrierPtr->instructs,
        sizeof(instruct_t) * CarrierPtr->instructs_length
        );

    memcpy(
    CarrierPtr->instructs + (CarrierPtr->instructs_length - instructs.instructs_length),
    instructs.instructs,
    sizeof(instruct_t) * instructs.instructs_length
        );
}

void print_instruct_carrier(instruct_carrier_t const instruct_carrier)
{
    for (int i = 0;i < instruct_carrier.instructs_length; i++)
    {
        printf("%d ins: %d ",i,instruct_carrier.instructs[i].instruct);

        if (instruct_carrier.instructs[i].instruct == INSTRUCT_LOAD_VALUE)
        {
            printf("number: %f pos %llu\n",((object_t*)instruct_carrier.instructs[i].object)->vv.value.value.number,instruct_carrier.instructs[i].object);
        }
        else
            printf("object: %llu\n",instruct_carrier.instructs[i].object);
    }
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

                instruct_carrier_t const * const head_instruct_carrier_ptr = head.function((void*)&ThisTrain);

                pushInstructsIntoCarrier(&instruct_carrier,*head_instruct_carrier_ptr);

                print_instruct_carrier(*head_instruct_carrier_ptr);

                fre((void*)head_instruct_carrier_ptr->instructs);
                fre((void*)head_instruct_carrier_ptr);

                break;
            }
        case TRAIN_SIMPLE:
            {

                pushInstructIntoCarrier(&instruct_carrier,(instruct_t){
                    .instruct = INSTRUCT_PRINT_NUMBER,
                    .object = (long long unsigned)memcpy(alc(0,sizeof(object_t)),ThisTrain.carriage_carrier.carriages[0].passengers.objects,sizeof(object_t))
                });


                break;
            }

        default:
            exit(2);
        }
    }

    return instruct_carrier;
}
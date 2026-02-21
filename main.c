#include <stdio.h>

#include "main.h"

#include <stdlib.h>

int main(void)
{
    FILE * f = fopen("./index.mio","r");

    word_carrier_t const WordCarrier = stream_to_word(f,EOF);

    fclose(f);

    object_carrier_t const ObjectCarrier = word_to_object(WordCarrier);

    for (unsigned i = 0; i < WordCarrier.words_length; i++)
        free(WordCarrier.words[i].word);
    free(WordCarrier.words);

    train_carrier_t const TrainCarrier = object_to_train(ObjectCarrier);

    free(ObjectCarrier.objects);

    instruct_carrier_t const InstructCarrier = train_to_instruct(TrainCarrier);

    for (unsigned i = 0; i < TrainCarrier.trains_length; i++)
    {
        for (unsigned j = 0; j < TrainCarrier.trains[i].carriage_carrier.carriages_length; j++)
            free(TrainCarrier.trains[i].carriage_carrier.carriages[j].passengers.objects);
        free(TrainCarrier.trains[i].carriage_carrier.carriages);
    }
    free(TrainCarrier.trains);

    CreateLine(); //start using Allocate



    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main(void)
{
    FILE * f = fopen("./index.mio","r");

    CreateLine();

    word_carrier_t const WordCarrier = stream_to_word(f,EOF);

    fclose(f);

    scope_t current_scope = {0};
    object_carrier_t const ObjectCarrier = word_to_object(WordCarrier,&current_scope);
    print_scope(current_scope,0);

    for (unsigned i = 0; i < WordCarrier.words_length; i++)
        fre(WordCarrier.words[i].word);
    fre((unit_t*)WordCarrier.words);

    train_carrier_t const TrainCarrier = object_to_train(ObjectCarrier);

    fre((unit_t*)ObjectCarrier.objects);

    instruct_carrier_t const InstructCarrier = train_to_instruct(TrainCarrier);

    for (unsigned i = 0; i < TrainCarrier.trains_length; i++)
    {
        for (unsigned j = 0; j < TrainCarrier.trains[i].carriage_carrier.carriages_length; j++)
            fre((unit_t*)TrainCarrier.trains[i].carriage_carrier.carriages[j].passengers.objects);
        fre((unit_t*)TrainCarrier.trains[i].carriage_carrier.carriages);
    }
    fre((unit_t*)TrainCarrier.trains);

    run(InstructCarrier);

    return 0;
}

#include <stdio.h>

#include "main.h"

int main(void)
{
    FILE * f = fopen("./index.mio","r");

    word_carrier_t const WordCarrier = stream_to_word(f,EOF);

    object_carrier_t const ObjectCarrier = word_to_object(WordCarrier);

    train_carrier_t const TrainCarrier = object_to_train(ObjectCarrier);

    CreateLine(); //start using Allocate

    return 0;
}
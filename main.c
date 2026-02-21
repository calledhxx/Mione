#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "word.h"
#include "object.h"

int main(void)
{
    FILE * f = fopen("./index.mio","r");

    word_carrier_t const WordCarrier = stream_to_word(f,EOF);

    for (unsigned i = 0;i < WordCarrier.words_length;i++)
    {
        printf("word %u : %s (type %u)\n",i,WordCarrier.words[i].word,WordCarrier.words[i].word_type);
    }

    object_carrier_t ObjectCarrier;

    CreateLine(); //start using Allocate


    return 0;
}
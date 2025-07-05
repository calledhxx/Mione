//
// Created by calle on 2025/6/12.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <tgmath.h>
#include <wchar.h>

#include "OBJECTS.h"

extern inline uint32_t CharNumberToInteger(const wchar_t * input,const unsigned int inputSize)
{
    uint32_t res = 0;

    for (int i = 0;i < inputSize ;i++)
        res += (input[inputSize - i - 1]-0x30) * (uint32_t)pow(10,inputSize - i - 1);

    return res;
}

NumberObj wcharToNumber(const wchar_t* input)
{
    NumberObj Number;

    IntegerObj Integer;

    Integer.Carrier = malloc(0);
    Integer.CarrierLength = 0;

    wchar_t * loader = malloc(0);
    unsigned int loaderSize = 0;

    for (int charIndex = 0; charIndex < wcslen(input);charIndex++)
    {
        loaderSize++;
        loader = realloc(loader, loaderSize * sizeof(wchar_t));
        loader[loaderSize-1] = input[wcslen(input)-charIndex-1];

        if ((charIndex % 9 == 0 && charIndex) || charIndex == wcslen(input)-1)
        {
            const uint32_t a = CharNumberToInteger(loader,loaderSize);

            free(loader);
            loader = malloc(0);
            loaderSize = 0;

            Integer.CarrierLength++;
            Integer.Carrier = realloc(Integer.Carrier, Integer.CarrierLength * sizeof(uint32_t));
            Integer.Carrier[Integer.CarrierLength-1] = a;
        }
    }



    Number.Integer = Integer;

    return Number;
}

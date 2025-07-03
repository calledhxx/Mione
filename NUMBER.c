//
// Created by calle on 2025/6/12.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <tgmath.h>
#include <wchar.h>

#include "OBJECTS.h"

IntegerObj INTEGERAdd(const IntegerObj a,const IntegerObj b)
{
    IntegerObj result;
    result.Carrier = malloc(0);
    result.CarrierLength = 0;

    unsigned int maxCarrierLen = a.CarrierLength > b.CarrierLength ? a.CarrierLength : b.CarrierLength;

    int toAdd = 0;

    if (a.Carrier[0] & b.Carrier[0] & 0b10000000)
    {
        //負數
        for (int carrierIndex = 0; carrierIndex < maxCarrierLen; carrierIndex++)
        {

        }
    }else
    {
        //正數
        for (int carrierIndex = 0; carrierIndex < maxCarrierLen; carrierIndex++)
        {

        }
    }



    return result;
}

extern inline uint8_t CharNumberToUint8(const wchar_t * input,const unsigned int inputSize)
{
    uint8_t res = 0;

    for (int i = 0;i < inputSize ;i++)
        res += (input[inputSize - i - 1]-0x30) * pow(10,inputSize - i - 1);

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


        if (charIndex & 1 || charIndex == wcslen(input)-1)
        {
            const uint8_t a = CharNumberToUint8(loader,loaderSize);

            free(loader);
            loader = malloc(0);
            loaderSize = 0;

            Integer.CarrierLength++;
            Integer.Carrier = realloc(Integer.Carrier, Integer.CarrierLength * sizeof(uint8_t));
            Integer.Carrier[Integer.CarrierLength-1] = a;
        }
    }

    Integer.Carrier[Integer.CarrierLength-1] += 0b10000000;


    Number.Integer = Integer;

    return Number;
}

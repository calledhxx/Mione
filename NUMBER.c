//
// Created by calle on 2025/6/12.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <tgmath.h>
#include <wchar.h>

#include "OBJECTS.h"

IntegerObj IntegerSub(const IntegerObj A,const IntegerObj B)
{
    IntegerObj result;
    result.Digits = 0;
    result.Units = NULL;
    result.UnitsLen = 0;

    const unsigned int maxLen = A.UnitsLen > B.UnitsLen ? A.UnitsLen : B.UnitsLen;
    const unsigned int maxDigit = A.Digits > B.Digits ? A.Digits : B.Digits;

    for (unsigned int i = 0;; i++)
    {

    }

    return result;
}

IntegerObj IntegerAdd(const IntegerObj A,const IntegerObj B)
{
    IntegerObj result;
    result.Digits = 0;
    result.Units = NULL;
    result.UnitsLen = 0;

    const unsigned int maxLen = A.UnitsLen > B.UnitsLen ? A.UnitsLen : B.UnitsLen;
    const unsigned int maxDigit = A.Digits > B.Digits ? A.Digits : B.Digits;

    uint8_t NextCycleAdd = 0;

    for (unsigned int i = 0;; i++)
    {
        const uint8_t ThisCycleAdd = NextCycleAdd ? 1 : 0;
        NextCycleAdd = 0;

        if (i >= maxLen)
        {
            if (ThisCycleAdd) result.Digits++;
            break;
        }

        const uint32_t a  = A.UnitsLen > i ? A.Units[i] : 0;
        const uint32_t b  = B.UnitsLen > i ? B.Units[i] : 0;

        const uint32_t c = a + b + ThisCycleAdd;

        if (c < a && c < b)
            NextCycleAdd = 1;

        result.UnitsLen++;
        result.Units = realloc(result.Units, result.UnitsLen * sizeof(uint32_t));
        result.Units[result.UnitsLen - 1] = c;
    }

    result.Digits = A.Digits == B.Digits ? result.Digits + A.Digits : maxDigit;

    return result;
}

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
    Number.Decimal = (IntegerObj){0};
    Number.Integer = (IntegerObj){0};

    IntegerObj Integer;

    Integer.Digits = 0;
    Integer.Units = NULL;
    Integer.UnitsLen = 0;


    wchar_t * loader = NULL;
    unsigned int loaderSize = 0;

    for (int charIndex = 0; charIndex < wcslen(input);charIndex++)
    {
        loaderSize++;
        loader = realloc(loader, loaderSize * sizeof(wchar_t));
        loader[loaderSize-1] = input[wcslen(input)-charIndex-1];

        Integer.Digits++;

        if ((charIndex % 9 == 0 && charIndex) || charIndex == wcslen(input)-1)
        {
            const uint32_t a = CharNumberToInteger(loader,loaderSize);

            free(loader);
            loader = malloc(0);
            loaderSize = 0;

            Integer.UnitsLen++;
            Integer.Units = realloc(Integer.Units, Integer.UnitsLen * sizeof(uint32_t));
            Integer.Units[Integer.UnitsLen-1] = a;
        }
    }

    Number.Integer = Integer;

    return Number;
}

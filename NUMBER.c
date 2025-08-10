//
// Created by calle on 2025/6/12.
//

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <tgmath.h>

#include "STDMIO.h"

void printIntegerObj(const IntegerObj Obj)
{
    printf("Digits: %d\n",Obj.Digits);

    printf("REMEMBER:HIGH TO LOW POSITION:\n",Obj.Digits);
    for (unsigned int i = 0;i < Obj.UnitsLen; i++)
        printf("%u ",Obj.Units[i]);

    printf("\nLen: %d\n\n",Obj.UnitsLen);
}

IntegerObj IntegerSub(const IntegerObj A,const IntegerObj B)
{
    IntegerObj result;
    result.Digits = 0;
    result.Units = NULL;
    result.UnitsLen = 0;

    const unsigned int maxLen = A.UnitsLen > B.UnitsLen ? A.UnitsLen : B.UnitsLen;
    const unsigned int maxDigit = A.Digits > B.Digits ? A.Digits : B.Digits;

    uint8_t NextCycleSub = 0;

    result.UnitsLen = maxLen;
    result.Units = realloc(result.Units, result.UnitsLen * sizeof(uint32_t));

    for (unsigned int i = 0;; i++)
    {
        const uint8_t ThisCycleSub = NextCycleSub;
        NextCycleSub = 0;

        const uint32_t a = A.UnitsLen > i ? A.Units[i] : 0;
        const uint32_t b  = B.UnitsLen > i ? B.Units[i] : 0;

        const uint32_t c = a - b - ThisCycleSub;

        if (i+1 == maxLen)
        {
            if (!c)
            {
                result.UnitsLen--;
                result.Units = realloc(result.Units, result.UnitsLen * sizeof(uint32_t));
            }
            break;
        }

        if (c > a && c > b)
            NextCycleSub=1;

        result.Units[i] = c;
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

    result.Digits += maxDigit;

    uint8_t NextCycleAdd = 0;

    result.UnitsLen = maxLen;
    result.Units = realloc(result.Units, result.UnitsLen * sizeof(uint32_t));

    for (unsigned int i = 0;; i++)
    {
        const uint8_t ThisCycleAdd = NextCycleAdd;
        NextCycleAdd = 0;

        if (i >= maxLen)
            if (ThisCycleAdd)
            {
                result.Digits++;

                result.UnitsLen++;
                result.Units = realloc(result.Units, result.UnitsLen * sizeof(uint32_t));
            }
            else break;

        const uint32_t a  = A.UnitsLen > i ? A.Units[i] : 0;
        const uint32_t b  = B.UnitsLen > i ? B.Units[i] : 0;

        const uint32_t c = a + b + ThisCycleAdd;

        if (c < a && c < b)
            NextCycleAdd = 1;

        result.Units[i] = c;
    }

    return result;
}

static  uint32_t CharNumberToInteger(const char * input,const unsigned int inputSize)
{
    uint32_t res = 0;

    for (int i = 0;i < inputSize ;i++)
        res += (input[inputSize - i - 1]-0x30) * (uint32_t)pow(10,inputSize - i - 1);

    return res;
}

NumberObj stringToNumber(const char* input)
{
    NumberObj Number;
    Number.Decimal = (IntegerObj){0};
    Number.Integer = (IntegerObj){0};
    Number.Sign = 0;

    IntegerObj Integer;

    Integer.Digits = 0;
    Integer.Units = NULL;
    Integer.UnitsLen = 0;


    char * loader = NULL;
    unsigned int loaderSize = 0;

    for (int charIndex = 0; charIndex < strlen(input);charIndex++)
    {
        loaderSize++;
        loader = realloc(loader, loaderSize * sizeof(char));
        loader[loaderSize-1] = input[strlen(input)-charIndex-1];

        Integer.Digits++;

        if ((charIndex % 9 == 0 && charIndex) || charIndex == strlen(input)-1)
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

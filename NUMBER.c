//
// Created by calle on 2025/6/12.
//

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "STDMIO.h"

extern inline uint32_t ePow(const unsigned int e)
{
    uint32_t res = 1;

    for (unsigned int i = 0; i < e; i++)
        res *= 10;

    return res;
}

void printIntegerObj(const IntegerObj Obj)
{
    printf("Digits: %d\n",Obj.Digits);

    printf("REMEMBER:HIGH TO LOW POSITION:\n");
    for (unsigned int i = 0;i < Obj.UnitsLen; i++)
        printf("%lu ",Obj.Units[i]);

    printf("\nUnit Len: %d\n\n",Obj.UnitsLen);
}

IntegerObj IntegerMul(const IntegerObj A,const IntegerObj B)
{
    IntegerObj result;
    result.Digits = 0;
    result.UnitsLen = A.UnitsLen + B.UnitsLen;
    result.Units = calloc(result.UnitsLen, sizeof(uint32_t));

    const unsigned int maxLen = A.UnitsLen > B.UnitsLen ? A.UnitsLen : B.UnitsLen;
    const unsigned int maxDigit = A.Digits > B.Digits ? A.Digits : B.Digits;

    for (unsigned int x = 0;x<maxLen; x++) //O(A.UnitsLen * B.UnitsLen)
    {
        const uint32_t a = A.UnitsLen > x ? A.Units[x] : 0;

       for (unsigned int y = 0;y<maxLen; y++)
        {
            const uint32_t b = B.UnitsLen > y ? B.Units[y] : 0;

            const uint64_t c = a * b;

            const uint32_t lowC = c % 1000000000;
            const uint32_t highC = c / 1000000000;

            const IntegerObj cPack = {
                .UnitsLen = x+y+2,
                .Units = calloc(x+y+2 ,sizeof(uint32_t)),
            };

            cPack.Units[x+y+0] = lowC;
            cPack.Units[x+y+1] = highC;

            result = IntegerAdd(cPack,result);

            free(cPack.Units);
        }
    }

    for (unsigned int i = result.UnitsLen ;i>0; i--)
        if (!result.Units[i-1])
        {
            result.UnitsLen--;
            result.Units = realloc(result.Units, result.UnitsLen * sizeof(uint32_t));
        }
        else
            break;


    if (result.UnitsLen)
    {
        result.Digits = 9*(result.UnitsLen-1);

        for (unsigned int j = 0; j < 9; j++)
            if ((result.Units[result.UnitsLen-1] / ePow(9 - j - 1)))
            {
                result.Digits+=9 - (j-1) - 1;
                break;
            }
    }else
        result.Digits = 0;

    printIntegerObj(result);


    return result;
}

IntegerObj IntegerDiv(const IntegerObj A,const IntegerObj B)
{
    IntegerObj result;
    result.Digits = 0;
    result.Units = NULL;
    result.UnitsLen = 0;

    const unsigned int maxLen = A.UnitsLen > B.UnitsLen ? A.UnitsLen : B.UnitsLen;
    const unsigned int maxDigit = A.Digits > B.Digits ? A.Digits : B.Digits;

    result.UnitsLen = maxLen;
    result.Units = malloc(result.UnitsLen * sizeof(uint32_t));

    for (int x = (int)A.UnitsLen; x >= 0; x--)
    {
        const uint32_t a  = A.UnitsLen > x ? A.Units[x] : 0;

        for (unsigned int y = 0; y < B.UnitsLen; y++)
        {
            const uint32_t b  = B.UnitsLen > y ? B.Units[y] : 0;

            uint32_t c = (uint32_t)(a / b);
            c *= ePow(x - y);

            uint32_t d = (uint32_t)(a % b);

        }
    }

    return result;
}

//todo 除法：(817*m^2 + 626*m + 123) / 545


IntegerObj IntegerSub(const IntegerObj A,const IntegerObj B)
{
    IntegerObj result;
    result.Digits = 0;
    result.Units = NULL;
    result.UnitsLen = 0;

    const unsigned int maxLen = A.UnitsLen > B.UnitsLen ? A.UnitsLen : B.UnitsLen;
    const unsigned int maxDigit = A.Digits > B.Digits ? A.Digits : B.Digits;

    result.Digits += maxDigit;

    uint8_t NextCycleSub = 0;

    result.UnitsLen = maxLen;
    result.Units = realloc(result.Units, result.UnitsLen * sizeof(uint32_t));

    for (unsigned int i = 0;i<result.UnitsLen; i++)
    {
        const uint8_t ThisCycleSub = NextCycleSub;
        NextCycleSub = 0;

        const uint32_t a  = A.UnitsLen > i ? A.Units[i] : 0;
        const uint32_t b  = B.UnitsLen > i ? B.Units[i] : 0;

        uint32_t c = a - b - ThisCycleSub;

        if (c > 999999999)
        {
            NextCycleSub = 1;
            c = 1000000000 - b - ThisCycleSub + a;
        }

        result.Units[i] = c;
    }


    for (unsigned int i = result.UnitsLen ;i>0; i--)
        if (!result.Units[i-1])
        {
            result.UnitsLen--;
            result.Units = realloc(result.Units, result.UnitsLen * sizeof(uint32_t));
        }
        else
            break;

    if (result.UnitsLen)
    {
        result.Digits = 9*(result.UnitsLen-1);

        for (unsigned int j = 0; j < 9; j++)
            if ((result.Units[result.UnitsLen-1] / ePow(9 - j - 1)))
            {
                result.Digits+=9 - (j-1) - 1;
                break;
            }
    }else
        result.Digits = 0;


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

    for (unsigned int i = 0;i<result.UnitsLen; i++)
    {
        const uint8_t ThisCycleAdd = NextCycleAdd;
        NextCycleAdd = 0;

        const uint32_t a  = A.UnitsLen > i ? A.Units[i] : 0;
        const uint32_t b  = B.UnitsLen > i ? B.Units[i] : 0;

        uint32_t c = a + b + ThisCycleAdd;

        if (c > 999999999) //(c < a && c < b) IMPOSSIBLE TO HAPPEN (I hope)
        {
            NextCycleAdd = 1;
            c -= 1000000000;

            if (result.UnitsLen-1 == i)
            {
                result.Digits ++;
                result.UnitsLen++;
                result.Units = realloc(result.Units, result.UnitsLen * sizeof(uint32_t));
            }
        }

        result.Units[i] = c;
    }

    return result;
}


static  uint32_t CharNumberToInteger(const char * input,const unsigned int inputSize)
{
    uint32_t res = 0;

    for (int i = 0;i < inputSize ;i++)
        res += (input[inputSize - i - 1] - 0x30)*ePow(inputSize - i - 1);



    return res;
}

NumberObj stringToNumber(const char* input)
{
    NumberObj Number;
    Number.Decimal = 0;
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

        if (((charIndex+1) % 9 == 0 && charIndex) || charIndex == strlen(input)-1)
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
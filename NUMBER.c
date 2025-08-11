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
    result.Units = NULL;
    result.UnitsLen = 0;

    const unsigned int maxLen = A.UnitsLen > B.UnitsLen ? A.UnitsLen : B.UnitsLen;
    const unsigned int maxDigit = A.Digits > B.Digits ? A.Digits : B.Digits;

    result.UnitsLen = A.UnitsLen + B.UnitsLen;
    result.Units = realloc(result.Units, result.UnitsLen * sizeof(uint32_t));

    for (unsigned int i = 0;i<maxLen; i++)
    {
        const uint32_t a  = A.UnitsLen > i ? A.Units[i] : 0;
        const uint32_t b  = B.UnitsLen > i ? B.Units[i] : 0;

        // a : a3:1000, a2:2000, a1:3000 即 300020001000
        // *
        // b : b3:3000, b2:4000, b1:5000 即 500040003000
        // =
        // c

        // c1 = a1 * b1
    }

    return result;
}


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


    for (unsigned int i = result.UnitsLen ;i>0; i--) //I hope no one will make unit len over than 2147483647
        if (result.Units[i-1])
            break;
        else{
            result.UnitsLen--;
            result.Units = realloc(result.Units, result.UnitsLen * sizeof(uint32_t));
        }

    result.Digits = 9*(result.UnitsLen-1);

    for (unsigned int j = 0; j < 9; j++)
        if ((result.Units[result.UnitsLen-1] / ePow(9 - j - 1))) //這東西他媽卡了我20分鐘，我才意識到可以吃真跳
        {
            result.Digits+=9 - (j-1) - 1;
            break;
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
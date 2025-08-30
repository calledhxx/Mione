//
// Created by calle on 24-12-28.
//

#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../STDMIO.h"

HeadFunctionRespondObj SET(const HeadFunctionRequestObj * HeadCallObjectPointer)
{
    HeadFunctionRespondObj Result = {0};

    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;


    VariableObjPtrCarrier HeadSuffix = {0};

    ValueObjCarrier SetPromptSuffix = {0};

    unsigned int Registration = 0;

    for (unsigned int PairIndex = 0; PairIndex < PairsSize; PairIndex++)
    {
        const CarriageObj Pair = Pairs[PairIndex];

        switch (Pair.CarriageManager.ObjType)
        {
        case HEAD:
            {
                HeadSuffix = REQUEST(Pair.CarriagePassengers);
                break;
            }
        case PROMPT:
            {
                switch (Pair.CarriageManager.Prompt.Identification)
                {
                case 1:
                    {
                        SetPromptSuffix = COUNT(Pair.CarriagePassengers);
                        break;
                    }
                default: exit(-5);

                }

                Registration |= 1<<(Pair.CarriageManager.Prompt.Identification-1);

                break;
            }
        default: exit(-3);
        }
    }

    for (int i = 0; i < 32;i++)
    {
        const int cmp = 1<<i;

        switch (Registration & cmp)
        {
        case 0: continue;
        case 1<<(PROMPT_SET-1):
            {
                Result.MajorVariables.Carrier = realloc(
                    Result.MajorVariables.Carrier,
                    (Result.MajorVariables.CarrierLen + HeadSuffix.CarrierLen)*sizeof(VariableObj)
                    );

                unsigned len = _min(HeadSuffix.CarrierLen,SetPromptSuffix.CarrierLen);

                for (
                    unsigned int HeadSuffixIndex = 0;
                    HeadSuffixIndex <  len;
                    HeadSuffixIndex++
                    )
                {
                    Result.MajorVariables.CarrierLen++;
                    HeadSuffix.Carrier[HeadSuffixIndex]->Value = SetPromptSuffix.Carrier[HeadSuffixIndex];
                    Result.MajorVariables.Carrier[Result.MajorVariables.CarrierLen-1] = *HeadSuffix.Carrier[HeadSuffixIndex];
                }

                break;
            }
        default: exit(-3);
        }
    }

    return Result;
}

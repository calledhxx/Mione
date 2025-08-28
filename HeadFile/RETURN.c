//
// Created by calle on 24-12-28.

#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../STDMIO.h"

EventObj RETURN(const HeadCallObj * HeadCallObjectPointer)
{
    EventObj Result = {0};

    const HeadCallObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;

    ValueObjCarrier HeadSuffix = {0};

    for (unsigned int PairIndex = 0; PairIndex < PairsSize; PairIndex++)
    {
        const CarriageObj Pair = Pairs[PairIndex];

        switch (Pair.CarriageManager.ObjType)
        {
        case HEAD:
            {
                HeadSuffix = COUNT(Pair.CarriagePassengers);

                Result.ToState |= EVENT_RETURN_VALUES;
                Result.ReturnValues = HeadSuffix;

                break;
            }

        default: exit(-5);
        }
    }


    return Result;
}

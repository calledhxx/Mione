#include <stdlib.h>
#include <windows.h>

#include "../STDMIO.h"

EventObj LIBRARY(const HeadCallObj * HeadCallObjectPointer)
{

    EventObj Result = {0};

    const HeadCallObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;

    ValueObjCarrier HeadSuffix = {0};
    VariableObjPtrCarrier IntoPromptSuffix = {0};

    unsigned int Registration = 0;

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
        case PROMPT:
            {
                switch (Pair.CarriageManager.Prompt.Identification)
                {
                case 3:
                    {
                        IntoPromptSuffix = REQUEST(Pair.CarriagePassengers);
                        break;
                    }
                default: exit(1);

                }

                Registration |= 1<<(Pair.CarriageManager.Prompt.Identification-1);

                break;
            }

        default: exit(3);
        }
    }

    if (Registration & 4)
    {
        if (HeadSuffix.CarrierLen != 1)
            exit(101);

        if (HeadSuffix.Carrier[0].ValueType != VALUE_STRING_TYPE)
            exit(102);


        HMODULE LIB = LoadLibraryA(HeadSuffix.Carrier[0].String);
        if (LIB == NULL)
            exit(103);
    }

    return Result;
}
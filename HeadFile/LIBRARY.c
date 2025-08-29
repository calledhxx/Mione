#include <stdlib.h>
#include <libloaderapi.h>

#include "../STDMIO.h"

HeadFunctionRespondObj LIBRARY(const HeadFunctionRequestObj * HeadCallObjectPointer)
{

    HeadFunctionRespondObj Result = {0};

    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;

    VariableObjPtrCarrier HeadSuffix = {0};
    ValueObjCarrier FromPromptSuffix = {0};

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
                case 6:
                    {
                        FromPromptSuffix = COUNT(Pair.CarriagePassengers);
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

    if (Registration & 32)
    {
        if (HeadSuffix.CarrierLen != 1)
            exit(101);

        if (FromPromptSuffix.CarrierLen != 1)
            exit(102);

        const HMODULE LibraryDll = LoadLibrary(FromPromptSuffix.Carrier[0].String);

        if (LibraryDll == NULL)
            exit(105);

        HeadSuffix.Carrier[0]->Value = (ValueObj){
            .ValueType = VALUE_WINDOWS_LIBRARY_TYPE,
            .WindowsLibrary = LibraryDll,
        };

        Result.MajorVariables.CarrierLen++;
        Result.MajorVariables.Carrier = realloc(
            Result.MajorVariables.Carrier,
            sizeof(VariableObj) * Result.MajorVariables.CarrierLen
            );
        Result.MajorVariables.Carrier[
            Result.MajorVariables.CarrierLen - 1] = *HeadSuffix.Carrier[0];
    }else exit(-154);

    return Result;
}
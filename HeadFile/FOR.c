#include "../STDMIO.h"

HeadFunctionRespondObj FOR(const HeadFunctionRequestObj * HeadCallObjectPointer)
{
    HeadFunctionRespondObj Result = {0};
    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;

    ValueObjCarrier HeadSuffix = {0};

    unsigned int Registration = 0;

    for (unsigned int PairIndex = 0; PairIndex < PairsSize; PairIndex++)
    {
        const CarriageObj Pair = Pairs[PairIndex];

        switch (Pair.CarriageManager.ObjType)
        {
        case HEAD:
            {
                HeadSuffix = RESOURCE((RESOURCERequestObj){
                    .MioneCarrier = Pair.CarriagePassengers,
                    .EventTemplate = HeadCallObject.EventTemplate
                }).ValueCarrier;
                break;
            }
        case PROMPT:
            {
                switch (Pair.CarriageManager.Prompt.Identification)
                {

                case 7:
                    {

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

    return Result;
}
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../STDMIO.h"

HeadFunctionRespondObj SVV(const HeadFunctionRequestObj * HeadCallObjectPointer)
{
     HeadFunctionRespondObj Result = {0};
    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;

    ValueObjCarrier RESOURCEHeadSuffix = {0};
    VariableObjPtrCarrier CONTAINERHeadSuffix = {0};

    unsigned int Registration = 0;

    for (unsigned int PairIndex = 0; PairIndex < PairsSize; PairIndex++)
    {
        const CarriageObj Pair = Pairs[PairIndex];
        const PASSENGERRespondObj PassengersRes = PASSENGERS((PASSENGERRequestObj){
            .EventTemplate = HeadCallObject.EventTemplate,
            .PassenegrsCarrier = Pair.CarriagePassengersCarrier
        });
        const MioneObjCarrier Passengers = PassengersRes.MioneCarrier;

        switch (Pair.CarriageManager.ObjType)
        {
        case VARIABLE:
        case VALUE:
            {
                RESOURCEHeadSuffix = RESOURCE((RESOURCERequestObj){
                    .MioneCarrier = Passengers,
                    .EventTemplate = HeadCallObject.EventTemplate
                }).ValueCarrier;

                break;
            }
        case PROMPT:
            {
                switch (Pair.CarriageManager.Prompt.Identification)
                {
                case PROMPT_SET:
                    {
                        CONTAINERHeadSuffix = CONTAINER((CONTAINERRequestObj){
                            .MioneCarrier = Passengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).VariablePtrCarrier;

                        // CONTAINERHeadSuffix.Carrier[0] -> Value //todo
                        break;
                    }
                default: exit(5);

                }

                Registration |= 1<<(Pair.CarriageManager.Prompt.Identification-1);

                break;
            }

        default: exit(3);
        }
    }

    return (HeadFunctionRespondObj){0};
}

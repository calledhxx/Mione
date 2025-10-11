//
// Created by calle on 24-12-28.
//

#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../STDMIO.h"

HeadFunctionRespondObj WHILE(const HeadFunctionRequestObj * HeadCallObjectPointer)
{
    HeadFunctionRespondObj Result = {0};

    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;

    ValueObjCarrier HeadSuffix = {0};
    ValueObjCarrier DoPromptSuffix = {0};

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
        case HEAD:
            {
                HeadSuffix = RESOURCE((RESOURCERequestObj){
                            .MioneCarrier = Passengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).ValueCarrier;
                break;
            }
        case PROMPT:
            {
                switch (Pair.CarriageManager.Prompt.Identification)
                {
                case PROMPT_DO:
                    {
                        DoPromptSuffix = RESOURCE((RESOURCERequestObj){
                            .MioneCarrier = Passengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).ValueCarrier;
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

    if (HeadSuffix.CarrierLen != 1)
        exit(-1111);


    if (DoPromptSuffix.CarrierLen != 1)
        exit(-2223);

    if (DoPromptSuffix.Carrier[0].ValueType != VALUE_RANGE_TYPE)
        exit(-2223);

    if (Registration & 1024)
    {
        while (HeadSuffix.Carrier[0].ValueType == VALUE_DB_TYPE ? HeadSuffix.Carrier[0].db : 1)
        {
             const IMPLEMENTFunctionRespondObj Res = IMPLEMENT((IMPLEMENTFunctionRequestObj){
                .EventTemplate = HeadCallObject.EventTemplate,
                .Built = *DoPromptSuffix.Carrier[0].Area.TrainObjCarrierPointer
            });

            // FreeVariableByTheirCarrier(Res.MajorVariables);
        }
    }

    return Result;
}

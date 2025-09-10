#include "../STDMIO.h"

HeadFunctionRespondObj FOR(const HeadFunctionRequestObj * HeadCallObjectPointer)
{
    HeadFunctionRespondObj Result = {0};
    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;

    VariableObjPtrCarrier HeadSuffix = {0};
    ValueObjCarrier OfPromptSuffix = {0};
    ValueObjCarrier DoPromptSuffix = {0};

    unsigned int Registration = 0;

    for (unsigned int PairIndex = 0; PairIndex < PairsSize; PairIndex++)
    {
        const CarriageObj Pair = Pairs[PairIndex];

        switch (Pair.CarriageManager.ObjType)
        {
        case HEAD:
            {
                HeadSuffix = CONTAINER((CONTAINERRequestObj){
                    .MioneCarrier = Pair.CarriagePassengers,
                    .EventTemplate = HeadCallObject.EventTemplate
                }).VariablePtrCarrier;
                break;
            }
        case PROMPT:
            {
                switch (Pair.CarriageManager.Prompt.Identification)
                {

                case PROMPT_OF:
                    {
                        OfPromptSuffix = RESOURCE((RESOURCERequestObj){
                            .MioneCarrier = Pair.CarriagePassengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).ValueCarrier;
                        break;
                    }
                case PROMPT_DO:
                    {
                        DoPromptSuffix = RESOURCE((RESOURCERequestObj){
                            .MioneCarrier = Pair.CarriagePassengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).ValueCarrier;
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

    if (HeadSuffix.CarrierLen != 1)
        exit(155);

    if (OfPromptSuffix.CarrierLen != 1)
        exit(156);

    if (OfPromptSuffix.Carrier[0].ValueType != VALUE_TABLE_TYPE)
        exit(157);

    if (DoPromptSuffix.CarrierLen != 1)
        exit(158);

    if (DoPromptSuffix.Carrier[0].ValueType != VALUE_RANGE_TYPE)
        exit(159);


    for (
        unsigned TableIndex = 0;
        TableIndex<OfPromptSuffix.Carrier[0].Table.VariableObjCarrierPointer->CarrierLen;
        TableIndex++)
    {
        printf("yes\n");
    }

    return Result;
}
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

    const IMPLEMENTFunctionRequestObj IMPLEMENTFunctionRequest = {
        .Built = *DoPromptSuffix.Carrier[0].Area.TrainObjCarrierPointer,
        .EventTemplate = HeadCallObject.EventTemplate
    };

    for (
        unsigned TableChildIndex = 0;
        TableChildIndex<OfPromptSuffix.Carrier[0].Table.VariableObjCarrierPointer->CarrierLen;
        TableChildIndex++
        )
    {
        if (OfPromptSuffix.Carrier[0].Table.VariableObjCarrierPointer->Carrier[TableChildIndex].VariablePlace)
            HeadSuffix.Carrier[0]->Value =
                (ValueObj){
                    .ValueType = VALUE_NUMBER_TYPE,
                    .Number = OfPromptSuffix.Carrier[0].Table.VariableObjCarrierPointer->Carrier[TableChildIndex].VariablePlace
                };
        else
            HeadSuffix.Carrier[0]->Value =
                (ValueObj){
                    .ValueType = VALUE_STRING_TYPE,
                    .String = OfPromptSuffix.Carrier[0].Table.VariableObjCarrierPointer->Carrier[TableChildIndex].VariableName
                };

        const IMPLEMENTFunctionRespondObj IMPLEMENTFunctionRespond =
            IMPLEMENT(IMPLEMENTFunctionRequest);

        printf("loop ended with normal eve\n");
    }

    return Result;
}
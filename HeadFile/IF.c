#include "../STDMIO.h"

HeadFunctionRespondObj IF(const HeadFunctionRequestObj * HeadCallObjectPointer)
{
    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;

    HeadFunctionRespondObj Result = {0};
    Result.Event = HeadCallObject.EventTemplate;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;

    ValueObjCarrier HeadSuffix = {0};
    ValueObjCarrier ThenPromptSuffix = {0};
    ValueObjCarrier ElsePromptSuffix = {0};

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

                if (HeadSuffix.CarrierLen != 1)
                {
                    Result.Event.Code = EVENT_HEAD_SUFFIX_ERROR;
                    Result.Event.Message = "Suffix of IF Head should be only a resource.";
                    Result.Event.EventPosition = Pair.CarriageManager.MioneObjectPosition;

                    goto end;
                }

                break;

            }
        case PROMPT:
            {
                switch (Pair.CarriageManager.Prompt.Identification)
                {
                case PROMPT_THEN:
                    {
                        ThenPromptSuffix = RESOURCE((RESOURCERequestObj){
                            .MioneCarrier = Passengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).ValueCarrier;

                        if (ThenPromptSuffix.CarrierLen != 1)
                        {
                            Result.Event.Code = EVENT_PROMPT_SUFFIX_ERROR;
                            Result.Event.Message = "Suffix of Then Prompt should be only a resource.";
                            Result.Event.EventPosition = Pair.CarriageManager.MioneObjectPosition;

                            goto end;
                        }

                        if (ThenPromptSuffix.Carrier[0].ValueType != VALUE_RANGE_TYPE)
                        {
                            Result.Event.Code = EVENT_PROMPT_SUFFIX_ERROR;
                            Result.Event.Message = "Suffix of Then Prompt should be a resource of RANGE VALUE";
                            Result.Event.EventPosition = Pair.CarriageManager.MioneObjectPosition;

                            goto end;
                        }

                        break;
                    }
                case PROMPT_ELSE:
                    {
                        ElsePromptSuffix = RESOURCE((RESOURCERequestObj){
                                    .MioneCarrier = Passengers,
                                    .EventTemplate = HeadCallObject.EventTemplate
                                }).ValueCarrier;

                        if (ElsePromptSuffix.CarrierLen != 1)
                        {
                            Result.Event.Code = EVENT_PROMPT_SUFFIX_ERROR;
                            Result.Event.Message = "Suffix of Else Prompt should be only a resource.";
                            Result.Event.EventPosition = Pair.CarriageManager.MioneObjectPosition;

                            goto end;
                        }

                        if (ElsePromptSuffix.Carrier[0].ValueType != VALUE_RANGE_TYPE)
                        {
                            Result.Event.Code = EVENT_PROMPT_SUFFIX_ERROR;
                            Result.Event.Message = "Suffix of Else Prompt should be a resource of RANGE VALUE";
                            Result.Event.EventPosition = Pair.CarriageManager.MioneObjectPosition;

                            goto end;
                        }

                        break;
                    }
                default: exit(-5);

                }

                Registration |= 1<<(Pair.CarriageManager.Prompt.Identification-1);

                break;
            }

        default: exit(-5);
        }
    }

    if (!(Registration & 256))
    {
        Result.Event.Code = EVENT_TRAIN_ERROR;
        Result.Event.Message = "Meaningless or incomprehensible Trian.";
        Result.Event.EventPosition = Pairs[0].CarriageManager.MioneObjectPosition;

        goto end;
    }

    const AreaObj ThenRange = ThenPromptSuffix.Carrier[0].Area;
    const AreaObj ElseRange = ElsePromptSuffix.Carrier[0].Area;

    const IMPLEMENTFunctionRespondObj IMPLEMENTReturn =
        IMPLEMENT((IMPLEMENTFunctionRequestObj){
            .Built = HeadSuffix.Carrier[0].ValueType == VALUE_DB_TYPE && HeadSuffix.Carrier[0].db == 0 ?
                *ElseRange.TrainObjCarrierPointer :
                *ThenRange.TrainObjCarrierPointer
            ,
            .EventTemplate = HeadCallObject.EventTemplate,
        });

    MainEventHandler(IMPLEMENTReturn.Event);

    end:

    return Result;
}
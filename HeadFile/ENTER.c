#include <windows.h>
#include "../STDMIO.h"

HeadFunctionRespondObj ENTER(const HeadFunctionRequestObj * HeadCallObjectPointer)
{
    HeadFunctionRespondObj Result = {0};
    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;

    ValueObjCarrier HeadSuffix = {0};
    ValueObjCarrier OfPromptSuffix = {0};
    ValueObjCarrier RequestPromptSuffix = {0};
    VariableObjPtrCarrier RespondPromptSuffix = {0};

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
                case PROMPT_REQUEST:
                    {
                        RequestPromptSuffix = RESOURCE((RESOURCERequestObj){
                            .MioneCarrier = Passengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).ValueCarrier;
                        break;
                    }
                case PROMPT_RESPOND:
                    {
                        RespondPromptSuffix = CONTAINER((CONTAINERRequestObj){
                            .MioneCarrier = Passengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).VariablePtrCarrier;
                        break;
                    }

                case PROMPT_OF:
                    {
                        OfPromptSuffix = RESOURCE((RESOURCERequestObj){
                            .MioneCarrier = Passengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).ValueCarrier;;
                        break;
                    }
                default: exit(4);

                }

                Registration |= 1<<(Pair.CarriageManager.Prompt.Identification-1);

                break;
            }

        default: exit(3);
        }
    }

    if (Registration & 64)
    {
        if (HeadSuffix.CarrierLen != 1)
            exit(101);

        if (OfPromptSuffix.CarrierLen != 1)
            exit(102);

        if (RequestPromptSuffix.CarrierLen != 1)
            exit(103);

        if (RespondPromptSuffix.CarrierLen != 1)
            exit(104);

        if (OfPromptSuffix.Carrier[0].ValueType != VALUE_WINDOWS_LIBRARY_TYPE)
            exit(106);

        if (HeadSuffix.Carrier[0].ValueType != VALUE_STRING_TYPE)
            exit(107);

        ExternalLibraryRespondObj
            (*EntranceAddress)(ExternalLibraryRequestObj) = GetProcAddress(
                    OfPromptSuffix.Carrier[0].WindowsLibrary,
                    HeadSuffix.Carrier[0].String
                );

        if (EntranceAddress)
            EntranceAddress((ExternalLibraryRequestObj){
                .ValueCarrier = RequestPromptSuffix
            });
        else
            exit(-507);

    }else exit(-154);

    return Result;
}
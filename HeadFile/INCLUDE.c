#include "../STDMIO.h"
#include "ENTER.h"

HeadFunctionRespondObj INCLUDE(const HeadFunctionRequestObj * HeadCallObjectPointer)
{
    HeadFunctionRespondObj Result = {0};

    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;

    ValueObjCarrier HeadSuffix = {0};
    VariableObjPtrCarrier AsPromptSuffix = {0};

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
                case PROMPT_AS:
                    {
                        AsPromptSuffix = CONTAINER((CONTAINERRequestObj){
                            .MioneCarrier = Passengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).VariablePtrCarrier;
                        break;
                    }
                    default: exit (1234);
                }

                Registration |= 1<<(Pair.CarriageManager.Prompt.Identification-1);

                break;
            }

        default: exit(-5);
        }
    }

    if (Registration & 128)
    {
        if (HeadSuffix.CarrierLen != 1)
            exit(-6);

        if (HeadSuffix.Carrier[0].ValueType != VALUE_STRING_TYPE)
            exit(-7);

        FILE * File = fopen(HeadSuffix.Carrier[0].String,"r");

        if (File == NULL)
            exit(-8);

        ScopeObj newScope = {0};
        newScope.ChildrenScopePtrCarrierPointer = malloc(sizeof(ScopeObjPtrCarrier));
        *newScope.ChildrenScopePtrCarrierPointer = (ScopeObjPtrCarrier){0};

        const EventObj EventTemplate = (EventObj){
            .Address = HeadSuffix.Carrier[0].String
        };

        const FCOFunctionRespondObj FCOReturn = FCO((FCOFunctionRequestObj){.f = File, .EventTemplate = EventTemplate});

        const CMOFunctionRespondObj CMOReturn = CMO((CMOFunctionRequestObj){.EventTemplate = EventTemplate, .CassCarrier = FCOReturn.CaseCarrier, .ScopePointer = &newScope});

        const MIONEFunctionRespondObj ToMioneReturn = ToMione((MIONEFunctionRequestObj){ .EventTemplate = EventTemplate, .MioneCarrier = CMOReturn.MioneCarrier});

        const IMPLEMENTFunctionRespondObj IMPLEMENTReturn = IMPLEMENT((IMPLEMENTFunctionRequestObj){
            .Built = ToMioneReturn.TrainCarrier,
            .EventTemplate = EventTemplate,
        });

        if (!AsPromptSuffix.CarrierLen)
            exit(-9);

        const unsigned len = _min(IMPLEMENTReturn.ReturnValues.CarrierLen,AsPromptSuffix.CarrierLen);

        for (unsigned i = 0; i < len ; i++)
            AsPromptSuffix.Carrier[i]->Value = IMPLEMENTReturn.ReturnValues.Carrier[i];
    }

    return Result;
}
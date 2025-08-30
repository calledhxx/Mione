#include "../STDMIO.h"

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

        switch (Pair.CarriageManager.ObjType)
        {
        case HEAD:
            {
                HeadSuffix = COUNT(Pair.CarriagePassengers);

                if (!HeadSuffix.CarrierLen)
                {
                    HeadSuffix.CarrierLen = 1;
                    HeadSuffix.Carrier = malloc(sizeof(ValueObjCarrier));
                    HeadSuffix.Carrier[0] = (ValueObj){
                        .ValueType = VALUE_NUMBER_TYPE,
                        .Number = 0,
                    };
                }
                Result.ReturnValues = HeadSuffix;

                break;
            }
        case PROMPT:
            {
                switch (Pair.CarriageManager.Prompt.Identification)
                {
                case 8:
                    {
                        AsPromptSuffix = REQUEST(Pair.CarriagePassengers);
                        break;
                    }
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

        const FCOFunctionRespondObj FCOReturn = FCO(File);

        const CMOFunctionRespondObj CMOReturn = CMO(FCOReturn.CaseCarrier,&newScope);

        const MIONEFunctionRespondObj ToMioneReturn = ToMione(CMOReturn.MioneCarrier);

        const IMPLEMENTFunctionRespondObj IMPLEMENTReturn = IMPLEMENT((ToImplementObj){
            .Built = ToMioneReturn.TrainCarrier
        });

        if (!AsPromptSuffix.CarrierLen)
            exit(-9);

        const unsigned len = _min(IMPLEMENTReturn.ReturnValues.CarrierLen,AsPromptSuffix.CarrierLen);

        for (unsigned i = 0; i < len ; i++)
            AsPromptSuffix.Carrier[i]->Value = IMPLEMENTReturn.ReturnValues.Carrier[i];
    }

    return Result;
}
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../STDMIO.h"

HeadFunctionRespondObj GET(const HeadFunctionRequestObj * HeadCallObjectPointer)
{
    HeadFunctionRespondObj Result = {0};

    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.Train.CarriageLen;
    const CarriageObj * Pairs = HeadCallObject.Train.Carriages;

    VariableObjPtrCarrier HeadSuffix = {0};
    VariableObjPtrCarrier ModelPromptSuffix = {0};

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
                case 2:
                    {
                        ModelPromptSuffix = CONTAINER((CONTAINERRequestObj){
                            .MioneCarrier = Pair.CarriagePassengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).VariablePtrCarrier;
                        break;
                    }
                default: exit(-3);

                }

                Registration |= 1<<(Pair.CarriageManager.Prompt.Identification-1);

                break;
            }

        default: exit(-2);
        }
    }

    if (Registration & 2)
    {
        if (HeadSuffix.CarrierLen)
            exit(-101);

        if (ModelPromptSuffix.CarrierLen != 1)
            exit(-99);


        ModelPromptSuffix.Carrier[0]->Value = (ValueObj) {
            .ValueType = VALUE_TABLE_TYPE,
            .Table = (TableObj){
                .TrainObjCarrierPointer = 0,
                .VariableObjCarrierPointer =
                    malloc(sizeof(VariableObjCarrier)),
            }
        };

        VariableObjCarrier Copy = {0};
        Copy.CarrierLen = HeadCallObject.VariablePtrCarrier.CarrierLen;
        Copy.Carrier = malloc(sizeof(VariableObj) * Copy.CarrierLen);

        for (unsigned index = 0; index < Copy.CarrierLen; index++)
            Copy.Carrier[index] = *(HeadCallObject.VariablePtrCarrier.Carrier[index]);
    }else
    {
        if (!HeadSuffix.CarrierLen)
            exit(-101);

        const unsigned len = _min(HeadSuffix.CarrierLen,HeadCallObject.CallByValueCarrier.CarrierLen);

        for (unsigned i = 0; i < len;i++)
            HeadSuffix.Carrier[i]->Value = HeadCallObject.CallByValueCarrier.Carrier[i];
    }


    return Result;
}

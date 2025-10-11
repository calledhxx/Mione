//
// Created by calle on 25-1-3.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "STDMIO.h"


CONTAINERRespondObj CONTAINER(CONTAINERRequestObj input)
{
    VariableObj ** VariablePointerCarrier = NULL;
    int VariablePointerCarrierLen = 0;

    const COMPUTATIONRespondObj Computed = COMPUTATION((COMPUTATIONRequestObj){
        .EventTemplate = input.EventTemplate,
        .MioneCarrier = input.MioneCarrier
    });

    const MioneObj * Pack = Computed.MioneCarrier.Carrier;
    const unsigned int PackSize = Computed.MioneCarrier.CarrierLen;

    for (int i = 0; i < PackSize; i++)
        if (Pack[i].ObjType == VARIABLE)
        {
            VariablePointerCarrierLen ++;
            VariablePointerCarrier = realloc(VariablePointerCarrier, sizeof(ValueObj) * (VariablePointerCarrierLen));
            VariablePointerCarrier[VariablePointerCarrierLen-1] = ReturnVariablePtrFromLink(*Pack[i].VariableLinkPtr);
        }else if ( Pack[i].ObjType == VALUE)
        {
            printf("exit because VALUE APPEARS %d\n", Pack[i].Value.ValueType);

            exit(-2);

        }

    return (CONTAINERRespondObj){
        .VariablePtrCarrier = (VariableObjPtrCarrier){
            .Carrier = VariablePointerCarrier,
        .CarrierLen= VariablePointerCarrierLen
        },
        .Event = Computed.Event,
    };
}
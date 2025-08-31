//
// Created by calle on 24-11-3.
//

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "STDMIO.h"


RESOURCERespondObj RESOURCE(const RESOURCERequestObj input)
{

    ValueObj * ValueCarrier = NULL;
    int ValueCarrierLen = 0;


    const COMPUTATIONRespondObj Computed = COMPUTATION((COMPUTATIONRequestObj){
        .EventTemplate = input.EventTemplate,
        .MioneCarrier = input.MioneCarrier
    });

    const MioneObj * Pack = Computed.MioneCarrier.Carrier;
    const unsigned int PackSize = Computed.MioneCarrier.CarrierLen;


    for (int i = 0; i < PackSize; i++)
        if ( Pack[i].ObjType == VALUE)
        {
            ValueCarrierLen ++;
            ValueCarrier = realloc(ValueCarrier, sizeof(ValueObj) * (ValueCarrierLen));
            ValueCarrier[ValueCarrierLen-1] = Pack[i].Value;
        }else if (Pack[i].ObjType == VARIABLE)
        {
            ValueCarrierLen ++;
            ValueCarrier = realloc(ValueCarrier, sizeof(ValueObj) * (ValueCarrierLen));
            ValueCarrier[ValueCarrierLen-1] = ReturnVariablePtrFromLink(*Pack[i].VariableLinkPtr)->Value;
        }

    return (RESOURCERespondObj){
        .ValueCarrier = (ValueObjCarrier){
            .Carrier = ValueCarrier,
            .CarrierLen= ValueCarrierLen
        },
        .Event = Computed.Event,
    };
}
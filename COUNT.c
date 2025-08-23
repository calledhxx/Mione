//
// Created by calle on 24-11-3.
//

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "STDMIO.h"


ValueObjCarrier COUNT(const MioneObjCarrier input)
{

    ValueObj * ValueCarrier = NULL;
    int ValueCarrierLen = 0;

    const MioneObjCarrier Computed = COMPUTATION(input);

    const MioneObj * Pack = Computed.Carrier;
    const unsigned int PackSize = Computed.CarrierLen;


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
            ValueCarrier[ValueCarrierLen-1] = (*Pack[i].PointerOfScopeVariablePtr)->Value;
        }

    return (ValueObjCarrier){
        .Carrier = ValueCarrier,
        .CarrierLen= ValueCarrierLen
    };
}
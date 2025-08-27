//
// Created by calle on 25-1-3.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "STDMIO.h"


VariableObjPtrCarrier REQUEST(const MioneObjCarrier input)
{
    VariableObj ** VariablePointerCarrier = NULL;
    int VariablePointerCarrierLen = 0;

    const MioneObjCarrier Computed = COMPUTATION(input);

    const MioneObj * Pack = Computed.Carrier;
    const unsigned int PackSize = Computed.CarrierLen;


    for (int i = 0; i < PackSize; i++)
        if (Pack[i].ObjType == VARIABLE)
        {
            VariablePointerCarrierLen ++;
            VariablePointerCarrier = realloc(VariablePointerCarrier, sizeof(ValueObj) * (VariablePointerCarrierLen));
            VariablePointerCarrier[VariablePointerCarrierLen-1] = (*Pack[i].PointerOfScopeVariablePtr);
        }else if ( Pack[i].ObjType == VALUE)
        {
            printf("exit because VALUE APPEARS%d\n", Pack[i].Value.ValueType);

            exit(-2);

        }

    return (VariableObjPtrCarrier){
        .Carrier = VariablePointerCarrier,
        .CarrierLen= VariablePointerCarrierLen
    };
}

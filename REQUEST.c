//
// Created by calle on 25-1-3.
//

#include "OBJECTS.h"
#include <stdlib.h>
#include <string.h>


VariableObjPtrCarrier REQUEST(const MioneObjCarrier input)
{
    const MioneObj * Pack = input.Carrier;
    const unsigned int PackSize = input.CarrierLen;

    VariableObj ** VariablePointerCarrier = NULL;
    int VariablePointerCarrierLen = 0;

    for (int i = 0; i < PackSize; i++)
        if ( Pack[i].ObjType == VALUE)
        {
            exit(-2);
        }else if (Pack[i].ObjType == VARIABLE)
        {
            VariablePointerCarrierLen ++;
            VariablePointerCarrier = realloc(VariablePointerCarrier, sizeof(ValueObj) * (VariablePointerCarrierLen));
            VariablePointerCarrier[VariablePointerCarrierLen-1] = Pack[i].VariablePointer;
        }


    return (VariableObjPtrCarrier){
        .Carrier = VariablePointerCarrier,
        .CarrierLen= VariablePointerCarrierLen
    };
}

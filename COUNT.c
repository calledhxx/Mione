//
// Created by calle on 24-11-3.
//

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ERR.h"
#include "Mione.h"
#include "PROMPT_DEF.h"
#include "SYMBOL_DEF.h"
#include "IMPLEMENT.h"


ValueObjCarrier COUNT(const MioneObjCarrier input)
{
    const MioneObj * Pack = input.Carrier;
    const unsigned int PackSize = input.CarrierLen;

    ValueObj * ValueCarrier = NULL;
    int ValueCarrierLen = 0;

    for (int i = 0; i < PackSize; i++)
        if ( Pack[i].ObjType == VALUE)
        {
            ValueCarrierLen ++;
            ValueCarrier = realloc(ValueCarrier, sizeof(ValueObj) * (ValueCarrierLen));
            ValueCarrier[ValueCarrierLen-1] = Pack[i].Val;
        }else if (Pack[i].ObjType == VARIABLE)
        {
            ValueCarrierLen ++;
            ValueCarrier = realloc(ValueCarrier, sizeof(ValueObj) * (ValueCarrierLen));
            ValueCarrier[ValueCarrierLen-1] = Pack[i].VarUP->Value;
        }


    return (ValueObjCarrier){
        .Carrier = ValueCarrier,
        .CarrierLen= ValueCarrierLen
    };
}
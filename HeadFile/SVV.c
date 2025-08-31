#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../STDMIO.h"

HeadFunctionRespondObj SVV(const HeadFunctionRequestObj * HeadCallObjectPointer)
{
    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;


    const ValueObjCarrier a = RESOURCE((RESOURCERequestObj){
                            .MioneCarrier = HeadCallObject.Train.Carriages[0].CarriagePassengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).ValueCarrier;

    printf("%f\n",a.Carrier[0].Number);

    return (HeadFunctionRespondObj){0};
}

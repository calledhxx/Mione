#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../STDMIO.h"

HeadFunctionRespondObj SVV(const HeadFunctionRequestObj * HeadCallObjectPointer)
{
    const HeadFunctionRequestObj HeadCallObject = *HeadCallObjectPointer;

    const PASSENGERRespondObj PassengersRes = PASSENGERS((PASSENGERRequestObj){
            .EventTemplate = HeadCallObject.EventTemplate,
            .PassenegrsCarrier = HeadCallObject.Train.Carriages[0].CarriagePassengersCarrier
        });
    const MioneObjCarrier Passengers = PassengersRes.MioneCarrier;


    const ValueObjCarrier a = RESOURCE((RESOURCERequestObj){
                            .MioneCarrier = Passengers,
                            .EventTemplate = HeadCallObject.EventTemplate
                        }).ValueCarrier;




    return (HeadFunctionRespondObj){0};
}

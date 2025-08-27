#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../STDMIO.h"

EventObj SVV(const HeadCallObj * HeadCallObjectPointer)
{
    const HeadCallObj HeadCallObject = *HeadCallObjectPointer;

    const ValueObjCarrier a = COUNT(HeadCallObject.Train.Carriages[0].CarriagePassengers);

    printf("%d\n",a.Carrier[0].ValueType);

    return (EventObj){0};
}

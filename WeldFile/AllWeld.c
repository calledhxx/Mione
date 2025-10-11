#include <stdlib.h>
#include <string.h>

#include "../OBJECTS.h"

#include "SUBJECT.h"

WeldObjCarrier ReturnWelds()
{
    WeldObj const Welds[]  = {
        (WeldObj){
            .Name = "subject",
            .Identification = WELD_SUBJECT,
            .Fuc = SUBJECT,
        },
    };

    WeldObjCarrier result = {0};
    result.CarrierLen =
        sizeof(Welds)/sizeof(WeldObj);
    result.Carrier =
        malloc(sizeof(Welds));

    memcpy(
        result.Carrier,
        Welds,
        sizeof(Welds)
        );

    return result;
}
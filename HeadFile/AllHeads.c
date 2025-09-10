#include <stdlib.h>
#include <string.h>

#include "../OBJECTS.h"

#include "SET.h"
#include "RETURN.h"
#include "GET.h"
#include "LIBRARY.h"
#include "ENTER.h"
#include "INCLUDE.h"
#include "IF.h"
#include "SVV.h"



HeadObjCarrier ReturnHeadList()
{
    const HeadObj Heads[]  = {
        (HeadObj){
            .Fuc = SET,
            .Name = "set",
            .Identification = HEAD_SET,
        },

        (HeadObj){
            .Fuc = RETURN,
            .Name = "return",
            .Identification = HEAD_RETURN,
        },

        (HeadObj){
            .Fuc = GET,
            .Name = "get",
            .Identification = HEAD_GET,
        },

        (HeadObj){
            .Fuc = LIBRARY,
            .Name = "library",
            .Identification = HEAD_LIBRARY,
        },

        (HeadObj){
            .Fuc = ENTER,
            .Name = "enter",
            .Identification = HEAD_ENTER,
        },

        (HeadObj){
            .Fuc = INCLUDE,
            .Name = "include",
            .Identification = HEAD_INCLUDE,
        },

        (HeadObj){
            .Fuc = IF,
            .Name = "if",
            .Identification = HEAD_IF,
        }
    };

    HeadObjCarrier result = {0};
    result.CarrierLen =
        sizeof(Heads)/sizeof(HeadObj);
    result.Carrier =
        malloc(sizeof(Heads));

    memcpy(
        result.Carrier,
        Heads,
        sizeof(Heads)
        );

    return result;
}
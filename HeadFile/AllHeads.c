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
            .Identification = 1,
        },

        (HeadObj){
            .Fuc = RETURN,
            .Name = "return",
            .Identification = 2,
        },

        (HeadObj){
            .Fuc = GET,
            .Name = "get",
            .Identification = 3,
        },

        (HeadObj){
            .Fuc = LIBRARY,
            .Name = "library",
            .Identification = 4,
        },

        (HeadObj){
            .Fuc = ENTER,
            .Name = "enter",
            .Identification = 5,
        },

        (HeadObj){
            .Fuc = INCLUDE,
            .Name = "include",
            .Identification = 6,
        },

        (HeadObj){
            .Fuc = IF,
            .Name = "if",
            .Identification = 7,
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
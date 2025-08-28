//
// Created by calle on 25-1-18.
//
#include <stdlib.h>
#include <string.h>

#include "OBJECTS.h"



PromptObjCarrier ReturnPromptList()
{
    const PromptObj Prompts[] = {
        (PromptObj) {
            .Name = "=",
            .Identification = 1,
        },

        (PromptObj) {
            .Name = "model",
            .Identification = 2,
        },

        (PromptObj) {
            .Name = "into",
            .Identification = 3,
        },
    };

    PromptObjCarrier result = {0};
    result.CarrierLen =
        sizeof(Prompts)/sizeof(Prompts[0]);
    result.Carrier =
        malloc(sizeof(PromptObj) * result.CarrierLen);

    memcpy(
        result.Carrier,
        Prompts,
        sizeof(PromptObj) * result.CarrierLen
        );

    return result;
}
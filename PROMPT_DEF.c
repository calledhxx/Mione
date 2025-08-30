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
            .Name = "request",
            .Identification = 3,
        },

        (PromptObj) {
            .Name = "position",
            .Identification = 4,
        },

        (PromptObj) {
            .Name = "respond",
            .Identification = 5,
        },

        (PromptObj) {
            .Name = "from",
            .Identification = 6,
        },

        (PromptObj) {
            .Name = "of",
            .Identification = 7,
        },

        (PromptObj) {
            .Name = "as",
            .Identification = 8,
        }
    };

    PromptObjCarrier result = {0};
    result.CarrierLen =
        sizeof(Prompts)/sizeof(PromptObj);
    result.Carrier =
        malloc(sizeof(Prompts));

    memcpy(
        result.Carrier,
        Prompts,
        sizeof(Prompts)
        );

    return result;
}
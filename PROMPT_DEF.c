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
        }
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
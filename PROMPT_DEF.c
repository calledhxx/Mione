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
            .Identification = PROMPT_SET,
        },

        (PromptObj) {
            .Name = "model",
            .Identification = PROMPT_MODEL,
        },

        (PromptObj) {
            .Name = "request",
            .Identification = PROMPT_REQUEST,
        },

        (PromptObj) {
            .Name = "position",
            .Identification = PROMPT_POSITION,
        },

        (PromptObj) {
            .Name = "respond",
            .Identification = PROMPT_RESPOND,
        },

        (PromptObj) {
            .Name = "from",
            .Identification = PROMPT_FROM,
        },

        (PromptObj) {
            .Name = "of",
            .Identification = PROMPT_OF,
        },

        (PromptObj) {
            .Name = "as",
            .Identification = PROMPT_AS,
        },

        (PromptObj) {
            .Name = "then",
            .Identification = PROMPT_THEN,
        },

        (PromptObj) {
            .Name = "else",
            .Identification = PROMPT_ELSE,
        },

        (PromptObj) {
            .Name = "do",
            .Identification = PROMPT_DO,
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
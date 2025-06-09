//
// Created by calle on 25-1-27.
//

#include "../OBJECTS.h"
#include "../REQUEST.h"
#include "../COUNT.h"
#include "../ERR.h"
#include "../MIONE.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HeadReturnObj FOR(HeadRequestObj * HeadRequestUP)
{
    HeadRequestObj HeadRequest = *HeadRequestUP;

    PairObj * Pairs = HeadRequest.Pairs;
    int PairsSize = HeadRequest.PairsSize;

    HeadReturnObj ToReturn;
    ToReturn.ToState = 0;

    VariableRequestUPObj RequestedSuffixOfHead = {.VariablesSize = 0};

    int registeredPrompts = 0;

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {
            RequestedSuffixOfHead = REQUEST(Pairs[i].Source, Pairs[i].SourceSize);
            if (RequestedSuffixOfHead.VariablesSize !=1)ErrCall("`for` only accept ONE VARIABLE.","M007",NULL,Prompt.Line,Prompt.Column);
        }
        if (Prompt.ObjType == 2)
        {
            switch (Prompt.Prompt.CurNumber)
            {
            default:
                ErrCall("This Prompt is not supported by `for`","M016",NULL,Prompt.Line,Prompt.Column);
                break;
            }

            if (registeredPrompts&1<<Prompt.Prompt.CurNumber-1) ErrCall("This Prompt has been registered before.","M017",NULL,Prompt.Line,Prompt.Column);
            registeredPrompts |= 1<<Prompt.Prompt.CurNumber-1;
        }
    }
}


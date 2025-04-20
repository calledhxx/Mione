//
// Created by calle on 24-12-28.
//

#include <inttypes.h>

#include "../OBJECTS.h"
#include "../REQUEST.h"
#include "../COUNT.h"
#include "../ERR.h"
#include <stdio.h>

HeadReturnObj RETURN(HeadRequestObj * HeadRequestUP)
{
    HeadRequestObj HeadRequest = *HeadRequestUP;

    PairObj * Pairs = HeadRequest.Pairs;
    int PairsSize = HeadRequest.PairsSize;

    HeadReturnObj ToReturn;
    ToReturn.ToState = 0;


    VariableRequestUPObj Request = {.VariablesSize = 0};
    CountObj Counted = {.ValueSize = 0};



    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {
            CountObj Counted = COUNT(Pairs[i].Source, Pairs[i].SourceSize);

            ToReturn.ToState = ToReturn.ToState+1;
            ToReturn.Values = (ValuesObj){
                .ValueSize = Counted.ValueSize,
                .Value = Counted.Value,
            };
        }
        if (Prompt.ObjType == 2)
        {
            switch (Prompt.Prompt.CurNumber)
            {
            default:
                ErrCall("`return` should be a NON-PROMPT HEAD sentence.","M001","Put a `;` after every end of sentences.",Prompt.Line,Prompt.Column);
                break;
            }
        }
    }


    return ToReturn;
}

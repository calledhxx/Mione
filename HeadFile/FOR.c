//
// Created by calle on 25-1-27.
//


#include "../OBJECTS.h"
#include "../REQUEST.h"
#include "../COUNT.h"
#include "../ERR.h"
#include <stdio.h>

HeadReturnObj FOR(struct _PairObject*Pairs,int PairsSize)
{
    HeadReturnObj ToReturn;
    ToReturn.ToState = 0;


    VariableRequestUPObj Request = {.VariablesSize = 0};
    CountObj Counted = {.ValueSize = 0};


    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {

        }
        if (Prompt.ObjType == 2)
        {
            switch (Prompt.Prompt.CurNumber)
            {
            default:
                ErrCall("unsupported prompt type","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }
        }
    }

    return ToReturn;
}


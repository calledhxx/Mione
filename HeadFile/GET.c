//
// Created by calle on 2025/5/21.
//

#include <io.h>
#include <stdio.h>
#include "../OBJECTS.h"
#include "../REQUEST.h"
#include "../COUNT.h"
#include "../ERR.h"

HeadReturnObj GET(HeadRequestObj * HeadRequestUP)
{
    HeadRequestObj HeadRequest = *HeadRequestUP;


    HeadReturnObj ToReturn = {0};

    PairObj * Pairs = HeadRequest.Pairs;
    int PairsSize = HeadRequest.PairsSize;




    int registeredPrompts = 0;

    VariableRequestUPObj Container;

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1)
        {
            Container = REQUEST(Pairs[i].Source, Pairs[i].SourceSize);
            if (Container.VariablesSize == 0) ErrCall("no variables found",NULL,NULL,0,0);
        }
        if (Prompt.ObjType == 2)
        {

            switch (Prompt.Prompt.CurNumber)
            {
            case 11:
                {
                    if (Pairs[i].SourceSize) ErrCall("ijoaijoadsijo",NULL,NULL,0,0);
                    break;
                }
            default:
                ErrCall("unsupported prompt type","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }

            registeredPrompts |= 1<<Prompt.Prompt.CurNumber-1;
        }
    }

    if (registeredPrompts & (1<<11-1))
    {

    }else
        for (int i = 0; i < Container.VariablesSize; i++)
        {
            if (HeadRequest.FucRequest.ValueSize <= i) break;

            Container.VariableUPs[i]->Val = HeadRequest.FucRequest.Value[i];
        }


    return ToReturn;
};
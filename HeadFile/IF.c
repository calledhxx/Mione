//
// Created by calle on 25-1-26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../OBJECTS.h"
#include "../COUNT.h"
#include "../ERR.h"

HeadReturnObj IF(struct _PairObject*Pairs,int PairsSize){
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

            if(Counted.ValueSize == 1) ErrCall("IF Error","M9121321",NULL,Prompt.Line,Prompt.Column);
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

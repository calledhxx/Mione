//
// Created by calle on 25-1-26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#include "../OBJECTS.h"
#include "../COUNT.h"
#include "../MIONE.h"
#include "../ERR.h"
#include "../IMPLEMENT.h"

HeadReturnObj IF(HeadRequestObj * HeadRequestUP)
{
    HeadRequestObj HeadRequest = *HeadRequestUP;

    PairObj * Pairs = HeadRequest.Pairs;
    int PairsSize = HeadRequest.PairsSize;

    HeadReturnObj ToReturn;
    ToReturn.ToState = 0;


    VariableRequestUPObj Request = {.VariablesSize = 0};

    CountObj CountedDB = {.ValueSize = 0};
    CountObj CountedThenRange = {.ValueSize = 0};
    CountObj CountedElseRange = {.ValueSize = 0};

    int db = 0;

    int registeredPrompts = 0;

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {
            CountedDB = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
            if(CountedDB.ValueSize != 1) ErrCall("After `if` HEAD should be only a SOURCE","M020",NULL,Prompt.Line,Prompt.Column);

            db = (CountedDB.Value[0].ValueType) ? ((CountedDB.Value[0].ValueType != 8) ? 1 : CountedDB.Value[0].db) :0;
        }
        if (Prompt.ObjType == 2)
        {
            switch (Prompt.Prompt.CurNumber)
            {
              case 3:
                  CountedThenRange = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                  if(CountedThenRange.ValueSize != 1) ErrCall("THEN Error","M9121321",NULL,Prompt.Line,Prompt.Column);
                  if(CountedThenRange.Value[0].ValueType != 5) ErrCall("THEN (RANGE) Error","M9121321",NULL,Prompt.Line,Prompt.Column);

                  break;
            case 4:
                CountedElseRange = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                if(CountedElseRange.ValueSize != 1) ErrCall("ELSE Error","M9121321",NULL,Prompt.Line,Prompt.Column);
                if(CountedElseRange.Value[0].ValueType != 5) ErrCall("ELSE (RANGE) Error","M9121321",NULL,Prompt.Line,Prompt.Column);


                break;

            default:
                ErrCall("This Prompt is not supported by IF","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }

            registeredPrompts |= 1<<Prompt.Prompt.CurNumber-1;
        }
    }

    int max = 0;
    for (int i = 0;;i++)
        if (pow(2,i-1) > registeredPrompts)
        {
            max = i-1;
            break;
        }

    ImplementedObj Return;

    for (int i = 0;max>i;i++)
    {
        const int cmp = pow(2,i);

        if (!registeredPrompts) break;

        if (registeredPrompts & cmp)
        {
            switch (i+1)
            {
            case 3:
                if (db)
                {
                    Return = IMPLEMENT((ToImplementObj){
                        .Built = *CountedThenRange.Value[0].Area.AreaUP
                    });
                }
                break;
            case 4:
                if (!db)
                {
                    Return = IMPLEMENT((ToImplementObj){
                        .Built = *CountedElseRange.Value[0].Area.AreaUP
                    });
                }
                break;
            default:
                ErrCall("This Prompt is not supported by IF","M111",NULL,-1,-1);
            }
        }
    }

    return ToReturn;
}

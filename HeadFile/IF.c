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

    
    CountObj CountedSuffixOfHead = {.ValueSize = 0};
    CountObj CountedSuffixOfThenPrompt = {.ValueSize = 0};
    CountObj CountedSuffixOfElsePrompt = {.ValueSize = 0};

    char IfValueIsTure = 0;

    int registeredPrompts = 0;

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {
            CountedSuffixOfHead = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
            if(CountedSuffixOfHead.ValueSize != 1) ErrCall("After `if` HEAD should be only a SOURCE","M020",NULL,Prompt.Line,Prompt.Column);

            IfValueIsTure = (char)(CountedSuffixOfHead.Value[0].ValueType) ? ((CountedSuffixOfHead.Value[0].ValueType != 8) ? 1 : CountedSuffixOfHead.Value[0].db) :0;
        }
        if (Prompt.ObjType == 2)
        {
            switch (Prompt.Prompt.CurNumber)
            {
              case 3:
                  CountedSuffixOfThenPrompt = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                  if(CountedSuffixOfThenPrompt.ValueSize != 1) ErrCall("THEN Error","M9121321",NULL,Prompt.Line,Prompt.Column);
                  if(CountedSuffixOfThenPrompt.Value[0].ValueType != 5) ErrCall("THEN (RANGE) Error","M9121321",NULL,Prompt.Line,Prompt.Column);

                  break;
            case 4:
                CountedSuffixOfElsePrompt = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                if(CountedSuffixOfElsePrompt.ValueSize != 1) ErrCall("ELSE Error","M9121321",NULL,Prompt.Line,Prompt.Column);
                if(CountedSuffixOfElsePrompt.Value[0].ValueType != 5) ErrCall("ELSE (RANGE) Error","M9121321",NULL,Prompt.Line,Prompt.Column);


                break;

            default:
                ErrCall("This Prompt is not supported by IF","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }

            if (registeredPrompts&1<<Prompt.Prompt.CurNumber-1) ErrCall("This Prompt has been registered before.","M017",NULL,Prompt.Line,Prompt.Column);
            registeredPrompts |= 1<<Prompt.Prompt.CurNumber-1;
        }
    }

    int max = 0;
    for (int i = 0;;i++)
        if (1<<i-1 > registeredPrompts)
        {
            max = i-1;
            break;
        }

    ImplementedObj Return;

    for (int i = 0;max>i;i++)
    {
        const int cmp = 1<<i;

        if (!registeredPrompts) break;

        if (registeredPrompts & cmp)
        {
            switch (i+1)
            {
            case 3:
                if (IfValueIsTure)
                {
                    Return = IMPLEMENT((ToImplementObj){
                        .Built = *CountedSuffixOfThenPrompt.Value[0].Area.AreaUP
                    });
                }
                break;
            case 4:
                if (!IfValueIsTure)
                {
                    Return = IMPLEMENT((ToImplementObj){
                        .Built = *CountedSuffixOfElsePrompt.Value[0].Area.AreaUP
                    });
                }
                break;
            default:
                ErrCall("This Prompt is not supported by IF","M111",NULL,-1,-1);
            }
        }
    }

    if (Return.ToState & 1)
    {
        ToReturn.ToState |= 1;
        ToReturn.Values = Return.Values;
    }

    return ToReturn;
}

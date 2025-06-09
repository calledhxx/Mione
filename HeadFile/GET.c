//
// Created by calle on 2025/5/21.
//

#include <io.h>
#include <stdio.h>
#include <stdlib.h>

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

    VariableRequestUPObj RequestedSuffixOfHead = {0};

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1)
        {
            RequestedSuffixOfHead = REQUEST(Pairs[i].Source, Pairs[i].SourceSize);
            if (RequestedSuffixOfHead.VariablesSize == 0) ErrCall("no variables found",NULL,NULL,0,0);
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
        if (RequestedSuffixOfHead.VariablesSize != 1) ErrCall("ijoaijoadsijo",NULL,NULL,0,0);

        VariableUPsObj VariableUPs;
        VariableUPs.VarUPs = malloc(0);
        VariableUPs.VarUPsSize = 0;

        ValueObj Value = (ValueObj){
            .ValueType = VALUE_TABLE_TYPE,
            .Table = (TableObj){
                .VariableUPsUP = malloc(sizeof(VariableUPsObj))
            }
        };

        for (int i = 0; i < HeadRequest.FucRequest.ValueSize; i++)
        {
            VariableUPs.VarUPsSize++;
            VariableUPs.VarUPs = realloc(VariableUPs.VarUPs, VariableUPs.VarUPsSize * sizeof(VariableObj*));

            VariableUPs.VarUPs[VariableUPs.VarUPsSize-1] = malloc(sizeof(VariableObj));
            *VariableUPs.VarUPs[VariableUPs.VarUPsSize-1] = (VariableObj){
                .Place = i+1,
                .Val = HeadRequest.FucRequest.Value[i]
            };
        }

        *Value.Table.VariableUPsUP = VariableUPs;

        RequestedSuffixOfHead.VariableUPs[0]->Val = Value;

    }else
        for (int i = 0; i < RequestedSuffixOfHead.VariablesSize; i++)
        {
            if (HeadRequest.FucRequest.ValueSize <= i) break;

            RequestedSuffixOfHead.VariableUPs[i]->Val = HeadRequest.FucRequest.Value[i];
        }


    return ToReturn;
};
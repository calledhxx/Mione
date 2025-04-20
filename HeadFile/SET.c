//
// Created by calle on 24-12-28.
//

#include <inttypes.h>

#include "../OBJECTS.h"
#include "../REQUEST.h"
#include "../COUNT.h"

#include "../ERR.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HeadReturnObj SET(HeadRequestObj * HeadRequestUP)
{
    HeadRequestObj HeadRequest = *HeadRequestUP;

    PairObj * Pairs = HeadRequest.Pairs;
    int PairsSize = HeadRequest.PairsSize;

    HeadReturnObj ToReturn;
    ToReturn.ToState = 0;

    ToReturn.VAVs.DefinedVariables = malloc(0);
    ToReturn.VAVs.DefinedVariablesSize = 0;

    VariableRequestUPObj Request = {.VariablesSize = 0};
    CountObj Counted = {.ValueSize = 0};

    int set = 0,host = 0;

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {
            Request = REQUEST(Pairs[i].Source, Pairs[i].SourceSize);
            if (Request.VariablesSize == 0) ErrCall("no", "M11176", NULL, Prompt.Line, Prompt.Column);
        }
        if (Prompt.ObjType == 2)
        {
            switch (Prompt.Prompt.CurNumber)
            {
            case 1:
                Counted = COUNT(Pairs[i].Source, Pairs[i].SourceSize);



                if (Counted.ValueSize!=Request.VariablesSize) ErrCall("Variables couldn't be paired with values","M111",NULL,Prompt.Line,Prompt.Column);

                set = 1;

                break;
            case 2:
                if (Pairs[i].SourceSize) ErrCall("`host` CAN NOT BE SET TO ANY SOURCE","M123",NULL,Prompt.Line,Prompt.Column);

                host = 1;

                break;
            default:
                ErrCall("unsupported prompt type","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }
        }
    }

    if (!Request.VariablesSize) ErrCall("no REQUEST?","M111",NULL,Pairs[0].Source[0].Line,Pairs[0].Source[0].Column);

    // `host`

    if (host)
    {
        extern DefineVariableObj * Dvo;
        extern int DvoSize;

        ValueObj V = (ValueObj){.ValueType = 0};

        ToReturn.ToState = ToReturn.ToState+2;

        for (int RequestIndex = 0; RequestIndex < Request.VariablesSize; RequestIndex++)
        {
            for (int DvoIndex = 0; DvoIndex < DvoSize; DvoIndex++)
            {
                for (int VariableIndex = 0; VariableIndex < *(Dvo[DvoIndex].VariablesSizeUP); VariableIndex++)
                {
                    if ((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Name && Request.VariableUPs[RequestIndex]->Name)
                    if (strcmp((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Name, Request.VariableUPs[RequestIndex]->Name) == 0)
                    {
                        V = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val;
                       (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val = (ValueObj){
                            .ValueType = 0};

                        ToReturn.VAVs.DefinedVariablesSize++;
                        ToReturn.VAVs.DefinedVariables = realloc( ToReturn.VAVs.DefinedVariables, ToReturn.VAVs.DefinedVariablesSize * sizeof(DefinedVariableObj));
                        ToReturn.VAVs.DefinedVariables[ToReturn.VAVs.DefinedVariablesSize-1] = (DefinedVariableObj){
                            .Value = V,
                            .TheDefinedVarUP = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]
                        };

                        break;
                    }
                }
                if (V.ValueType) break;
            }
            if (V.ValueType) break;
        }
    }


    // `=`

    if (set)
    {
        for(int CountedIndex = 0; CountedIndex < Counted.ValueSize; CountedIndex++)
        {
            Request.VariableUPs[CountedIndex]->Val = Counted.Value[CountedIndex];
        }

        ToReturn.ToState = ToReturn.ToState+4;

        VariableObj * Vars = malloc(0);
        for (int VariableIndex = 0; VariableIndex < Request.VariablesSize; VariableIndex++)
        {
            Vars = realloc(Vars, (VariableIndex + 1) * sizeof(VariableObj));
            Vars[VariableIndex] = *(Request.VariableUPs[VariableIndex]);
        }
        ToReturn.Vars = (VariablesObj){
            .VarsSize = Request.VariablesSize,
            .Vars = Vars
        };
    }


    return ToReturn;
}

//
// Created by calle on 25-1-27.
//

#include "../OBJECTS.h"
#include "../REQUEST.h"
#include "../COUNT.h"
#include "../ERR.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


HeadReturnObj FOR(struct _PairObject*Pairs,int PairsSize)
{
    HeadReturnObj ToReturn;
    ToReturn.ToState = 0;


    VariableRequestUPObj Request = {.VariablesSize = 0};
    CountObj SetCounted = {.ValueSize = 0};
    CountObj DoCounted = {.ValueSize = 0};
    CountObj ToCounted = {.ValueSize = 0};

    int Reverse = 1;

    int set=0,_do=0,to=0;

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {
            Request = REQUEST(Pairs[i].Source, Pairs[i].SourceSize);
        }
        if (Prompt.ObjType == 2)
        {
            switch (Prompt.Prompt.CurNumber)
            {
            case 1: // =
                set = 1;
                SetCounted = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                break;
            case 5: //do
                DoCounted = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
              	_do = 1;
                break;
            case 6: //to
              	ToCounted = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                to = 1;
                break;
            default:
                ErrCall("unsupported prompt type","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }
        }
    }

    if(set){
      	extern DefineVariableObj * Dvo;
        extern int DvoSize;

        ValueObj V = (ValueObj){.ValueType = 0};

        ToReturn.ToState = 2;

        for (int RequestIndex = 0; RequestIndex < Request.VariablesSize; RequestIndex++)
        {
            for (int DvoIndex = 0; DvoIndex < DvoSize; DvoIndex++)
            {
                for (int VariableIndex = 0; VariableIndex < *(Dvo[DvoIndex].VariablesSizeUP); VariableIndex++)
                {
                  printf("%d %d %d %d\n",DvoIndex,VariableIndex,*(Dvo->VariablesSizeUP),DvoSize);
                  printf("%s %s\n",(*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Name,Request.VariableUPs[RequestIndex]->Name);
                    if (strcmp((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Name, Request.VariableUPs[RequestIndex]->Name) == 0)
                    {
                        V = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val;
                       (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val = (ValueObj){
                            .ValueType = 0};

                        ToReturn.VAV = (DefinedVarsAndValueObj){
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


      	for(int CountedIndex = 0; CountedIndex < SetCounted.ValueSize; CountedIndex++)
        {
            Request.VariableUPs[CountedIndex]->Val = SetCounted.Value[CountedIndex];
        }
    }

    return ToReturn;
}


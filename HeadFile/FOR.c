//
// Created by calle on 25-1-27.
//

#include "../OBJECTS.h"
#include "../REQUEST.h"
#include "../COUNT.h"
#include "../ERR.h"
#include "../MIONE.h"
#include "../MTC.h"

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
    CountObj InCounted = {.ValueSize = 0};

    int Reverse = 1;

    int set=0,_do=0,to=0,in = 0,with = 0;

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {
            Request = REQUEST(Pairs[i].Source, Pairs[i].SourceSize);
            if (Request.VariablesSize !=1)ErrCall("`for` only accept ONE VARIABLE.","M007",NULL,Prompt.Line,Prompt.Column);

        }
        if (Prompt.ObjType == 2)
        {
            switch (Prompt.Prompt.CurNumber)
            {
            case 1: // =

                set = 1;
                SetCounted = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                if (SetCounted.ValueSize !=1)ErrCall("`=` PROMPT in `for` only accept ONE SOURCE.","M008",NULL,Prompt.Line,Prompt.Column);
                if (SetCounted.Value[0].ValueType!= VALUE_NOPOINTNUMBER_TYPE)ErrCall("After `=` PROMPT in `for` is`n a NPNumber.","M009",NULL,Prompt.Line,Prompt.Column);

                break;
            case 5: //do
                DoCounted = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                if (DoCounted.ValueSize !=1)ErrCall("`do` PROMPT in `for` only accept ONE SOURCE.","M010",NULL,Prompt.Line,Prompt.Column);
                if (DoCounted.Value[0].ValueType!= VALUE_RANGE_TYPE)ErrCall("After `do` PROMPT in `for` is`n a Range.","M011",NULL,Prompt.Line,Prompt.Column);
              	_do = 1;
                break;
            case 6: //to
              	ToCounted = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                if (ToCounted.ValueSize !=1)ErrCall("`to` PROMPT in `for` only accept ONE SOURCE.","M012",NULL,Prompt.Line,Prompt.Column);
                if (ToCounted.Value[0].ValueType!= VALUE_NOPOINTNUMBER_TYPE)ErrCall("After `to` PROMPT in `for` is`n a NPNumber.","M013",NULL,Prompt.Line,Prompt.Column);

                to = 1;
                break;
            case 8: //in
                InCounted = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                if (InCounted.ValueSize !=1)ErrCall("`in` PROMPT in `for` only accept ONE SOURCE.","M014",NULL,Prompt.Line,Prompt.Column);
                if (InCounted.Value[0].ValueType!= VALUE_TABLE_TYPE)ErrCall("After `in` PROMPT in `for` is`n a Table.","M015",NULL,Prompt.Line,Prompt.Column);

                 in = 1;
                break;
                 break;
            default:
                ErrCall("This Prompt is not supported by `for`","M016",NULL,Prompt.Line,Prompt.Column);
                break;
            }
        }
    }

    extern ThreadsObj Threads;

    ValueObj V = (ValueObj){.ValueType = 0};
    VariableObj * VariableUP = NULL;

    int toTimes = 0;

    if(in&&set)ErrCall("`in` and `=` isn't allowed at the same time in `for`","M017",NULL,Pairs[0].Prompt.Line,Pairs[0].Prompt.Column);
    if(!set&&!(with&&in))ErrCall("`with` only works in `table loops`","M018",NULL,Pairs[0].Prompt.Line,Pairs[0].Prompt.Column);
    if(to&&!set)ErrCall("only using`to` won`t make us know what shall we use the value to count for loop.","M019",NULL,Pairs[0].Prompt.Line,Pairs[0].Prompt.Column);

    if(set){
      	extern DefineVariableObj * Dvo;
        extern int DvoSize;

        for (int RequestIndex = 0; RequestIndex < Request.VariablesSize; RequestIndex++)
        {
            for (int DvoIndex = 0; DvoIndex < DvoSize; DvoIndex++)
            {
                for (int VariableIndex = 0; VariableIndex < *(Dvo[DvoIndex].VariablesSizeUP); VariableIndex++)
                {
                    if (strcmp((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Name, Request.VariableUPs[RequestIndex]->Name) == 0)
                    {
                        V = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val;
                       (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val = (ValueObj){
                            .ValueType = 0};
                       VariableUP = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex];

                        break;
                    }
                }
                if (V.ValueType) break;
            }
            if (V.ValueType) break;
        }


      	Request.VariableUPs[0]->Val = SetCounted.Value[0];
    }
    if(to&&set){
      toTimes = ToCounted.Value[0].NPNumber - SetCounted.Value[0].NPNumber;
    }

    if(_do){
        if(set){
            //todo toTimes
        }
        if(in){
            //todo
        }
    }

    VariableUP->Val = V;

    return ToReturn;
}


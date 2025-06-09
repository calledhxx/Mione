#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../OBJECTS.h"
#include "../COUNT.h"
#include "../ERR.h"
#include "../REQUEST.h"

HeadReturnObj SVV(HeadRequestObj * HeadRequestUP)
{
    HeadRequestObj HeadRequest = *HeadRequestUP;

    PairObj * Pairs = HeadRequest.Pairs;
    int PairsSize = HeadRequest.PairsSize;

    VariableObj ** VariablesUP = HeadRequest.VariablesUP;
    int * VariablesUPSizeUP = HeadRequest.VariablesUPSizeUP;

    HeadReturnObj Re = {0};
    Re.ToState = 0;
    Re.Vars.VarsSize = 0;

    Re.VAVs.DefinedVariablesSize = 0;
    Re.VAVs.DefinedVariables = malloc(0);


    CountObj CountedSuffixOfHead;
    VariableRequestUPObj RequestedSuffixOfHead;

    CountObj CountedSuffixOfSetPrompt = {0};
    CountObj CountedSuffixOfPointPrompt = {0};

    int registeredPrompts = 0;

    VariablesObj Vars;
    Vars.VarsSize = 0;
    Vars.Vars = malloc(0);

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (!i)
        {

            if (PairsSize == 1)
            {
                CountedSuffixOfHead = COUNT(Pairs[i].Source,Pairs[i].SourceSize);

                if (VariablesUP)
                {
                    for (int a = 0; a < CountedSuffixOfHead.ValueSize; a++)
                    {
                        Vars.VarsSize++;
                        Vars.Vars = realloc(Vars.Vars,Vars.VarsSize*sizeof(VariableObj));

                        Vars.Vars[Vars.VarsSize-1].Name = NULL;
                        Vars.Vars[Vars.VarsSize-1].Place = (*VariablesUPSizeUP)+1;
                        Vars.Vars[Vars.VarsSize-1].Val = CountedSuffixOfHead.Value[a];
                    }


                    Re.Vars = Vars;
                    Re.ToState |=4;
                }
            }
        }else
        {
            switch (Prompt.Prompt.CurNumber)
            {
            case 1:
                CountedSuffixOfSetPrompt = COUNT(Pairs[i].Source,Pairs[i].SourceSize);
                break;
            case 7:
                CountedSuffixOfPointPrompt = COUNT(Pairs[i].Source,Pairs[i].SourceSize);
                break;
            default:
                ErrCall("aaa unsupported prompt type","M111",NULL,Prompt.Line,Prompt.Column);
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


    for (int i = 0;max>i;i++)
    {
        const int cmp = 1<<i;

        if (!registeredPrompts) break;

        if (registeredPrompts & cmp)
        {
            switch (i+1)
            {
            case 1:
                {
                    RequestedSuffixOfHead = REQUEST(Pairs[0].Source,Pairs[0].SourceSize);
                    if (CountedSuffixOfSetPrompt.ValueSize>RequestedSuffixOfHead.VariablesSize) ErrCall("dkakakpdkapkdakd","kkkk",NULL,Pairs[1].Prompt.Line,Pairs[1].Prompt.Column);

                    for(int CountedIndex = 0; CountedIndex < CountedSuffixOfSetPrompt.ValueSize; CountedIndex++)
                    {
                        *RequestedSuffixOfHead.VariableUPs[CountedIndex] = (VariableObj){
                            .Name = RequestedSuffixOfHead.VariableUPs[CountedIndex]->Name,
                            .Val = CountedSuffixOfSetPrompt.Value[CountedIndex]
                        };
                    }

                    for (int VariableIndex = 0; VariableIndex < RequestedSuffixOfHead.VariablesSize; VariableIndex++)
                    {
                        Vars.VarsSize++;
                        Vars.Vars = realloc(Vars.Vars, Vars.VarsSize * sizeof(VariableObj));
                        Vars.Vars[Vars.VarsSize-1] = *(RequestedSuffixOfHead.VariableUPs[VariableIndex]);
                    }

                    Re.ToState |= 4;

                    break;
                }
            case 7:
                {
                    extern ScopeObj MainSVU;

                    CountObj PointNamesCounted = COUNT(Pairs[0].Source,Pairs[0].SourceSize);
                    if (CountedSuffixOfPointPrompt.ValueSize>PointNamesCounted.ValueSize) ErrCall("dkakakpdkapkdakd","iaijdaisod",NULL,Pairs[1].Source[0].Line,Pairs[1].Source[0].Column);


                    for (int PNCIndex = 0; PNCIndex < PointNamesCounted.ValueSize; PNCIndex++)
                    {
                        VariableObj Var;

                        Var.Val = CountedSuffixOfPointPrompt.Value[PNCIndex];

                        switch (PointNamesCounted.Value[PNCIndex].ValueType)
                        {
                        case 1://str
                            Var.Name = PointNamesCounted.Value[PNCIndex].String;
                            break;
                        case 2://npn
                            Var.Place = PointNamesCounted.Value[PNCIndex].NPNumber;
                            break;
                        default:
                            ErrCall("dkakakpdkapkdakd","Invalid value type for point name",NULL,Pairs[1].Source[0].Line,Pairs[1].Source[0].Column);
                        }

                        Vars.VarsSize++;
                        Vars.Vars = realloc(Vars.Vars, Vars.VarsSize * sizeof(VariableObj));
                        Vars.Vars[Vars.VarsSize-1] = Var;

                    }

                    Re.ToState |= 4;

                    break;
                }
            }
        }
    }

    if (Re.ToState & 4) Re.Vars = Vars;

    return Re;
}

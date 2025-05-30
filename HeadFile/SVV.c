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

    HeadReturnObj Re;
    Re.ToState = 0;
    Re.Vars.VarsSize = 0;

    Re.VAVs.DefinedVariablesSize = 0;
    Re.VAVs.DefinedVariables = malloc(0);


    CountObj Counted;
    VariableRequestUPObj Requested;

    CountObj SetCounted = {0};
    CountObj PointCounted = {0};

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
                Counted = COUNT(Pairs[i].Source,Pairs[i].SourceSize);

                if (VariablesUP)
                {
                    for (int a = 0; a < Counted.ValueSize; a++)
                    {
                        Vars.VarsSize++;
                        Vars.Vars = realloc(Vars.Vars,Vars.VarsSize*sizeof(VariableObj));

                        Vars.Vars[Vars.VarsSize-1].Name = NULL;
                        Vars.Vars[Vars.VarsSize-1].Place = (*VariablesUPSizeUP)+1;
                        Vars.Vars[Vars.VarsSize-1].Val = Counted.Value[a];
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
                SetCounted = COUNT(Pairs[i].Source,Pairs[i].SourceSize);
                break;
            case 7:
                PointCounted = COUNT(Pairs[i].Source,Pairs[i].SourceSize);
                break;
            default:
           printf("%d\n", Prompt.Prompt.CurNumber);
                ErrCall("aaa unsupported prompt type","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }

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
                    Requested = REQUEST(Pairs[0].Source,Pairs[0].SourceSize);
                    if (SetCounted.ValueSize>Requested.VariablesSize) ErrCall("dkakakpdkapkdakd","kkkk",NULL,Pairs[1].Prompt.Line,Pairs[1].Prompt.Column);

                    for(int CountedIndex = 0; CountedIndex < SetCounted.ValueSize; CountedIndex++)
                    {
                        *Requested.VariableUPs[CountedIndex] = (VariableObj){
                            .Name = Requested.VariableUPs[CountedIndex]->Name,
                            .Val = SetCounted.Value[CountedIndex]
                        };
                    }

                    for (int VariableIndex = 0; VariableIndex < Requested.VariablesSize; VariableIndex++)
                    {
                        Vars.VarsSize++;
                        Vars.Vars = realloc(Vars.Vars, Vars.VarsSize * sizeof(VariableObj));
                        Vars.Vars[Vars.VarsSize-1] = *(Requested.VariableUPs[VariableIndex]);
                    }

                    Re.ToState |= 4;

                    break;
                }
            case 7:
                {
                    extern ScopeObj MainSVU;


                    DefinedVariablesCaseObj DefinedVariables;
                    DefinedVariables.DefinedVariablesSize = 0;
                    DefinedVariables.DefinedVariables = malloc(0);

                    CountObj PointNamesCounted = COUNT(Pairs[0].Source,Pairs[0].SourceSize);
                    if (PointCounted.ValueSize>PointNamesCounted.ValueSize) ErrCall("dkakakpdkapkdakd","iaijdaisod",NULL,Pairs[1].Source[0].Line,Pairs[1].Source[0].Column);


                    for (int PNCIndex = 0; PNCIndex < PointNamesCounted.ValueSize; PNCIndex++)
                    {
                        VariableObj Var;

                        Var.Val = PointCounted.Value[PNCIndex];

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

                    for (int i = 0; i < DefinedVariables.DefinedVariablesSize; i++)
                    {
                        *DefinedVariables.DefinedVariables[i].TheDefinedVarUP = (VariableObj){
                            .Name = DefinedVariables.DefinedVariables[i].TheDefinedVarUP->Name,
                            .Place = DefinedVariables.DefinedVariables[i].TheDefinedVarUP->Place,
                            .Val = DefinedVariables.DefinedVariables[i].Value
                        };

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

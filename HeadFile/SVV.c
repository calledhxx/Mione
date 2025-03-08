#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../OBJECTS.h"
#include "../COUNT.h"
#include "../ERR.h"
#include "../REQUEST.h"

HeadReturnObj SVV(HeadRequestObj HeadRequest)
{
    PairObj * Pairs = HeadRequest.Pairs;
    int PairsSize = HeadRequest.PairsSize;

    VariableObj ** VariablesUP = HeadRequest.VariablesUP;
    int * VariablesUPSizeUP = HeadRequest.VariablesUPSizeUP;

    extern DefineVariableObj * Dvo;
    extern int DvoSize;

    HeadReturnObj Re;
    Re.ToState = 0;
    Re.Vars.VarsSize = 0;

    Re.VAVs.VAVsSize = 0;
    Re.VAVs.VAVs = malloc(0);


    CountObj Counted;
    VariableRequestUPObj Requested;

    CountObj SetCounted;
    CountObj PointCounted;


    int set = 0,point = 0;

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
                    VariablesObj Vars;
                    Vars.VarsSize = 0;
                    Vars.Vars = malloc(Vars.VarsSize);

                    for (int a = 0; a < Counted.ValueSize; a++)
                    {
                        Vars.VarsSize++;
                        Vars.Vars = realloc(Vars.Vars,Vars.VarsSize*sizeof(VariableObj));
                        Vars.Vars[Vars.VarsSize-1].Name = NULL;
                        Vars.Vars[Vars.VarsSize-1].Place = (*VariablesUPSizeUP)+1;
                        Vars.Vars[Vars.VarsSize-1].Val = Counted.Value[a];
                    }


                    Re.Vars = Vars;
                    Re.ToState = Re.ToState+4;
                }
                // else ErrCall("無意義的語句","M111",NULL,Prompt.Line,Prompt.Column);
            }
        }else
        {
            switch (Prompt.Prompt.CurNumber)
            {
            case 1:
                SetCounted = COUNT(Pairs[i].Source,Pairs[i].SourceSize);
                set = 1;
                break;
            case 7:
                PointCounted = COUNT(Pairs[i].Source,Pairs[i].SourceSize);
                point = 1;
                break;
            default:
                ErrCall("unsupported prompt type","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }

        }


        // for (int index = 0; index < Pairs[i].SourceSize; index++)
        // {
        //
        // }
    }


    VariablesObj Vars;
    Vars.VarsSize = 0;
    Vars.Vars = malloc(0);

    if (set)
    {
        Requested = REQUEST(Pairs[0].Source,Pairs[0].SourceSize);
        if (SetCounted.ValueSize>Requested.VariablesSize) ErrCall("dkakakpdkapkdakd","kkkk",NULL,Pairs[1].Prompt.Line,Pairs[1].Prompt.Column);

        for(int CountedIndex = 0; CountedIndex < SetCounted.ValueSize; CountedIndex++)
        {
            Requested.VariableUPs[CountedIndex]->Val = SetCounted.Value[CountedIndex];

        }

        for (int VariableIndex = 0; VariableIndex < Requested.VariablesSize; VariableIndex++)
        {
            Vars.VarsSize++;
            Vars.Vars = realloc(Vars.Vars, (VariableIndex + 1) * sizeof(VariableObj));
            Vars.Vars[VariableIndex] = *(Requested.VariableUPs[VariableIndex]);
        }
    }

    if (point)
    {


        DefinedVarsAndValueObj VAVs;
        VAVs.VAVsSize = 0;
        VAVs.VAVs = malloc(0);

        CountObj PointNamesCounted = COUNT(Pairs[0].Source,Pairs[0].SourceSize);
        if (PointCounted.ValueSize>PointNamesCounted.ValueSize) ErrCall("dkakakpdkapkdakd","iaijdaisod",NULL,Pairs[1].Source[0].Line,Pairs[1].Source[0].Column);

        VariableObj ** VarUPs = malloc(0);
        int VarUPsSize = 0;

        for (int PNCIndex = 0; PNCIndex < PointNamesCounted.ValueSize; PNCIndex++)
        {
            switch (PointNamesCounted.Value[PNCIndex].ValueType)
            {
            case 1://str

                for (int DvoIndex = 0; DvoIndex < DvoSize; DvoIndex++)
                {
                    for (int VariableIndex = 0; VariableIndex < *(Dvo[DvoIndex].VariablesSizeUP); VariableIndex++)
                    {
                        if (strcmp((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Name, PointNamesCounted.Value[PNCIndex].String) == 0)
                        {
                            VarUPsSize++;
                            VarUPs = realloc(VarUPs, (VarUPsSize) * sizeof(VariableObj*));
                            (VarUPs[VarUPsSize-1]) = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex];


                            VAVs.VAVsSize++;
                            VAVs.VAVs = realloc(VAVs.VAVs, (VAVs.VAVsSize) * sizeof(DefinedVarAndValueObj));
                            VAVs.VAVs[VAVs.VAVsSize-1].Value = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val;

                            (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val = PointCounted.Value[PNCIndex];

                            for (int VariableIndex = Vars.VarsSize; VariableIndex < VarUPsSize; VariableIndex++)
                            {
                                Vars.VarsSize++;
                                Vars.Vars = realloc(Vars.Vars, (VariableIndex + 1) * sizeof(VariableObj));
                                Vars.Vars[VariableIndex] = *((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]);
                                Vars.Vars[VariableIndex] = (VariableObj){
                                    .Val = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val,
                                    .Name = PointNamesCounted.Value[PNCIndex].String
                                };;
                            }

                            VAVs.VAVs[VAVs.VAVsSize-1].TheDefinedVarUP = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex];

                            DvoIndex = DvoIndex + 1;
                            break;
                        }
                    }
                }



                if (!VarUPsSize)
                {
                    VarUPsSize++;
                    VarUPs = realloc(VarUPs, (VarUPsSize) * sizeof(VariableObj*));
                    (VarUPs[VarUPsSize-1]) = malloc(sizeof(VariableObj));
                    *(VarUPs[VarUPsSize-1]) = (VariableObj){
                        .Name = PointNamesCounted.Value[PNCIndex].String,
                        .Val = PointCounted.Value[PNCIndex],
                    };


                    (*Dvo[DvoSize-1].VariablesSizeUP)++;
                    (*Dvo[DvoSize-1].VariableUPsUP) = realloc((*Dvo[DvoSize-1].VariableUPsUP), (*Dvo[DvoSize-1].VariablesSizeUP) * sizeof(VariableObj**));
                    (*Dvo[DvoSize-1].VariableUPsUP)[(*Dvo[DvoSize-1].VariablesSizeUP)-1] = (VarUPs[VarUPsSize-1]);

                    VAVs.VAVsSize++;
                    VAVs.VAVs = realloc(VAVs.VAVs, (VAVs.VAVsSize) * sizeof(DefinedVarAndValueObj));
                    VAVs.VAVs[VAVs.VAVsSize-1].TheDefinedVarUP = (VarUPs[VarUPsSize-1]);
                    VAVs.VAVs[VAVs.VAVsSize-1].Value = (ValueObj){.ValueType = 0};

                    for (int VariableIndex = Vars.VarsSize; VariableIndex < VarUPsSize; VariableIndex++)
                    {
                        Vars.VarsSize++;
                        Vars.Vars = realloc(Vars.Vars, (Vars.VarsSize) * sizeof(VariableObj));
                        Vars.Vars[VariableIndex] = *((*Dvo[DvoSize-1].VariableUPsUP)[*Dvo[DvoSize-1].VariablesSizeUP-1]);
                    }
                }



                break;
            case 2://npn
                for (int DvoIndex = 0; DvoIndex < DvoSize; DvoIndex++)
                {
                    for (int VariableIndex = 0; VariableIndex < *(Dvo[DvoIndex].VariablesSizeUP); VariableIndex++)
                    {
                        if ((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Place == PointNamesCounted.Value[PNCIndex].NPNumber && PointNamesCounted.Value[PNCIndex].NPNumber > 1)
                        {
                            VarUPsSize++;
                            VarUPs = realloc(VarUPs, (VarUPsSize) * sizeof(VariableObj*));
                            (VarUPs[VarUPsSize-1]) = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex];

                            VAVs.VAVsSize++;
                            VAVs.VAVs = realloc(VAVs.VAVs, (VAVs.VAVsSize) * sizeof(DefinedVarAndValueObj));
                            VAVs.VAVs[VAVs.VAVsSize-1].Value = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val;
                            printf("adijoso %p\n",(*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]);


                            (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val = PointCounted.Value[PNCIndex];

                            for (int VariableIndex = Vars.VarsSize; VariableIndex < VarUPsSize; VariableIndex++)
                            {
                                Vars.VarsSize++;
                                Vars.Vars = realloc(Vars.Vars, (VariableIndex + 1) * sizeof(VariableObj));
                                Vars.Vars[VariableIndex] = (VariableObj){
                                    .Val = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Val,
                                    .Place = PointNamesCounted.Value[PNCIndex].NPNumber
                                };
                            }

                            VAVs.VAVs[VAVs.VAVsSize-1].TheDefinedVarUP = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex];


                            DvoIndex = DvoIndex + 1;
                            break;
                        }
                    }
                }

                if (!VarUPsSize)
                {
                    if (PointNamesCounted.Value[PNCIndex].NPNumber<=0) ErrCall("aaa","aaaa",NULL,Pairs[0].Source[0].Line,Pairs[0].Source[0].Column);
                    VarUPsSize++;
                    VarUPs = realloc(VarUPs, (VarUPsSize) * sizeof(VariableObj*));
                    (VarUPs[VarUPsSize-1]) = malloc(sizeof(VariableObj));
                    *(VarUPs[VarUPsSize-1]) =(VariableObj){
                        .Place = PointNamesCounted.Value[PNCIndex].NPNumber,
                        .Val = PointCounted.Value[PNCIndex],
                    };

                    (*Dvo[DvoSize-1].VariablesSizeUP)++;
                    (*Dvo[DvoSize-1].VariableUPsUP) = realloc((*Dvo[DvoSize-1].VariableUPsUP), (*Dvo[DvoSize-1].VariablesSizeUP) * sizeof(VariableObj*));
                    (*Dvo[DvoSize-1].VariableUPsUP)[*Dvo[DvoSize-1].VariablesSizeUP-1] = (VarUPs[VarUPsSize-1]);

                    VAVs.VAVsSize++;
                    VAVs.VAVs = realloc(VAVs.VAVs, (VAVs.VAVsSize) * sizeof(DefinedVarAndValueObj));
                    VAVs.VAVs[VAVs.VAVsSize-1].TheDefinedVarUP = (VarUPs[VarUPsSize-1]);
                    VAVs.VAVs[VAVs.VAVsSize-1].Value = (ValueObj){.ValueType = 0};


                    for (int VariableIndex = Vars.VarsSize; VariableIndex < VarUPsSize; VariableIndex++)
                    {
                        Vars.VarsSize++;
                        Vars.Vars = realloc(Vars.Vars, (VariableIndex + 1) * sizeof(VariableObj));
                        Vars.Vars[VariableIndex] = *((*Dvo[DvoSize-1].VariableUPsUP)[*Dvo[DvoSize-1].VariablesSizeUP-1]);
                    }
                }
                break;
            default:
                ErrCall("dkakakpdkapkdakd","Invalid value type for point name",NULL,Pairs[0].Source[0].Line,Pairs[0].Source[0].Column);
            }
        }

        for (int i = 0; i < VAVs.VAVsSize; i++) VAVs.VAVs[i].TheDefinedVarUP-> Val = VAVs.VAVs[i].Value;
    }

    if (set || point)
    {
        Re.ToState = Re.ToState+4;
        Re.Vars = Vars;
    }

    return Re;
}

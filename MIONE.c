//
// Created by calle on 24-12-28.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OBJECTS.h"
#include "MIONE.h"

#include <inttypes.h>

#include "HeadFile/AllHeads.h"
#include "HeadFile/SVV.h"
#include "ERR.h"
#include "PROMPT_DEF.h"

int NormalSupportHeads[]={
    1,
    2,
    3,
    4
};

int RangeSupportHeads[]={
    1,
    2,
    3,
    4
};

int FunctionSupportHeads[]={
    1,
    2,
    3,
    4
};
int TableSupportHeads[]={
    1
};

MioneReturnObj Range(const MioneObj* Objs, const int ObjsSize)
{
    DefinedVarAndValueObj EndLoacl = (DefinedVarAndValueObj){
    .TheDefinedVarUP = 0};



    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = 0;
    PairObj *Pairs = malloc(0);
    int PairsSize = 0;

    MioneObj LastMio = (MioneObj) {
        .ObjType = 0
    };

    MioneReturnObj Return;
    Return.ToState = 0;

    int MioneReturnStats = 1;

    for (int index = 0; index < ObjsSize; index++)
    {

        MioneObj Mio = Objs[index];


       if ((Mio.ObjType == 3 && strcmp(Mio.Symbol.Name, ";") == 0) ||
            Mio.ObjType == 1 ||
        (LastMio.ObjType == Mio.ObjType && (Mio.ObjType == 3 ? !Mio.Symbol.CanConnect : 1) && Mio.ObjType != 2) ||
        (LastMio.ObjType == 4 &&  Mio.ObjType == 5) ||  (LastMio.ObjType == 5 &&  Mio.ObjType == 4)
            )
        {


            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;

                if (a.ToState >= 4)
                {
                    a.ToState = a.ToState-4;
                    //printf("todo MIONE.c MIONE FUC\n");
                    //exit(200);
                }
                if (a.ToState >= 2)
                {
                    a.ToState = a.ToState-2;
                    EndLoacl = a.VAV;
                }
                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    Return.ToState =  Return.ToState +1;

                    index=ObjsSize; //break break
                    continue;
                }

                if (Mio.ObjType == 3 && strcmp(Mio.Symbol.Name, ";") == 0)continue;
            }

        }

        if (Mio.ObjType == 1) // Head
        {
            for (int i = 0; 1; i++)
            {
                if (Heads[i].CurNumber == -1) break;
                if (strcmp(Mio.Head.Name, Heads[i].Name) == 0)
                {
                    int HasFound = 0;
                    for (int SPHIndex = 0; SPHIndex < sizeof(RangeSupportHeads) / sizeof(int); SPHIndex++) if (RangeSupportHeads[SPHIndex] == Mio.Head.CurNumber)
                    {
                        HasFound = 1;
                        break;
                    }
                    if (!HasFound) ErrCall("range support head not found", "MI1",NULL, Mio.Line, Mio.Column);

                    PairsSize++;
                    Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                    Pairs[PairsSize - 1].Prompt = Mio; // Type = 1

                    Pairs[PairsSize - 1].SourceSize = 0;
                    Pairs[PairsSize - 1].Source = malloc(0);

                    HeadFuc = Heads[i].Fuc;
                }
            }

        }

        if (Mio.ObjType == 2) // PROMPT
        {
            if (!HeadFuc) ErrCall(" head not foundadskakdpasdkas", "MI1",NULL, Mio.Line, Mio.Column);
            PairsSize++;
            Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);

            Pairs[PairsSize - 1].SourceSize = 0;
            Pairs[PairsSize - 1].Source = malloc(0);
            Pairs[PairsSize - 1].Prompt = Mio; // Type = 2


        }

        if (Mio.ObjType == 3 || Mio.ObjType == 4 || Mio.ObjType == 5) // SVV
        {
            if (HeadFuc == 0) { // 偽HeadFuc
                PairsSize++;
                Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                Pairs[PairsSize - 1].Prompt = Mio; // 自主

                Pairs[PairsSize - 1].SourceSize = 0;
                Pairs[PairsSize - 1].Source = malloc(0);


                HeadFuc = SVV;
            }

            (Pairs[PairsSize - 1].SourceSize)++;

            (Pairs[PairsSize - 1].Source) = realloc(Pairs[PairsSize - 1].Source, sizeof(MioneObj) * (Pairs[PairsSize - 1].SourceSize));
            Pairs[PairsSize - 1].Source[Pairs[PairsSize - 1].SourceSize - 1] = Mio;

        }

        if (ObjsSize - 1 == index)
        {
            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;


                if (a.ToState >= 4)
                {
                    a.ToState = a.ToState-4;
                    //printf("todo MIONE.c MIONE FUC\n");
                    //exit(200);
                }
                if (a.ToState >= 2)
                {
                    a.ToState = a.ToState-2;
                    EndLoacl = a.VAV;
                }
                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    Return.ToState =  Return.ToState +1;

                    index=ObjsSize; //break break
                    continue;
                }
            }
        }
        LastMio = Mio;
    }
    if (EndLoacl.TheDefinedVarUP != 0) EndLoacl.TheDefinedVarUP->Val = EndLoacl.Value;

    

    return Return;
}

MioneReturnObj Function(const MioneObj* Objs, const int ObjsSize,const ValueObj* Request, const int RequestSize)
{
    DefinedVarAndValueObj EndLoacl = (DefinedVarAndValueObj){
    .TheDefinedVarUP = 0};



    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = 0;
    PairObj *Pairs = malloc(0);
    int PairsSize = 0;

    MioneObj LastMio = (MioneObj) {
        .ObjType = 0
    };

    MioneReturnObj Return;
    Return.ToState = 0;

    int MioneReturnStats = 1;

    for (int index = 0; index < ObjsSize; index++)
    {

        MioneObj Mio = Objs[index];


       if ((Mio.ObjType == 3 && strcmp(Mio.Symbol.Name, ";") == 0) ||
            Mio.ObjType == 1 ||
        (LastMio.ObjType == Mio.ObjType && (Mio.ObjType == 3 ? !Mio.Symbol.CanConnect : 1) && Mio.ObjType != 2) ||
        (LastMio.ObjType == 4 &&  Mio.ObjType == 5) ||  (LastMio.ObjType == 5 &&  Mio.ObjType == 4)
            )
        {


            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;

                if (a.ToState >= 4)
                {
                    a.ToState = a.ToState-4;
                    //printf("todo MIONE.c MIONE FUC\n");
                    //exit(200);
                }
                if (a.ToState >= 2)
                {
                    a.ToState = a.ToState-2;
                    EndLoacl = a.VAV;
                }
                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    index=ObjsSize; //break break
                    continue;
                }

                if (Mio.ObjType == 3 && strcmp(Mio.Symbol.Name, ";") == 0)continue;
            }

        }

        if (Mio.ObjType == 1) // Head
        {
            for (int i = 0; 1; i++)
            {
                if (Heads[i].CurNumber == -1) break;
                if (strcmp(Mio.Head.Name, Heads[i].Name) == 0)
                {
                    int HasFound = 0;
                    for (int SPHIndex = 0; SPHIndex < sizeof(RangeSupportHeads) / sizeof(int); SPHIndex++) if (RangeSupportHeads[SPHIndex] == Mio.Head.CurNumber)
                    {
                        HasFound = 1;
                        break;
                    }
                    if (!HasFound) ErrCall("range support head not found", "MI1",NULL, Mio.Line, Mio.Column);

                    PairsSize++;
                    Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                    Pairs[PairsSize - 1].Prompt = Mio; // Type = 1

                    Pairs[PairsSize - 1].SourceSize = 0;
                    Pairs[PairsSize - 1].Source = malloc(0);

                    HeadFuc = Heads[i].Fuc;
                }
            }

        }

        if (Mio.ObjType == 2) // PROMPT
        {
            if (!HeadFuc) ErrCall(" head not foundadskakdpasdkas", "MI1",NULL, Mio.Line, Mio.Column);
            PairsSize++;
            Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);

            Pairs[PairsSize - 1].SourceSize = 0;
            Pairs[PairsSize - 1].Source = malloc(0);
            Pairs[PairsSize - 1].Prompt = Mio; // Type = 2


        }

        if (Mio.ObjType == 3 || Mio.ObjType == 4 || Mio.ObjType == 5) // SVV
        {
            if (HeadFuc == 0) { // 偽HeadFuc
                PairsSize++;
                Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                Pairs[PairsSize - 1].Prompt = Mio; // 自主

                Pairs[PairsSize - 1].SourceSize = 0;
                Pairs[PairsSize - 1].Source = malloc(0);


                HeadFuc = SVV;
            }

            (Pairs[PairsSize - 1].SourceSize)++;

            (Pairs[PairsSize - 1].Source) = realloc(Pairs[PairsSize - 1].Source, sizeof(MioneObj) * (Pairs[PairsSize - 1].SourceSize));
            Pairs[PairsSize - 1].Source[Pairs[PairsSize - 1].SourceSize - 1] = Mio;

        }

        if (ObjsSize - 1 == index)
        {
            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;


                if (a.ToState >= 4)
                {
                    a.ToState = a.ToState-4;
                    //printf("todo MIONE.c MIONE FUC\n");
                    //exit(200);
                }
                if (a.ToState >= 2)
                {
                    a.ToState = a.ToState-2;
                    EndLoacl = a.VAV;
                }
                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    Return.ToState =  Return.ToState +1;

                    index=ObjsSize; //break break
                    continue;
                }
            }
        }
        LastMio = Mio;
    }
    if (EndLoacl.TheDefinedVarUP != 0) EndLoacl.TheDefinedVarUP->Val = EndLoacl.Value;

    

    return Return;
}

MioneReturnObj mione(const MioneObj* Objs, const int ObjsSize)
{

    DefinedVarAndValueObj EndLoacl = (DefinedVarAndValueObj){
    .TheDefinedVarUP = 0};
    

    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = 0;
    PairObj *Pairs = malloc(0);
    int PairsSize = 0;

    MioneObj LastMio = (MioneObj) {
        .ObjType = 0
    };

    MioneReturnObj Return;
    Return.ToState = 0;

    int MioneReturnStats = 1;

    for (int index = 0; index < ObjsSize; index++)
    {

        MioneObj Mio = Objs[index];


       if ((Mio.ObjType == 3 && strcmp(Mio.Symbol.Name, ";") == 0) ||
            Mio.ObjType == 1 ||
        (LastMio.ObjType == Mio.ObjType && (Mio.ObjType == 3 ? !Mio.Symbol.CanConnect : 1) && Mio.ObjType != 2) ||
        (LastMio.ObjType == 4 &&  Mio.ObjType == 5) ||  (LastMio.ObjType == 5 &&  Mio.ObjType == 4)
            )
        {


            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;

                if (a.ToState >= 4)
                {
                    a.ToState = a.ToState-4;
                    //printf("todo MIONE.c MIONE FUC\n");
                    //exit(200);
                }
                if (a.ToState >= 2)
                {
                    a.ToState = a.ToState-2;
                    EndLoacl = a.VAV;
                }
                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    index=ObjsSize; //break break
                    continue;
                }

                if (Mio.ObjType == 3 && strcmp(Mio.Symbol.Name, ";") == 0)continue;
            }

        }

        if (Mio.ObjType == 1) // Head
        {
            for (int i = 0; 1; i++)
            {
                if (Heads[i].CurNumber == -1) break;
                if (strcmp(Mio.Head.Name, Heads[i].Name) == 0)
                {
                    int HasFound = 0;
                    for (int SPHIndex = 0; SPHIndex < sizeof(RangeSupportHeads) / sizeof(int); SPHIndex++) if (RangeSupportHeads[SPHIndex] == Mio.Head.CurNumber)
                    {
                        HasFound = 1;
                        break;
                    }
                    if (!HasFound) ErrCall("range support head not found", "MI1",NULL, Mio.Line, Mio.Column);

                    PairsSize++;
                    Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                    Pairs[PairsSize - 1].Prompt = Mio; // Type = 1

                    Pairs[PairsSize - 1].SourceSize = 0;
                    Pairs[PairsSize - 1].Source = malloc(0);

                    HeadFuc = Heads[i].Fuc;
                }
            }

        }

        if (Mio.ObjType == 2) // PROMPT
        {
            if (!HeadFuc) ErrCall(" head not foundadskakdpasdkas", "MI1",NULL, Mio.Line, Mio.Column);
            PairsSize++;
            Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);

            Pairs[PairsSize - 1].SourceSize = 0;
            Pairs[PairsSize - 1].Source = malloc(0);
            Pairs[PairsSize - 1].Prompt = Mio; // Type = 2


        }

        if (Mio.ObjType == 3 || Mio.ObjType == 4 || Mio.ObjType == 5) // SVV
        {
            if (HeadFuc == 0) { // 偽HeadFuc

                PairsSize++;
                Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                Pairs[PairsSize - 1].Prompt = Mio; // 自主

                Pairs[PairsSize - 1].SourceSize = 0;
                Pairs[PairsSize - 1].Source = malloc(0);


                HeadFuc = SVV;
            }

            (Pairs[PairsSize - 1].SourceSize)++;

            (Pairs[PairsSize - 1].Source) = realloc(Pairs[PairsSize - 1].Source, sizeof(MioneObj) * (Pairs[PairsSize - 1].SourceSize));
            Pairs[PairsSize - 1].Source[Pairs[PairsSize - 1].SourceSize - 1] = Mio;

        }

        if (ObjsSize - 1 == index)
        {
            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;


                if (a.ToState >= 4)
                {
                    a.ToState = a.ToState-4;
                    //printf("todo MIONE.c MIONE FUC\n");
                    //exit(200);
                }
                if (a.ToState >= 2)
                {
                    a.ToState = a.ToState-2;
                    EndLoacl = a.VAV;
                }
                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    Return.ToState =  Return.ToState +1;

                    index=ObjsSize; //break break
                    continue;
                }
            }
        }
        LastMio = Mio;
    }
    if (EndLoacl.TheDefinedVarUP != 0) EndLoacl.TheDefinedVarUP->Val = EndLoacl.Value;

    

    return Return;
}

MioneReturnObj Table(const MioneObj* Objs, const int ObjsSize,VariableObj * * VariablesUP, int * VariablesUPSizeUP/*不需要固定變數記憶位置*/)
{
    DefinedVarAndValueObj EndLoacl = (DefinedVarAndValueObj){
    .TheDefinedVarUP = 0};

    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = 0;
    PairObj *Pairs = malloc(0);
    int PairsSize = 0;

    MioneObj LastMio = (MioneObj) {
        .ObjType = 0
    };

    MioneReturnObj Return;
    Return.ToState = 0;

    int MioneReturnStats = 1;

    for (int index = 0; index < ObjsSize; index++)
    {

        MioneObj Mio = Objs[index];


       if ((Mio.ObjType == 3 && strcmp(Mio.Symbol.Name, ";") == 0) ||
            Mio.ObjType == 1 ||
            (LastMio.ObjType == Mio.ObjType && (Mio.ObjType == 3 ? !Mio.Symbol.CanConnect : 1) && Mio.ObjType != 2)||
            (LastMio.ObjType == 4 &&  Mio.ObjType == 5) ||  (LastMio.ObjType == 5 &&  Mio.ObjType == 4)
            )
        {


            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;

                if (a.ToState >= 4)
                {
                    a.ToState = a.ToState-4;
                    for (int i = 0; i<a.Vars.VarsSize; i++)
                    {
                        (*VariablesUPSizeUP)++;
                        (*VariablesUP) = realloc((*VariablesUP), (*VariablesUPSizeUP) * sizeof(VariableObj));
                        (*VariablesUP)[(*VariablesUPSizeUP) - 1] = a.Vars.Vars[i];
                    }
                }
                if (a.ToState >= 2)
                {
                    a.ToState = a.ToState-2;
                    EndLoacl = a.VAV;
                }

                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    Return.ToState =  Return.ToState +1;

                    index=ObjsSize; //break break
                    continue;
                }

                if (Mio.ObjType == 3 && strcmp(Mio.Symbol.Name, ";") == 0)continue;
            }

        }

        if (Mio.ObjType == 1) // Head
        {
            for (int i = 0; 1; i++)
            {
                if (Heads[i].CurNumber == -1) break;
                if (strcmp(Mio.Head.Name, Heads[i].Name) == 0)
                {
                    int HasFound = 0;
                    for (int SPHIndex = 0; SPHIndex < sizeof(TableSupportHeads) / sizeof(int); SPHIndex++) if (TableSupportHeads[SPHIndex] == Mio.Head.CurNumber)
                    {
                        HasFound = 1;
                        break;
                    }
                    if (!HasFound) ErrCall("mione support head not found", "MI1",NULL, Mio.Line, Mio.Column);

                    PairsSize++;
                    Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                    Pairs[PairsSize - 1].Prompt = Mio; // Type = 1

                    Pairs[PairsSize - 1].SourceSize = 0;
                    Pairs[PairsSize - 1].Source = malloc(0);

                    HeadFuc = Heads[i].Fuc;
                }
            }

        }

        if (Mio.ObjType == 2) // PROMPT
        {
            if (!HeadFuc) ErrCall(" head not foundadskakdpasdkas", "MI1",NULL, Mio.Line, Mio.Column);
            PairsSize++;
            Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);

            Pairs[PairsSize - 1].SourceSize = 0;
            Pairs[PairsSize - 1].Source = malloc(0);
            Pairs[PairsSize - 1].Prompt = Mio; // Type = 2


        }

        if (Mio.ObjType == 3 || Mio.ObjType == 4 || Mio.ObjType == 5) // SVV
        {
            if (HeadFuc == 0) { // 偽HeadFuc
                PairsSize++;
                Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                Pairs[PairsSize - 1].Prompt = Mio; // 自主

                Pairs[PairsSize - 1].SourceSize = 0;
                Pairs[PairsSize - 1].Source = malloc(0);


                HeadFuc = SVV;
            }

            (Pairs[PairsSize - 1].SourceSize)++;

            (Pairs[PairsSize - 1].Source) = realloc(Pairs[PairsSize - 1].Source, sizeof(MioneObj) * (Pairs[PairsSize - 1].SourceSize));
            Pairs[PairsSize - 1].Source[Pairs[PairsSize - 1].SourceSize - 1] = Mio;

        }

        if (ObjsSize - 1 == index)
        {
            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;


                if (a.ToState >= 4)
                {
                    a.ToState = a.ToState-4;
                    for (int i = 0; i<a.Vars.VarsSize; i++)
                    {
                        (*VariablesUPSizeUP)++;
                        (*VariablesUP) = realloc((*VariablesUP), (*VariablesUPSizeUP) * sizeof(VariableObj));
                        (*VariablesUP)[(*VariablesUPSizeUP) - 1] = a.Vars.Vars[i];
                    }
                }
                if (a.ToState >= 2)
                {
                    a.ToState = a.ToState-2;
                    EndLoacl = a.VAV;
                }
                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    Return.ToState =  Return.ToState +1;

                    index=ObjsSize; //break break
                    continue;
                }

            }
        }
        LastMio = Mio;
    }
    if (EndLoacl.TheDefinedVarUP != 0) EndLoacl.TheDefinedVarUP->Val = EndLoacl.Value;
    

    return Return;
}
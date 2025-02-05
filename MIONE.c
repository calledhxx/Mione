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

//TODO

MioneReturnObj Range(const MioneObj* Objs, const int ObjsSize,ThreadObj Thread)
{
    DefinedVarAndValueObj * EndLoacl = Thread.EndLoacl;
    int EndLoaclSize = Thread.EndLoaclSize;
    
    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = Thread.HeadFuc;
    PairObj *Pairs = Thread.Pairs;
    int PairsSize = Thread.PairsSize;

    MioneObj LastMio = Thread.LastMio;

    MioneReturnObj Return = Thread.Return;

    for (int index = 0; index < ObjsSize; index++)
    {

        MioneObj Mio = Objs[index];


       if ((Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0) ||
            Mio.ObjType == HEAD ||
        (LastMio.ObjType == Mio.ObjType && (Mio.ObjType == SYMBOL ? !Mio.Symbol.CanConnect : 1) && Mio.ObjType != PROMPT) ||
        (LastMio.ObjType == VARIABLE &&  Mio.ObjType == VALUE) ||  (LastMio.ObjType == VALUE &&  Mio.ObjType == VARIABLE)||
        (LastMio.ObjType == SYMBOL && LastMio.Symbol.AfterConnect == 0 && (Mio.ObjType == VARIABLE || Mio.ObjType == VALUE))
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
                   EndLoacl = realloc((EndLoacl), (EndLoaclSize+a.VAVs.VAVsSize)*sizeof(VariableObj));
                    for (int ELIndex = EndLoaclSize; ELIndex<EndLoaclSize+a.VAVs.VAVsSize; ELIndex++)
                    {
                        EndLoacl[ELIndex] = a.VAVs.VAVs[ELIndex-EndLoaclSize];
                    }
                    EndLoaclSize = EndLoaclSize+a.VAVs.VAVsSize;
                }
                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    Return.ToState =  Return.ToState +1;

                    index=ObjsSize; //break break
                    continue;
                }

                if (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0)continue;
            }

        }

        if (Mio.ObjType == HEAD) // Head
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
                    if (!HasFound) ErrCall("This HEAD isn't be supported by range.", "M003",NULL, Mio.Line, Mio.Column);

                    PairsSize++;
                    Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                    Pairs[PairsSize - 1].Prompt = Mio; // Type = 1

                    Pairs[PairsSize - 1].SourceSize = 0;
                    Pairs[PairsSize - 1].Source = malloc(0);

                    HeadFuc = Heads[i].Fuc;
                }
            }

        }

        if (Mio.ObjType == PROMPT) // PROMPT
        {
            if (!HeadFuc) ErrCall("PROMPT should be after HEAD.", "M002",NULL, Mio.Line, Mio.Column);
            PairsSize++;
            Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);

            Pairs[PairsSize - 1].SourceSize = 0;
            Pairs[PairsSize - 1].Source = malloc(0);
            Pairs[PairsSize - 1].Prompt = Mio; // Type = 2


        }

        if (Mio.ObjType == SYMBOL || Mio.ObjType == VARIABLE || Mio.ObjType == VALUE) // SVV
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
                   EndLoacl = realloc((EndLoacl), (EndLoaclSize+a.VAVs.VAVsSize)*sizeof(VariableObj));
                    for (int ELIndex = EndLoaclSize; ELIndex<EndLoaclSize+a.VAVs.VAVsSize; ELIndex++)
                    {
                        EndLoacl[ELIndex] = a.VAVs.VAVs[ELIndex-EndLoaclSize];
                    }
                    EndLoaclSize = EndLoaclSize+a.VAVs.VAVsSize;
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
    for (int i = 0; i<EndLoaclSize; i++) if (EndLoacl[i].TheDefinedVarUP != 0) EndLoacl[i].TheDefinedVarUP->Val = EndLoacl[i].Value;

    

    return Return;
}

MioneReturnObj Function(const MioneObj* Objs, const int ObjsSize, ValueObj* Request, int RequestSize,ThreadObj Thread)
{
    DefinedVarAndValueObj * EndLoacl = Thread.EndLoacl;
    int EndLoaclSize = Thread.EndLoaclSize;

    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = Thread.HeadFuc;
    PairObj *Pairs = Thread.Pairs;
    int PairsSize = Thread.PairsSize;

    MioneObj LastMio = Thread.LastMio;

    MioneReturnObj Return = Thread.Return;

    for (int index = 0; index < ObjsSize; index++)
    {

        MioneObj Mio = Objs[index];


       if ((Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0) ||
            Mio.ObjType == HEAD ||
        (LastMio.ObjType == Mio.ObjType && (Mio.ObjType == SYMBOL ? !Mio.Symbol.CanConnect : 1) && Mio.ObjType != PROMPT) ||
        (LastMio.ObjType == VARIABLE &&  Mio.ObjType == VALUE) ||  (LastMio.ObjType == VALUE &&  Mio.ObjType == VARIABLE)||
        (LastMio.ObjType == SYMBOL && LastMio.Symbol.AfterConnect == 0 && (Mio.ObjType == VARIABLE || Mio.ObjType == VALUE))
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
                   EndLoacl = realloc((EndLoacl), (EndLoaclSize+a.VAVs.VAVsSize)*sizeof(VariableObj));
                    for (int ELIndex = EndLoaclSize; ELIndex<EndLoaclSize+a.VAVs.VAVsSize; ELIndex++)
                    {
                        EndLoacl[ELIndex] = a.VAVs.VAVs[ELIndex-EndLoaclSize];
                    }
                    EndLoaclSize = EndLoaclSize+a.VAVs.VAVsSize;
                }
                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    index=ObjsSize; //break break
                    continue;
                }

                if (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0)continue;
            }

        }

        if (Mio.ObjType == HEAD) // Head
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
                    if (!HasFound) ErrCall("This HEAD isn't be supported by function.", "M004",NULL, Mio.Line, Mio.Column);

                    PairsSize++;
                    Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                    Pairs[PairsSize - 1].Prompt = Mio; // Type = 1

                    Pairs[PairsSize - 1].SourceSize = 0;
                    Pairs[PairsSize - 1].Source = malloc(0);

                    HeadFuc = Heads[i].Fuc;
                }
            }

        }

        if (Mio.ObjType == PROMPT) // PROMPT
        {
            if (!HeadFuc) ErrCall("PROMPT should be after HEAD.", "M002",NULL, Mio.Line, Mio.Column);

            PairsSize++;
            Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);

            Pairs[PairsSize - 1].SourceSize = 0;
            Pairs[PairsSize - 1].Source = malloc(0);
            Pairs[PairsSize - 1].Prompt = Mio; // Type = 2


        }

        if (Mio.ObjType == SYMBOL || Mio.ObjType == VARIABLE || Mio.ObjType == VALUE) // SVV
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
                   EndLoacl = realloc((EndLoacl), (EndLoaclSize+a.VAVs.VAVsSize)*sizeof(VariableObj));
                    for (int ELIndex = EndLoaclSize; ELIndex<EndLoaclSize+a.VAVs.VAVsSize; ELIndex++)
                    {
                        EndLoacl[ELIndex] = a.VAVs.VAVs[ELIndex-EndLoaclSize];
                    }
                    EndLoaclSize = EndLoaclSize+a.VAVs.VAVsSize;
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
    for (int i = 0; i<EndLoaclSize; i++) if (EndLoacl[i].TheDefinedVarUP != 0) EndLoacl[i].TheDefinedVarUP->Val = EndLoacl[i].Value;



    return Return;
}

MioneReturnObj mione(const MioneObj* Objs, const int ObjsSize,ThreadObj Thread)
{

    DefinedVarAndValueObj * EndLoacl = Thread.EndLoacl;
    int EndLoaclSize = Thread.EndLoaclSize;

    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = Thread.HeadFuc;
    PairObj *Pairs = Thread.Pairs;
    int PairsSize = Thread.PairsSize;

    MioneObj LastMio = Thread.LastMio;

    MioneReturnObj Return = Thread.Return;
    

    for (int index = 0; index < ObjsSize; index++)
    {

        MioneObj Mio = Objs[index];


       if ((Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0) ||
            Mio.ObjType == HEAD ||
        (LastMio.ObjType == Mio.ObjType && (Mio.ObjType == SYMBOL ? !Mio.Symbol.CanConnect : 1) && Mio.ObjType != PROMPT) ||
        (LastMio.ObjType == VARIABLE &&  Mio.ObjType == VALUE) ||  (LastMio.ObjType == VALUE &&  Mio.ObjType == VARIABLE)||
        (LastMio.ObjType == SYMBOL && LastMio.Symbol.AfterConnect == 0 && (Mio.ObjType == VARIABLE || Mio.ObjType == VALUE))
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
                   EndLoacl = realloc((EndLoacl), (EndLoaclSize+a.VAVs.VAVsSize)*sizeof(VariableObj));
                    for (int ELIndex = EndLoaclSize; ELIndex<EndLoaclSize+a.VAVs.VAVsSize; ELIndex++)
                    {
                        EndLoacl[ELIndex] = a.VAVs.VAVs[ELIndex-EndLoaclSize];
                    }
                    EndLoaclSize = EndLoaclSize+a.VAVs.VAVsSize;
                }
                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    index=ObjsSize; //break break
                    continue;
                }

                if (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0)continue;
            }

        }

        if (Mio.ObjType == HEAD) // Head
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
                    if (!HasFound) ErrCall("This HEAD isn't be supported by main Mione.", "M005",NULL, Mio.Line, Mio.Column);

                    PairsSize++;
                    Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                    Pairs[PairsSize - 1].Prompt = Mio; // Type = 1

                    Pairs[PairsSize - 1].SourceSize = 0;
                    Pairs[PairsSize - 1].Source = malloc(0);

                    HeadFuc = Heads[i].Fuc;
                }
            }

        }

        if (Mio.ObjType == PROMPT) // PROMPT
        {
            if (!HeadFuc) ErrCall("PROMPT should be after HEAD.", "M002",NULL, Mio.Line, Mio.Column);

            PairsSize++;
            Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);

            Pairs[PairsSize - 1].SourceSize = 0;
            Pairs[PairsSize - 1].Source = malloc(0);
            Pairs[PairsSize - 1].Prompt = Mio; // Type = 2


        }

        if (Mio.ObjType == SYMBOL || Mio.ObjType == VARIABLE || Mio.ObjType == VALUE) // SVV
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

        if (
            (ObjsSize - 1 == index)
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
                   EndLoacl = realloc((EndLoacl), (EndLoaclSize+a.VAVs.VAVsSize)*sizeof(VariableObj));
                    for (int ELIndex = EndLoaclSize; ELIndex<EndLoaclSize+a.VAVs.VAVsSize; ELIndex++)
                    {
                        EndLoacl[ELIndex] = a.VAVs.VAVs[ELIndex-EndLoaclSize];
                    }
                    EndLoaclSize = EndLoaclSize+a.VAVs.VAVsSize;
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
    for (int i = 0; i<EndLoaclSize; i++) if (EndLoacl[i].TheDefinedVarUP != 0) EndLoacl[i].TheDefinedVarUP->Val = EndLoacl[i].Value;



    return Return;
}

MioneReturnObj Table(const MioneObj* Objs, const int ObjsSize,VariableObj * * VariablesUP, int * VariablesUPSizeUP/*不需要固定變數記憶位置*/,ThreadObj Thread)
{
    DefinedVarAndValueObj * EndLoacl = Thread.EndLoacl;
    int EndLoaclSize = Thread.EndLoaclSize;

    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = Thread.HeadFuc;
    PairObj *Pairs = Thread.Pairs;
    int PairsSize = Thread.PairsSize;

    MioneObj LastMio = Thread.LastMio;

    MioneReturnObj Return = Thread.Return;


    for (int index = 0; index < ObjsSize; index++)
    {

        MioneObj Mio = Objs[index];


        if ((Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0) ||
             Mio.ObjType == HEAD ||
         (LastMio.ObjType == Mio.ObjType && (Mio.ObjType == SYMBOL ? !Mio.Symbol.CanConnect : 1) && Mio.ObjType != PROMPT) ||
         (LastMio.ObjType == VARIABLE &&  Mio.ObjType == VALUE) ||  (LastMio.ObjType == VALUE &&  Mio.ObjType == VARIABLE)||
        (LastMio.ObjType == SYMBOL && LastMio.Symbol.AfterConnect == 0 && (Mio.ObjType == VARIABLE || Mio.ObjType == VALUE))
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
                    EndLoacl = realloc((EndLoacl), (EndLoaclSize+a.VAVs.VAVsSize)*sizeof(VariableObj));
                    for (int ELIndex = EndLoaclSize; ELIndex<EndLoaclSize+a.VAVs.VAVsSize; ELIndex++)
                    {
                        EndLoacl[ELIndex] = a.VAVs.VAVs[ELIndex-EndLoaclSize];
                    }
                    EndLoaclSize = EndLoaclSize+a.VAVs.VAVsSize;

                }

                if (a.ToState >= 1)
                {
                    a.ToState = a.ToState-1;
                    Return.Vs = a.Vs;

                    Return.ToState =  Return.ToState +1;

                    index=ObjsSize; //break break
                    continue;
                }

                if (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0)continue;
            }

        }

        if (Mio.ObjType == HEAD) // Head
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
                    if (!HasFound) ErrCall("This HEAD isn't be supported by table.", "M006",NULL, Mio.Line, Mio.Column);

                    PairsSize++;
                    Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);
                    Pairs[PairsSize - 1].Prompt = Mio; // Type = 1

                    Pairs[PairsSize - 1].SourceSize = 0;
                    Pairs[PairsSize - 1].Source = malloc(0);

                    HeadFuc = Heads[i].Fuc;
                }
            }

        }

        if (Mio.ObjType == PROMPT) // PROMPT
        {
            if (!HeadFuc) ErrCall("PROMPT should be after HEAD.", "M002",NULL, Mio.Line, Mio.Column);
            PairsSize++;
            Pairs = realloc(Pairs, sizeof(struct _PairObject) * PairsSize);

            Pairs[PairsSize - 1].SourceSize = 0;
            Pairs[PairsSize - 1].Source = malloc(0);
            Pairs[PairsSize - 1].Prompt = Mio; // Type = 2


        }

        if (Mio.ObjType == SYMBOL || Mio.ObjType == VARIABLE || Mio.ObjType == VALUE) // SVV
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
                   EndLoacl = realloc((EndLoacl), (EndLoaclSize+a.VAVs.VAVsSize)*sizeof(VariableObj));
                    for (int ELIndex = EndLoaclSize; ELIndex<EndLoaclSize+a.VAVs.VAVsSize; ELIndex++)
                    {
                        EndLoacl[ELIndex] = a.VAVs.VAVs[ELIndex-EndLoaclSize];
                    }
                    EndLoaclSize = EndLoaclSize+a.VAVs.VAVsSize;


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

    for (int i = 0; i<EndLoaclSize; i++) if (EndLoacl[i].TheDefinedVarUP != 0) EndLoacl[i].TheDefinedVarUP->Val = EndLoacl[i].Value;

    return Return;
}
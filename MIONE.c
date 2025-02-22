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
    DefinedVarAndValueObj * * EndLoaclUP = Thread.EndLoaclUP;
    int * EndLoaclSizeUP = Thread.EndLoaclSizeUP;
    
    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = Thread.HeadFuc;
    PairObj *Pairs = malloc(0);
    int PairsSize = 0;

    

    MioneReturnObj Return = Thread.Return;


    int index;

    for (;*Thread.IndexUP < ObjsSize; (*Thread.IndexUP)++)
    {
        index = *Thread.IndexUP;

        MioneObj Mio = Objs[index];


       if (
            (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0)
            )
        {
            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;

                int States[] =  {
                    4,2,1
                };
                
                for (int StateIndex = 0; StateIndex<(sizeof(States)/sizeof(int)); StateIndex++)
                {
                    if (a.ToState >= States[StateIndex])
                    {
                        a.ToState = a.ToState-States[StateIndex];

                        switch (States[StateIndex])
                        {
                        case 4:
                            break;
                        case 2:
                            (*EndLoaclUP) = realloc(((*EndLoaclUP)), ((*EndLoaclSizeUP)+a.VAVs.VAVsSize)*sizeof(VariableObj));
                            for (int ELIndex = (*EndLoaclSizeUP); ELIndex<(*EndLoaclSizeUP)+a.VAVs.VAVsSize; ELIndex++)
                            {
                                (*EndLoaclUP)[ELIndex] = a.VAVs.VAVs[ELIndex-(*EndLoaclSizeUP)];
                            }
                            (*EndLoaclSizeUP) = (*EndLoaclSizeUP)+a.VAVs.VAVsSize;
                            break;
                        case 1:
                            Return.Vs = a.Vs;

                            Return.ToState =  Return.ToState +1;

                            (*Thread.IndexUP)=ObjsSize; //break break
                            continue;
                        }
                    }
                }



                if (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0) continue;

                return Return;
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

         if (
            ObjsSize - 1 == index ||
            ((index != ObjsSize - 1) &&
                (Objs[index+1].ObjType == HEAD ||
                (Mio.ObjType == Objs[index+1].ObjType && (Objs[index+1].ObjType == SYMBOL ? !Objs[index+1].Symbol.CanConnect : 1)) ||
                (Mio.ObjType == VARIABLE &&  Objs[index+1].ObjType == VALUE) ||  (Mio.ObjType == VALUE &&  Objs[index+1].ObjType == VARIABLE)||
                (Mio.ObjType == SYMBOL && Mio.Symbol.AfterConnect == 0 && (Objs[index+1].ObjType == VARIABLE || Objs[index+1].ObjType == VALUE))
                    )
                )
            )
        {
            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;


                int States[] =  {
                    4,2,1
                };
                
                for (int StateIndex = 0; StateIndex<(sizeof(States)/sizeof(int)); StateIndex++)
                {
                    if (a.ToState >= States[StateIndex])
                    {
                        a.ToState = a.ToState-States[StateIndex];

                        switch (States[StateIndex])
                        {
                        case 4:
                            break;
                        case 2:
                            (*EndLoaclUP) = realloc(((*EndLoaclUP)), ((*EndLoaclSizeUP)+a.VAVs.VAVsSize)*sizeof(VariableObj));
                            for (int ELIndex = (*EndLoaclSizeUP); ELIndex<(*EndLoaclSizeUP)+a.VAVs.VAVsSize; ELIndex++)
                            {
                                (*EndLoaclUP)[ELIndex] = a.VAVs.VAVs[ELIndex-(*EndLoaclSizeUP)];
                            }
                            (*EndLoaclSizeUP) = (*EndLoaclSizeUP)+a.VAVs.VAVsSize;
                            break;
                        case 1:
                            Return.Vs = a.Vs;

                            Return.ToState =  Return.ToState +1;

                            (*Thread.IndexUP)=ObjsSize; //break break
                            continue;
                        }
                    }
                }


            }
        }
        
    }
    for (int i = 0; i<(*EndLoaclSizeUP); i++) if ((*EndLoaclUP)[i].TheDefinedVarUP != 0) (*EndLoaclUP)[i].TheDefinedVarUP->Val = (*EndLoaclUP)[i].Value;

    

    return Return;
}

MioneReturnObj Function(const MioneObj* Objs, const int ObjsSize, ValueObj* Request, int RequestSize,ThreadObj Thread)
{
     DefinedVarAndValueObj * * EndLoaclUP = Thread.EndLoaclUP;
    int * EndLoaclSizeUP = Thread.EndLoaclSizeUP;

    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = Thread.HeadFuc;
    PairObj *Pairs = malloc(0);
    int PairsSize = 0;

    

    MioneReturnObj Return = Thread.Return;


   int index;
    for (;*Thread.IndexUP < ObjsSize; (*Thread.IndexUP)++)
    {
        index = *Thread.IndexUP;

        MioneObj Mio = Objs[index];


       if (
            (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0)
            )
        {


            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;

                int States[] =  {
                    4,2,1
                };
                
                for (int StateIndex = 0; StateIndex<(sizeof(States)/sizeof(int)); StateIndex++)
                {
                    if (a.ToState >= States[StateIndex])
                    {
                        a.ToState = a.ToState-States[StateIndex];

                        switch (States[StateIndex])
                        {
                        case 4:
                            break;
                        case 2:
                            (*EndLoaclUP) = realloc(((*EndLoaclUP)), ((*EndLoaclSizeUP)+a.VAVs.VAVsSize)*sizeof(VariableObj));
                            for (int ELIndex = (*EndLoaclSizeUP); ELIndex<(*EndLoaclSizeUP)+a.VAVs.VAVsSize; ELIndex++)
                            {
                                (*EndLoaclUP)[ELIndex] = a.VAVs.VAVs[ELIndex-(*EndLoaclSizeUP)];
                            }
                            (*EndLoaclSizeUP) = (*EndLoaclSizeUP)+a.VAVs.VAVsSize;
                            break;
                        case 1:
                            Return.Vs = a.Vs;

                            Return.ToState =  Return.ToState +1;

                            (*Thread.IndexUP)=ObjsSize; //break break
                            continue;
                        }
                    }
                }
                if (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0) continue;

                return Return;
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

         if (
            ObjsSize - 1 == index ||
            ((index != ObjsSize - 1) &&
                (Objs[index+1].ObjType == HEAD ||
                (Mio.ObjType == Objs[index+1].ObjType && (Objs[index+1].ObjType == SYMBOL ? !Objs[index+1].Symbol.CanConnect : 1)) ||
                (Mio.ObjType == VARIABLE &&  Objs[index+1].ObjType == VALUE) ||  (Mio.ObjType == VALUE &&  Objs[index+1].ObjType == VARIABLE)||
                (Mio.ObjType == SYMBOL && Mio.Symbol.AfterConnect == 0 && (Objs[index+1].ObjType == VARIABLE || Objs[index+1].ObjType == VALUE))
                    )
                )
            )
        {
            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;


                int States[] =  {
                    4,2,1
                };


                
                for (int StateIndex = 0; StateIndex<(sizeof(States)/sizeof(int)); StateIndex++)
                {
                    if (a.ToState >= States[StateIndex])
                    {
                        a.ToState = a.ToState-States[StateIndex];


                        switch (States[StateIndex])
                        {
                        case 4:
                            break;
                        case 2:
                            (*EndLoaclUP) = realloc(((*EndLoaclUP)), ((*EndLoaclSizeUP)+a.VAVs.VAVsSize)*sizeof(VariableObj));
                            for (int ELIndex = (*EndLoaclSizeUP); ELIndex<(*EndLoaclSizeUP)+a.VAVs.VAVsSize; ELIndex++)
                            {
                                (*EndLoaclUP)[ELIndex] = a.VAVs.VAVs[ELIndex-(*EndLoaclSizeUP)];
                            }
                            (*EndLoaclSizeUP) = (*EndLoaclSizeUP)+a.VAVs.VAVsSize;
                            break;
                        case 1:
                            Return.Vs = a.Vs;

                            Return.ToState =  Return.ToState +1;

                            (*Thread.IndexUP)=ObjsSize; //break break
                            continue;
                        }
                    }
                }
            }
        }
        
    }
    for (int i = 0; i<(*EndLoaclSizeUP); i++) if ((*EndLoaclUP)[i].TheDefinedVarUP != 0) (*EndLoaclUP)[i].TheDefinedVarUP->Val = (*EndLoaclUP)[i].Value;

    return Return;
}

MioneReturnObj mione(const MioneObj* Objs, const int ObjsSize,ThreadObj Thread)
{
     DefinedVarAndValueObj * * EndLoaclUP = Thread.EndLoaclUP;
    int * EndLoaclSizeUP = Thread.EndLoaclSizeUP;

    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = Thread.HeadFuc;
    PairObj *Pairs = malloc(0);
    int PairsSize = 0;

    

    MioneReturnObj Return = Thread.Return;

    int index;
    
    for (;*Thread.IndexUP < ObjsSize; (*Thread.IndexUP)++)
    {

        index = *Thread.IndexUP;

        MioneObj Mio = Objs[index];

       if (
            (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0)
            )
        {
            if (HeadFuc != 0) {

                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;

                int States[] =  {
                    4,2,1
                };
                
                for (int StateIndex = 0; StateIndex<(sizeof(States)/sizeof(int)); StateIndex++)
                {
                    if (a.ToState >= States[StateIndex])
                    {
                        a.ToState = a.ToState-States[StateIndex];

                        switch (States[StateIndex])
                        {
                        case 4:
                            break;
                        case 2:
                            (*EndLoaclUP) = realloc(((*EndLoaclUP)), ((*EndLoaclSizeUP)+a.VAVs.VAVsSize)*sizeof(VariableObj));
                            for (int ELIndex = (*EndLoaclSizeUP); ELIndex<(*EndLoaclSizeUP)+a.VAVs.VAVsSize; ELIndex++)
                            {
                                (*EndLoaclUP)[ELIndex] = a.VAVs.VAVs[ELIndex-(*EndLoaclSizeUP)];
                            }
                            (*EndLoaclSizeUP) = (*EndLoaclSizeUP)+a.VAVs.VAVsSize;
                            break;
                        case 1:
                            Return.Vs = a.Vs;

                            Return.ToState =  Return.ToState +1;

                            (*Thread.IndexUP)=ObjsSize; //break break
                            continue;
                        }
                    }
                }

                if (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0) continue;

                
                return Return;
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

        if (
            ObjsSize - 1 == index ||
            ((index != ObjsSize - 1) &&
                (Objs[index+1].ObjType == HEAD ||
                (Mio.ObjType == Objs[index+1].ObjType && (Objs[index+1].ObjType == SYMBOL ? !Objs[index+1].Symbol.CanConnect : 1)) ||
                (Mio.ObjType == VARIABLE &&  Objs[index+1].ObjType == VALUE) ||  (Mio.ObjType == VALUE &&  Objs[index+1].ObjType == VARIABLE)||
                (Mio.ObjType == SYMBOL && Mio.Symbol.AfterConnect == 0 && (Objs[index+1].ObjType == VARIABLE || Objs[index+1].ObjType == VALUE))
                    )
                )
            )
        {

            if (HeadFuc != 0) {

                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;

                int States[] =  {
                    4,2,1
                };
                
                for (int StateIndex = 0; StateIndex<(sizeof(States)/sizeof(int)); StateIndex++)
                {
                    if (a.ToState >= States[StateIndex])
                    {
                        a.ToState = a.ToState-States[StateIndex];

                        switch (States[StateIndex])
                        {
                        case 4:
                            break;
                        case 2:
                            (*EndLoaclUP) = realloc(((*EndLoaclUP)), ((*EndLoaclSizeUP)+a.VAVs.VAVsSize)*sizeof(VariableObj));
                            for (int ELIndex = (*EndLoaclSizeUP); ELIndex<(*EndLoaclSizeUP)+a.VAVs.VAVsSize; ELIndex++)
                            {
                                (*EndLoaclUP)[ELIndex] = a.VAVs.VAVs[ELIndex-(*EndLoaclSizeUP)];
                            }
                            (*EndLoaclSizeUP) = (*EndLoaclSizeUP)+a.VAVs.VAVsSize;
                            break;
                        case 1:
                            Return.Vs = a.Vs;

                            Return.ToState =  Return.ToState +1;

                            (*Thread.IndexUP)=ObjsSize; //break break
                            continue;
                        }
                    }
                }

                (*Thread.IndexUP)++;
                
                return Return;
            }
        }
        
    }
    for (int i = 0; i<(*EndLoaclSizeUP); i++) if ((*EndLoaclUP)[i].TheDefinedVarUP != 0) (*EndLoaclUP)[i].TheDefinedVarUP->Val = (*EndLoaclUP)[i].Value;

    return Return;
}

MioneReturnObj Table(const MioneObj* Objs, const int ObjsSize,VariableObj * * VariablesUP, int * VariablesUPSizeUP/*不需要固定變數記憶位置*/,ThreadObj Thread)
{
     DefinedVarAndValueObj * * EndLoaclUP = Thread.EndLoaclUP;
    int * EndLoaclSizeUP = Thread.EndLoaclSizeUP;

    HeadReturnObj (*HeadFuc)(struct _PairObject* Pairs, int PairsSize) = Thread.HeadFuc;
    PairObj *Pairs = malloc(0);
    int PairsSize = 0;

    

    MioneReturnObj Return = Thread.Return;


   int index;
    for (;*Thread.IndexUP < ObjsSize; (*Thread.IndexUP)++)
    {
        index = *Thread.IndexUP;

        MioneObj Mio = Objs[index];


       if (
            (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0)
            )
        {
            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;

                int States[] =  {
                    4,2,1
                };
                
                for (int StateIndex = 0; StateIndex<(sizeof(States)/sizeof(int)); StateIndex++)
                {
                    if (a.ToState >= States[StateIndex])
                    {
                        a.ToState = a.ToState-States[StateIndex];

                        switch (States[StateIndex])
                        {
                        case 4:
                            for (int i = 0; i<a.Vars.VarsSize; i++)
                            {
                                (*VariablesUPSizeUP)++;
                                (*VariablesUP) = realloc((*VariablesUP), (*VariablesUPSizeUP) * sizeof(VariableObj));
                                (*VariablesUP)[(*VariablesUPSizeUP) - 1] = a.Vars.Vars[i];
                            }
                            break;
                        case 2:
                            (*EndLoaclUP) = realloc(((*EndLoaclUP)), ((*EndLoaclSizeUP)+a.VAVs.VAVsSize)*sizeof(VariableObj));
                            for (int ELIndex = (*EndLoaclSizeUP); ELIndex<(*EndLoaclSizeUP)+a.VAVs.VAVsSize; ELIndex++)
                            {
                                (*EndLoaclUP)[ELIndex] = a.VAVs.VAVs[ELIndex-(*EndLoaclSizeUP)];
                            }
                            (*EndLoaclSizeUP) = (*EndLoaclSizeUP)+a.VAVs.VAVsSize;
                            break;
                        case 1:
                            Return.Vs = a.Vs;

                            Return.ToState =  Return.ToState +1;

                            (*Thread.IndexUP)=ObjsSize; //break break
                            continue;
                        }
                    }
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

         if (
            ObjsSize - 1 == index ||
            ((index != ObjsSize - 1) &&
                (Objs[index+1].ObjType == HEAD ||
                (Mio.ObjType == Objs[index+1].ObjType && (Objs[index+1].ObjType == SYMBOL ? !Objs[index+1].Symbol.CanConnect : 1)) ||
                (Mio.ObjType == VARIABLE &&  Objs[index+1].ObjType == VALUE) ||  (Mio.ObjType == VALUE &&  Objs[index+1].ObjType == VARIABLE)||
                (Mio.ObjType == SYMBOL && Mio.Symbol.AfterConnect == 0 && (Objs[index+1].ObjType == VARIABLE || Objs[index+1].ObjType == VALUE))
                    )
                )
            )
        {
            if (HeadFuc != 0) {
                HeadReturnObj a = HeadFuc(Pairs, PairsSize);

                HeadFuc = 0;
                Pairs = NULL;
                PairsSize = 0;

                int States[] =  {
                    4,2,1
                };
                
                for (int StateIndex = 0; StateIndex<(sizeof(States)/sizeof(int)); StateIndex++)
                {
                    if (a.ToState >= States[StateIndex])
                    {
                        a.ToState = a.ToState-States[StateIndex];

                        switch (States[StateIndex])
                        {
                        case 4:
                            for (int i = 0; i<a.Vars.VarsSize; i++)
                            {
                                (*VariablesUPSizeUP)++;
                                (*VariablesUP) = realloc((*VariablesUP), (*VariablesUPSizeUP) * sizeof(VariableObj));
                                (*VariablesUP)[(*VariablesUPSizeUP) - 1] = a.Vars.Vars[i];
                            }
                            break;
                        case 2:
                            (*EndLoaclUP) = realloc(((*EndLoaclUP)), ((*EndLoaclSizeUP)+a.VAVs.VAVsSize)*sizeof(VariableObj));
                            for (int ELIndex = (*EndLoaclSizeUP); ELIndex<(*EndLoaclSizeUP)+a.VAVs.VAVsSize; ELIndex++)
                            {
                                (*EndLoaclUP)[ELIndex] = a.VAVs.VAVs[ELIndex-(*EndLoaclSizeUP)];
                            }
                            (*EndLoaclSizeUP) = (*EndLoaclSizeUP)+a.VAVs.VAVsSize;
                            break;
                        case 1:
                            Return.Vs = a.Vs;

                            Return.ToState =  Return.ToState +1;

                            (*Thread.IndexUP)=ObjsSize; //break break
                            continue;
                        }
                    }
                }
                

            }
        }
        
    }
    for (int i = 0; i<(*EndLoaclSizeUP); i++) if ((*EndLoaclUP)[i].TheDefinedVarUP != 0) (*EndLoaclUP)[i].TheDefinedVarUP->Val = (*EndLoaclUP)[i].Value;



    return Return;
}
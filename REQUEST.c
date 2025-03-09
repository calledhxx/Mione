//
// Created by calle on 25-1-3.
//

#include "OBJECTS.h"
#include "ERR.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "COUNT.h"
#include "SYMBOL_DEF.h"
#include "MIONE.h"


VariableRequestUPObj REQUEST(MioneObj*Pack,int PackSize)
{

    int FirstBracketIndex = 0;
    int BracketsChild = 0; //括號多寡

    int BracketCur = 0;//1 : [, 2 : (

    MioneObj* inBracket = malloc(0);
    int inBracketSize = 0;

    int CalculateType = 0;
    int CalculateLevel = 0;


    for(int i = 0; i < PackSize; i++)
    {
        int PastCost = 0;//符號所前扣之值

        switch (Pack[i].ObjType)
        {
        case SYMBOL:
            {
                switch (Pack[i].Symbol.CurNumber)
            {
            case 10:
                {
                    if (!BracketsChild)
                    {
                        BracketCur = 1;
                        FirstBracketIndex = i;

                        inBracketSize = 0;
                        free(inBracket);
                        inBracket = malloc(0);
                    }else
                    {
                        inBracketSize++;
                        inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                        inBracket[inBracketSize - 1] = Pack[i];
                    }
                    BracketsChild++;
                }
                break;
            case 11:
                {
                    BracketsChild--;
                    if (!BracketsChild && BracketCur == 1)
                    {
                        BracketCur = 0;
                    }else
                    {
                        inBracketSize++;
                        inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                        inBracket[inBracketSize - 1] = Pack[i];
                    }
                }
                break;
            case 12:
                {

                    if (!BracketsChild)
                    {
                        BracketCur = 2;
                        FirstBracketIndex = i;


                        inBracketSize = 0;
                        free(inBracket);
                        inBracket = malloc(0);
                    }else
                    {
                        inBracketSize++;
                        inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                        inBracket[inBracketSize - 1] = Pack[i];
                    }

                    BracketsChild++;
                }
                break;
                case 13:
                {
                    BracketsChild--;
                    if (!BracketsChild && BracketCur == 2)
                    {
                        int FunctionCalled = 0;
                        BracketCur = 0;

                        CountObj ChildCount = COUNT(inBracket, inBracketSize);

                        if (FirstBracketIndex > 0)
                        {
                             if (Pack[FirstBracketIndex - 1].ObjType == VARIABLE || Pack[FirstBracketIndex - 1].ObjType == VALUE)
                            {

                                if (Pack[FirstBracketIndex - 1].ObjType == VARIABLE)
                                {

                                    if (Pack[FirstBracketIndex - 1].VarUP->Val.ValueType == VALUE_FUNCTION_TYPE)
                                    {
                                        ValueReturnObj V = Function(
                                            Pack[FirstBracketIndex - 1].VarUP->Val.Area.Area,
                                            Pack[FirstBracketIndex - 1].VarUP->Val.Area.Size
                                            ).Vs;

                                        MioneObj* NewPack = malloc(0);
                                        int NewPackSize = 0;

                                        for (int index = 0; index < FirstBracketIndex-1; index++)
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }

                                        printf("sizoe of fuc returned : %d\n",V.ValueSize);

                                        if (V.ValueSize)
                                        {
                                            for (int index = 0; index < V.ValueSize; index++)
                                            {
                                                NewPackSize++;
                                                NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                                NewPack[NewPackSize - 1] = (MioneObj){
                                                    .ObjType = VALUE,
                                                    .Val = V.Value[index],
                                                    .Line = Pack[i-1].Line,
                                                    .Column = Pack[i-1].Column
                                                };
                                            }

                                        }else
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = (MioneObj){
                                                .ObjType = VALUE,
                                                .Val = (ValueObj){
                                                    .ValueType = VALUE_NOPOINTNUMBER_TYPE,.NPNumber = 0}
                                            };
                                        }



                                        for (int index = i+1 ; index < PackSize; index++)
                                        {

                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }



                                        PackSize = NewPackSize;
                                        Pack = NewPack;


                                        i = FirstBracketIndex - 1;

                                        FunctionCalled = 1;
                                    }
                                    else
                                    {

                                        char *str1 = "The Type of `";
                                        char *str2 = "` isn't a FUNCTION";
                                        char *VName = Pack[FirstBracketIndex - 1].VarUP->Name;

                                        char *REASON = malloc(sizeof(char) * (int)(strlen(str1) + strlen(str2) + strlen(VName) + 1));
                                        strcpy(REASON, str1);
                                        strcat(REASON, VName);
                                        strcat(REASON, str2);

                                        ErrCall(
                                            REASON,
                                                "MG001",
                                            "Maybe you can change the Variable Type to FUNCTION.",
                                            Pack[FirstBracketIndex - 1].Line,
                                            Pack[FirstBracketIndex - 1].Column

                                        );
                                    }
                                }
                                else
                                {
                                    if (Pack[FirstBracketIndex - 1].Val.ValueType == VALUE_FUNCTION_TYPE)
                                    {
                                        ValueReturnObj V = Function(
                                            Pack[FirstBracketIndex - 1].Val.Area.Area,
                                            Pack[FirstBracketIndex - 1].Val.Area.Size
                                            ).Vs;

                                        MioneObj* NewPack = malloc(0);
                                        int NewPackSize = 0;

                                        for (int index = 0; index < FirstBracketIndex-1; index++)
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }

                                        printf("sizoe of fuc returned : %d\n",V.ValueSize);

                                        if (V.ValueSize)
                                        {
                                            for (int index = 0; index < V.ValueSize; index++)
                                            {
                                                NewPackSize++;
                                                NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                                NewPack[NewPackSize - 1] = (MioneObj){
                                                    .ObjType = VALUE,
                                                    .Val = V.Value[index],
                                                    .Line = Pack[i-1].Line,
                                                    .Column = Pack[i-1].Column
                                                };

                                            }

                                        }else
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = (MioneObj){
                                                .ObjType = VALUE,
                                                .Val = (ValueObj){
                                                    .ValueType = VALUE_NOPOINTNUMBER_TYPE,.NPNumber = 0}
                                            };
                                        }


                                        for (int index = i+1 ; index < PackSize; index++)
                                        {

                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }

                                        PackSize = NewPackSize;
                                        Pack = NewPack;


                                        i = FirstBracketIndex - 1;





                                        FunctionCalled = 1;
                                    }
                                    else
                                    {
                                        ErrCall(
                                            "The Value before `()` Isn't a Function",
                                            "MG002",
                                            "Maybe you can try `function() <...:Mione> end()`.",
                                            Pack[FirstBracketIndex - 1].Line,
                                            Pack[FirstBracketIndex - 1].Column
                                        );
                                    }
                                }
                            }
                        }


                        if (!FunctionCalled)
                        {
                            MioneObj* NewPack = malloc(0);
                            int NewPackSize = 0;

                            for (int index = 0; index < FirstBracketIndex; index++)
                            {
                                NewPackSize++;
                                NewPack = realloc(NewPack, NewPackSize * sizeof(MioneObj));
                                NewPack[NewPackSize - 1] = Pack[index];
                            }


                            for (int index = 0; index < ChildCount.ValueSize; index++)
                            {
                                NewPackSize++;
                                NewPack = realloc(NewPack, NewPackSize * sizeof(MioneObj));
                                NewPack[NewPackSize - 1] = (MioneObj){
                                    .ObjType = VALUE,
                                    .Val = ChildCount.Value[index]
                                };
                            }

                            for (int index = i + 1; index < PackSize; index++)
                            {
                                NewPackSize++;
                                NewPack = realloc(NewPack, NewPackSize * sizeof(MioneObj));
                                NewPack[NewPackSize - 1] = Pack[index];
                            }

                            Pack = NewPack;
                            PackSize = NewPackSize;

                            i = FirstBracketIndex -1;
                        }
                    }else
                    {
                        inBracketSize++;
                        inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                        inBracket[inBracketSize - 1] = Pack[i];
                    }

                }
                break;
            default:
                    {
                        if(!BracketsChild){

                            if (CalculateLevel) switch (Pack[i].Symbol.SymbolType)
                            {
                                case 1:
                                    {

                                        if (i == PackSize - 1 || i - 1 <0)
                                        {
                                            ErrCall(
                                                "You can't connect any Two-side-count-SymbolGet.a to VOID (Meaning Nothing, even no Mione Object).",
                                                "MG003",
                                                "Maybe you can try `1+1` or anything else.",
                                                Pack[i].Line,
                                                Pack[i].Column
                                            );
                                        }



                                        // if (Pack[i-1].ObjType == SYMBOL || Pack[i+1].ObjType == SYMBOL)
                                        // {
                                        //     ErrCall(
                                        //       "dkaopkdapskdpsa",
                                        //       "MG3123i13",
                                        //       "Maybe you can try `1+1` or anything else.",
                                        //       Pack[i].Line,
                                        //       Pack[i].Column
                                        //   );
                                        // }
                                        if (Pack[i].Symbol.xIndex == CalculateLevel) CalculateType = Pack[i].Symbol.CurNumber;

                                        break;
                                    }
                                 case 2:
                                     {
                                        if (i == PackSize - 1 )
                                        {
                                            ErrCall(
                                                "bbbbb",
                                                "MG007",
                                                "Maybe you can try `typeof 1` or anything else.",
                                                Pack[i].Line,
                                                Pack[i].Column
                                            );
                                        }

                                        if (Pack[i+1].ObjType == SYMBOL)
                                        {
                                            ErrCall(
                                                "dmakldmadma",
                                                "MG03127",
                                                "Maybe you can try `typeof 1` or anything else.",
                                                Pack[i].Line,
                                                Pack[i].Column
                                            );
                                        }

                                        if (i)
                                        {
                                            if (Pack[i-1].ObjType != SYMBOL)
                                            {
                                                ErrCall(
                                                    "[dlsapld[pasd[l",
                                                    "dasdsadasa",
                                                    NULL,
                                                    Pack[i].Line,
                                                    Pack[i].Column
                                                );
                                            }
                                        }

                                        if (Pack[i].Symbol.xIndex == CalculateLevel) CalculateType = Pack[i].Symbol.CurNumber;

                                        break;
                                     }
                                case 3:
                                    {
                                        if (i == PackSize - 1)
                                        {
                                            ErrCall(
                                               "ccccc",
                                               "MG008",
                                               "Maybe you can try `- 1` or anything else.",
                                                Pack[i].Line,
                                                Pack[i].Column
                                           );
                                        }

                                        if ( Pack[i+1].ObjType == SYMBOL)
                                        {
                                            ErrCall(
                                               "jdioasjdioas",
                                               "MG0428",
                                               "Maybe you can try `- 1` or anything else.",
                                                Pack[i].Line,
                                                Pack[i].Column
                                           );
                                        }


                                        for (int index = 0; 1 ; index++)
                                        {
                                            if (Symbols[index].CurNumber == -1) break;

                                            if (strcmp(Symbols[index].Name, Pack[i].Symbol.Name) == 0)
                                            {

                                                if (i-1>=0 && (Pack[i-1].ObjType == VARIABLE || Pack[i-1].ObjType == VALUE))
                                                {
                                                    if (Pack[i].Symbol.yIndex == CalculateLevel) CalculateType = Pack[i].Symbol.CurNumber;
                                                }else
                                                {
                                                    if (Pack[i].Symbol.xIndex == CalculateLevel) CalculateType = Pack[i].Symbol.CurNumber;
                                                }

                                            }

                                        }
                                        break;
                                    }
                            }
                        }else
                        {
                            inBracketSize++;
                            inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                            inBracket[inBracketSize - 1] = Pack[i];
                        }
                    }
            }
            }
            break;
            default:
            {
            if (!BracketsChild)
            {
                if (Pack[i].ObjType == VALUE && Pack[i].Val.ValueType == VALUE_TABLE_TYPE && !Pack[i].Val.Table.Counted)
                {
                    ValueObj V = Pack[i].Val;

                    VariableObj * CountedTable = malloc(0);
                    int CountedTableSize = 0;

                    // MioneReturnObj R =
                    Table(V.Table.MioneTable,V.Table.MioneTableSize,&CountedTable,&CountedTableSize);


                    Pack[i].Val.Table.Counted=1;
                    Pack[i].Val.Table.MioneTable = NULL;
                    Pack[i].Val.Table.MioneTableSize = 0;

                    VariableObj * NewTable = malloc(0);
                    int NewTableSize = 0;

                    for (int TableChildIndex = 0; TableChildIndex<CountedTableSize ; TableChildIndex++)
                    {
                        NewTableSize++;
                        NewTable = realloc (NewTable, NewTableSize*sizeof(VariableObj));
                        NewTable[NewTableSize-1] = CountedTable[TableChildIndex];

                        if (CountedTable[TableChildIndex].Place)
                        {
                            for (int CTCIndex = 0; CTCIndex<NewTableSize ; CTCIndex++)
                            {
                                if (NewTable[CTCIndex].Place > NewTable[NewTableSize-1].Place &&NewTable[CTCIndex].Place )
                                {
                                    VariableObj Butter = NewTable[CTCIndex];
                                    NewTable[CTCIndex] = NewTable[NewTableSize-1];
                                    NewTable[NewTableSize-1] = Butter;
                                }
                            }
                        }
                    }


                    CountedTable = NewTable;
                    CountedTableSize = NewTableSize;

                    for (int TableChildIndex = 0; TableChildIndex<CountedTableSize ; TableChildIndex++)
                    {
                        if (CountedTable[TableChildIndex].Place) printf("bbb %d\n", CountedTable[TableChildIndex].Place);
                        if (CountedTable[TableChildIndex].Name) printf("aaa %s\n", CountedTable[TableChildIndex].Name);
                    }


                    Pack[i].Val.Table.CountedTable = CountedTable;
                    Pack[i].Val.Table.CountedTableSize = CountedTableSize;
                }

                if (CalculateType)
            {
                MioneObj Out;
                int UsePointNumber = 0;

                switch (CalculateType)
                {
                case 14:
                    {
                        ValueObj Target = Pack[i].ObjType == VARIABLE ? Pack[i].VarUP->Val : Pack[i].Val;

                        extern DefineVariableObj * Dvo;
                        extern int DvoSize;

                        VariableObj * FoundVarUP = NULL;

                       for (int DvoIndex = 0; DvoIndex < DvoSize; DvoIndex++)
                       {
                           for (int VariableIndex = 0; VariableIndex < *(Dvo[DvoIndex].VariablesSizeUP); VariableIndex++)
                           {
                               switch (Target.ValueType)
                               {
                               case VALUE_STRING_TYPE:
                                   {
                                       if ((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Name && strcmp((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Name, Target.String) == 0)
                                       {
                                           FoundVarUP = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex];
                                       }
                                   }
                                   break;

                               case VALUE_NOPOINTNUMBER_TYPE:
                                   {
                                       if ((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Place == Target.NPNumber)
                                       {
                                           FoundVarUP = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex];

                                       }
                                   }
                                   break;
                                   default:
                                       {
                                           ErrCall("dasdaskdoppas","dasjiodjaosjdoias",NULL,Pack[i-1].Line,Pack[i-1].Column);
                                       }
                               }

                           }
                       }

                        if (!FoundVarUP)
                        {
                            FoundVarUP = malloc(sizeof(VariableObj));

                            if (Target.ValueType == VALUE_STRING_TYPE)
                            {
                                *FoundVarUP = (VariableObj){
                                    .Name = Target.String
                                };
                            }else
                            {
                                *FoundVarUP = (VariableObj){
                                    .Place = Target.NPNumber
                                };
                            }

                            (*Dvo[DvoSize-1].VariablesSizeUP)++;
                            (*Dvo[DvoSize-1].VariableUPsUP) = realloc((*Dvo[DvoSize-1].VariableUPsUP), (*Dvo[DvoSize-1].VariablesSizeUP) * sizeof(VariableObj**));
                            (*Dvo[DvoSize-1].VariableUPsUP)[(*Dvo[DvoSize-1].VariablesSizeUP)-1] = FoundVarUP;
                        }

                        Out = (MioneObj){
                            .ObjType = VARIABLE,
                            .VarUP = FoundVarUP,
                        };

                        PastCost = 1;
                        CalculateType = 0;
                        break;

                    }
                    default:
                        ErrCall("dsa","不支援",NULL,Pack[i-1].Line,Pack[i-1].Column);

                        break;
                }

                MioneObj* NewPack = malloc(0);
                int NewPackSize = 0;


                for (int index = 0; index < i - PastCost; index++)
                {
                    NewPackSize++;
                    NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                    NewPack[NewPackSize - 1] = Pack[index];
                }

                NewPackSize++;
                NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                NewPack[NewPackSize - 1] = Out;
                int ToIndex = NewPackSize - 1;

                for (int index = i + 1; index < PackSize; index++)
                {
                    NewPackSize++;
                    NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                    NewPack[NewPackSize - 1] = Pack[index];
                }

                PackSize = NewPackSize;
                Pack = NewPack;

                i = ToIndex-1;
            }
            }else
            {
                inBracketSize++;
                inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                inBracket[inBracketSize - 1] = Pack[i];
            }
        }
        }

        if (i == PackSize - 1)
        {
            CalculateLevel++;
            i = -1;// ...困擾了我好久
            if (CalculateLevel > 3) { break; }

        }
    }

    VariableObj * * VPack = malloc(0);
    int VPackSize = 0;

    //printf("sizeof %d\n",PackSize);


    for (int i = 0; i < PackSize; i++)
    {

        if ( Pack[i].ObjType == VALUE)
        {
            ErrCall("dadsad","kopghtkoo",NULL,0,0);
        }else if (Pack[i].ObjType == VARIABLE)
        {
            VPackSize ++;
            VPack = realloc(VPack, sizeof(VariableObj*) * (VPackSize));
            VPack[VPackSize-1] = Pack[i].VarUP;
        }
    }

    if (BracketCur) ErrCall("cadasdas","dasdada",NULL,0,0);

    VariableRequestUPObj Returns = (VariableRequestUPObj){
        .VariableUPs = VPack,
        .VariablesSize= VPackSize
    };

    return Returns;
}

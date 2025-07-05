//
// Created by calle on 24-11-3.
//

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ERR.h"
#include "Mione.h"
#include "PROMPT_DEF.h"
#include "SYMBOL_DEF.h"
#include "IMPLEMENT.h"

extern inline int _mix(const int a,const int b)
{
    return a>b ? b : a;
}

CountObj COUNT(MioneObj*Pack,int PackSize)
{
    int FirstBracketIndex = 0; //頭括號位置
    int BracketsChild = 0; //括號多寡

    int BracketCur = 0;//括號樣式 1 : [, 2 : (

    MioneObj* inBracket = malloc(0);
    int inBracketSize = 0;

    int CountLoop = 1; //執行需求

    for (int CountIndex = 0;CountIndex < CountLoop;CountIndex++) //Count Layers
    for(int i = 0; i < PackSize; i++)
    {
        int PastCost = 0; //計算符號所扣除之項目 如 1 + 1 (大小3) 將輸出 2 (大小1) PastCost = 2;

        switch (Pack[i].ObjType)
        {
        case SYMBOL:
            {
                switch (Pack[i].Symbol.Identification)
                {
                case 10:{
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
                case 11:{
                    BracketsChild--;
                    if (!BracketsChild && BracketCur == 1)
                    {
                        BracketCur = 0;

                        CountObj ChildCount = COUNT(inBracket, inBracketSize);

                        ValueObj TheValue = {0};

                        if (ChildCount.ValueSize==1){
                            if (FirstBracketIndex - 1 >=0)
                            {
                                ValueObj nearByValue;

                                switch (Pack[FirstBracketIndex - 1].ObjType)
                                {
                                    case VARIABLE:
                                        {
                                            nearByValue = Pack[FirstBracketIndex - 1].VarUP->Val;
                                            break;
                                        }
                                    case VALUE:
                                        {
                                            nearByValue = Pack[FirstBracketIndex - 1].Val;
                                            break;
                                        }
                                    default: ErrCall("AAAA","DaaaASDASSASDadCVVCS",NULL,NULL,NULL);
                                }

                                if (nearByValue.ValueType == VALUE_TABLE_TYPE)
                                {

                                    for (int index = 0 ;index<nearByValue.Table.VariableUPsUP->VarUPsSize;index++)
                                    {
                                        switch (ChildCount.Value[0].ValueType)
                                        {
                                        case VALUE_STRING_TYPE:
                                            {

                                                if (wcscmp(nearByValue.Table.VariableUPsUP->VarUPs[index]->Name,ChildCount.Value[0].String) == 0 )
                                                    TheValue = nearByValue.Table.VariableUPsUP->VarUPs[index]->Val;
                                                break;
                                            }
                                        // case VALUE_NUMBER_TYPE:
                                        //     {
                                        //         if (nearByValue.Table.VariableUPsUP->VarUPs[index]->Place == ChildCount.Value[0].NPNumber)
                                        //             TheValue = nearByValue.Table.VariableUPsUP->VarUPs[index]->Val;
                                        //         break;
                                        //     }
                                        default:
                                            {
                                                ErrCall("AAAA","DASDASSASDCVVCS",NULL,NULL,NULL);
                                            }
                                        }
                                    }


                                }else ErrCall("kkopkopkopkopdasp","DASDASSASDadCVVCS",NULL,NULL,NULL);

                            }else
                            {
                                extern VariableObj * retVarUP(ScopeObj * SVUup,const wchar_t* Name,const int Place);

                                switch (ChildCount.Value[0].ValueType)
                                {
                                case VALUE_STRING_TYPE:
                                    {
                                        VariableObj * VarUP = retVarUP(Pack[0].ScopeUP,ChildCount.Value[0].String,0);

                                        if (VarUP) TheValue = VarUP->Val; else TheValue = (ValueObj){0};

                                        break;
                                    }
                                // case VALUE_NUMBER_TYPE:
                                //     {
                                //         VariableObj * VarUP = retVarUP(Pack[0].ScopeUP,NULL,ChildCount.Value[0].NPNumber);
                                //
                                //         if (VarUP) TheValue = VarUP->Val; else TheValue = (ValueObj){0};
                                //
                                //         break;
                                //     }
                                default:
                                    {
                                        ErrCall("AAAA","DASDASSASDCVVCS",NULL,NULL,NULL);
                                    }
                                }



                            }
                        }else ErrCall("DASDAS","DASDASD",NULL,NULL,NULL);

                        MioneObj* NewPack = malloc(0);
                        int NewPackSize = 0;


                        for (int index = 0; index < FirstBracketIndex-1; index++)
                        {
                            NewPackSize++;
                            NewPack = realloc(NewPack, sizeof(MioneObj) * NewPackSize);
                            NewPack[NewPackSize - 1] = Pack[index];
                        }

                        NewPackSize++;
                        NewPack = realloc(NewPack, sizeof(MioneObj) * NewPackSize);
                        NewPack[NewPackSize - 1] = (MioneObj){
                            .ObjType = VALUE,
                            .Val = TheValue,
                        };

                        for (int index = i+1; index < PackSize; index++)
                        {
                            NewPackSize++;
                            NewPack = realloc(NewPack, sizeof(MioneObj) * NewPackSize);
                            NewPack[NewPackSize - 1] = Pack[index];
                        }

                        PackSize = NewPackSize;
                        Pack = NewPack;

                        i = FirstBracketIndex-1;
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

                        break;
                    }
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

                                        ImplementedObj Return =  IMPLEMENT((ToImplementObj){
                                            .Built =  *Pack[FirstBracketIndex - 1].VarUP->Val.Area.AreaUP,
                                            .FunRequest = (ValuesObj){
                                                .Value = ChildCount.Value,
                                                .ValueSize = ChildCount.ValueSize,
                                            }
                                        });

                                        if (!Return.ToState&1) ErrCall(
                                            "The Function hasn't return any Value.",
                                            "MGs123",
                                            NULL,
                                            Pack[FirstBracketIndex - 1].Line,
                                            Pack[FirstBracketIndex - 1].Column
                                        );

                                        ValuesObj V = Return.Values;


                                        MioneObj* NewPack = malloc(0);
                                        int NewPackSize = 0;

                                        for (int index = 0; index < FirstBracketIndex-1; index++)
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }


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
                                        ImplementedObj Return = IMPLEMENT((ToImplementObj){
                                            .Built =  *Pack[FirstBracketIndex - 1].Val.Area.AreaUP,
                                            .FunRequest = (ValuesObj){
                                                .Value = ChildCount.Value,
                                                .ValueSize = ChildCount.ValueSize,
                                            }
                                        });



                                        if (!Return.ToState&1) ErrCall(
                                            "The Function hasn't return any Value.",
                                            "MGdas123",
                                            NULL,
                                            Pack[FirstBracketIndex - 1].Line,
                                            Pack[FirstBracketIndex - 1].Column
                                        );


                                        ValuesObj V = Return.Values;

                                        MioneObj* NewPack = malloc(0);
                                        int NewPackSize = 0;

                                        for (int index = 0; index < FirstBracketIndex-1; index++)
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }


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
                    break;
                }
                default:
                    {
                        if(!BracketsChild)
                        {
                            switch (Pack[i].Symbol.Identification)
                            {
                            case 1:
                                {
                                    if (CountIndex < 3)
                                    {
                                        CountLoop = _mix(CountIndex,3);
                                        continue;
                                    }

                                    if (!PackSize>i)
                                        exit(0xFF09);

                                    if (i)
                                        exit(0xFF09);

                                    ValueObj Value1 = Pack[i-1].ObjType == VALUE ? Pack[i-1].Val : Pack[i-1].VarUP->Val;
                                    ValueObj Value2 = Pack[i+1].ObjType == VALUE ? Pack[i+1].Val : Pack[i+1].VarUP->Val;;

                                    break;
                                }

                            default:
                                {

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
                break;
            }
        default:{
            if (!BracketsChild)
            {
                if (Pack[i].ObjType == VALUE && Pack[i].Val.ValueType == VALUE_TABLE_TYPE && !Pack[i].Val.Table.VariableUPsUP)
                {
                    ImplementedObj Return = IMPLEMENT((ToImplementObj){
                        .Built = *Pack[i].Val.Table.TableAreaUP
                    });

                    int VarsSize = Return.Vars.VarsSize;
                    VariableObj * Vars = Return.Vars.Vars;

                    VariableUPsObj NewTable;
                    NewTable.VarUPs = malloc(0);
                    NewTable.VarUPsSize = 0;


                    for (int TableChildIndex = 0; TableChildIndex<VarsSize ; TableChildIndex++)
                    {
                        NewTable.VarUPsSize++;
                        NewTable.VarUPs = realloc ( NewTable.VarUPs, NewTable.VarUPsSize*sizeof(VariableObj *));
                        NewTable.VarUPs[NewTable.VarUPsSize-1] = &Vars[TableChildIndex];

                        if (Vars[TableChildIndex].Place)
                        {
                            for (int CTCIndex = 0; CTCIndex<NewTable.VarUPsSize ; CTCIndex++)
                            {
                                if (NewTable.VarUPs[CTCIndex]->Place > NewTable.VarUPs[NewTable.VarUPsSize-1]->Place && NewTable.VarUPs[CTCIndex]->Place )
                                {
                                    VariableObj * Butter = NewTable.VarUPs[CTCIndex];
                                    NewTable.VarUPs[CTCIndex] = NewTable.VarUPs[NewTable.VarUPsSize-1];
                                    NewTable.VarUPs[NewTable.VarUPsSize-1] = Butter;
                                }
                            }
                        }
                    }

                    Pack[i].Val.Table.VariableUPsUP = malloc(sizeof(struct _VariablesObject *));

                    *Pack[i].Val.Table.VariableUPsUP = NewTable;
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

    ValueObj * VPack = malloc(0);
    int VPackSize = 0;

    for (int i = 0; i < PackSize; i++)
    {
        if ( Pack[i].ObjType == VALUE)
        {
            VPackSize ++;
            VPack = realloc(VPack, sizeof(ValueObj) * (VPackSize));
            VPack[VPackSize-1] = Pack[i].Val;
        }else if (Pack[i].ObjType == VARIABLE)
        {
            VPackSize ++;
            VPack = realloc(VPack, sizeof(ValueObj) * (VPackSize));
            VPack[VPackSize-1] = Pack[i].VarUP->Val;

        }
    }


    if (BracketCur) ErrCall("cadasdas","dasdada",NULL,0,0);

    CountObj Returns = (CountObj){
        .Value = VPack,
        .ValueSize= VPackSize
    };

    return Returns;
}
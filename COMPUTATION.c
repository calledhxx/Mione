#include <stdio.h>

#include "OBJECTS.h"
#include "ERR.h"
#include "COUNT.h"
#include "IMPLEMENT.h"
#include <stdlib.h>
#include <string.h>

extern inline int _mix(const int a,const int b)
{
    return a>b ? b : a;
}


MioneObjCarrier COMPUTATION(MioneObjCarrier input)
{
    unsigned int PackSize = input.CarrierLen;
    MioneObj* Pack = input.Carrier;

     if (PackSize == 0) return (MioneObjCarrier){0};

    int FirstBracketIndex = 0; //頭括號位置
    int BracketsChild = 0; //括號多寡

    uint8_t BracketCur = 0;//括號樣式 1 : [, 2 : (

    MioneObj* inBracket = malloc(0);
    int inBracketSize = 0;

    int CountLoop = 1; //執行需求

    for (int CountIndex = 0;CountIndex < CountLoop;CountIndex++) //Count Layers
    for(int i = 0; i < PackSize; i++)
    {
        int PastCost = 0; //計算符號所扣除之項目 如 1 + 1 (大小3) 將輸出 2 (大小1) 即PastCost = 2;

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

                        ValueObjCarrier ChildCount = COUNT(inBracket, inBracketSize);

                        ValueObj TheValue = {0};

                        if (ChildCount.CarrierLen==1){
                            if (FirstBracketIndex - 1 >=0)
                            {
                                ValueObj nearByValue;

                                switch (Pack[FirstBracketIndex - 1].ObjType)
                                {
                                    case VARIABLE:
                                        {
                                            nearByValue = Pack[FirstBracketIndex - 1].VariablePointer->Value;
                                            break;
                                        }
                                    case VALUE:
                                        {
                                            nearByValue = Pack[FirstBracketIndex - 1].Value;
                                            break;
                                        }
                                    default: ErrCall("AAAA","DaaaASDASSASDadCVVCS",NULL,NULL,NULL);
                                }

                                if (nearByValue.ValueType == VALUE_TABLE_TYPE)
                                {
                                    for (int index = 0 ;index<nearByValue.Table.VariableObjPointerCarrierPointer->CarrierLen;index++)
                                    {
                                        switch (ChildCount.Carrier[0].ValueType)
                                        {
                                            //todo
                                        // case VALUE_STRING_TYPE:
                                        //     {
                                        //
                                        //         if (wcscmp(nearByValue.Table.VariableUPsUP->VarUPs[index]->Name,ChildCount.Value[0].String) == 0 )
                                        //             TheValue = nearByValue.Table.VariableUPsUP->VarUPs[index]->Val;
                                        //         break;
                                        //     }
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

                                switch (ChildCount.Carrier[0].ValueType)
                                {
                                    //todo

                                // case VALUE_STRING_TYPE:
                                //     {
                                //         VariableObj * VarUP = retVarUP(Pack[0].ScopeUP,ChildCount.Value[0].String,0);
                                //
                                //         if (VarUP) TheValue = VarUP->Val; else TheValue = (ValueObj){0};
                                //
                                //         break;
                                //     }
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
                            .Value = TheValue,
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

                        ValueObjCarrier ChildCount = COUNT(inBracket, inBracketSize);

                        if (FirstBracketIndex > 0)
                        {
                             if (Pack[FirstBracketIndex - 1].ObjType == VARIABLE || Pack[FirstBracketIndex - 1].ObjType == VALUE)
                            {

                                if (Pack[FirstBracketIndex - 1].ObjType == VARIABLE)
                                {

                                    if (Pack[FirstBracketIndex - 1].VariablePointer->Value.ValueType == VALUE_FUNCTION_TYPE)
                                    {

                                        ImplementedObj Return =  IMPLEMENT((ToImplementObj){
                                            .Built =  *Pack[FirstBracketIndex - 1].VariablePointer->Value.Area.TrainObjCarrierPointer,
                                            .CallByValueCarrier = ChildCount
                                        });

                                        if (!Return.ToState&1) exit(1);

                                        ValueObjCarrier V = Return.ValueCarrier;


                                        MioneObj* NewPack = malloc(0);
                                        int NewPackSize = 0;

                                        for (int index = 0; index < FirstBracketIndex-1; index++)
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }


                                        for (int index = 0; index < V.CarrierLen; index++)
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = (MioneObj){
                                                .ObjType = VALUE,
                                                .Value = V.Carrier[index],
                                                // todo .Line = Pack[i-1].Line,
                                                // .Column = Pack[i-1].Column
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
                                        exit(1);
                                }
                                else
                                {
                                    if (Pack[FirstBracketIndex - 1].Value.ValueType == VALUE_FUNCTION_TYPE)
                                    {
                                        ImplementedObj Return = IMPLEMENT((ToImplementObj){
                                            .Built =  *Pack[FirstBracketIndex - 1].Value.Area.TrainObjCarrierPointer,
                                            .CallByValueCarrier = ChildCount
                                        });



                                        if (!Return.ToState&1)
                                            exit(1);


                                        ValueObjCarrier V = Return.ValueCarrier;

                                        MioneObj* NewPack = malloc(0);
                                        int NewPackSize = 0;

                                        for (int index = 0; index < FirstBracketIndex-1; index++)
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }


                                        for (int index = 0; index < V.CarrierLen; index++)
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = (MioneObj){
                                                .ObjType = VALUE,
                                                .Value = V.Carrier[index],
                                                //TODO .Line = Pack[i-1].Line,
                                                // .Column = Pack[i-1].Column
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
                                        exit(1);
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


                            for (int index = 0; index < ChildCount.CarrierLen; index++)
                            {
                                NewPackSize++;
                                NewPack = realloc(NewPack, NewPackSize * sizeof(MioneObj));
                                NewPack[NewPackSize - 1] = (MioneObj){
                                    .ObjType = VALUE,
                                    .Value = ChildCount.Carrier[index]
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

                                    ValueObj Value1 = Pack[i-1].ObjType == VALUE ? Pack[i-1].Value : Pack[i-1].VariablePointer->Value;
                                    ValueObj Value2 = Pack[i+1].ObjType == VALUE ? Pack[i+1].Value : Pack[i+1].VariablePointer->Value;;

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
                if (Pack[i].ObjType == VALUE && Pack[i].Value.ValueType == VALUE_TABLE_TYPE && !Pack[i].Value.Table.VariableObjPointerCarrierPointer)
                {
                    ImplementedObj Return = IMPLEMENT((ToImplementObj){
                        .Built = *Pack[i].Value.Table.TrainObjCarrierPointer
                    });

                    int VarsSize = Return.VariableCarrier.CarrierLen;
                    VariableObj * Vars = Return.VariableCarrier.Carrier;

                    VariableObjPtrCarrier NewTable;
                    NewTable.Carrier = NULL;
                    NewTable.CarrierLen = 0;


                    for (int TableChildIndex = 0; TableChildIndex<VarsSize ; TableChildIndex++)
                    {
                        NewTable.CarrierLen++;
                        NewTable.Carrier = realloc ( NewTable.Carrier, NewTable.CarrierLen*sizeof(VariableObj *));
                        NewTable.Carrier[NewTable.CarrierLen-1] = &Vars[TableChildIndex];

                        if (Vars[TableChildIndex].VariablePlace)
                        {
                            for (int CTCIndex = 0; CTCIndex<NewTable.CarrierLen ; CTCIndex++)
                            {
                                if (NewTable.Carrier[CTCIndex]->VariablePlace > NewTable.Carrier[NewTable.CarrierLen-1]->VariablePlace && NewTable.Carrier[CTCIndex]->VariablePlace )
                                {
                                    VariableObj * Butter = NewTable.Carrier[CTCIndex];
                                    NewTable.Carrier[CTCIndex] = NewTable.Carrier[NewTable.CarrierLen-1];
                                    NewTable.Carrier[NewTable.CarrierLen-1] = Butter;
                                }
                            }
                        }
                    }

                    Pack[i].Value.Table.VariableObjPointerCarrierPointer = malloc(sizeof(struct _VariablesObject *));

                    *Pack[i].Value.Table.VariableObjPointerCarrierPointer = NewTable;
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

    if (BracketCur) ErrCall("cadasdas","dasdada",NULL,0,0);

    return (MioneObjCarrier){
        .Carrier = Pack,
        .CarrierLen = PackSize,
    };
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "STDMIO.h"

#define _max(a,b) a<b ? b : a

MioneObjCarrier COMPUTATION(MioneObjCarrier input)
{
    unsigned int PackSize = input.CarrierLen;
    MioneObj* Pack = input.Carrier;

     if (PackSize == 0) return (MioneObjCarrier){0};

    int FirstBracketIndex = 0; //頭括號位置
    int BracketsChild = 0; //括號多寡

    char BracketCur = 0;//括號樣式 1 : [, 2 : (

    MioneObj* inBracket = NULL;
    int inBracketSize = 0;

    unsigned CountLoop = 1; //執行需求

    for (int CountIndex = 0;CountIndex < CountLoop;CountIndex++) //Count Layers
    for(int i = 0; i < PackSize; i++)
    {
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

                    break;
                }
                case 11:{
                    BracketsChild--;
                    if (!BracketsChild && BracketCur == 1)
                    {
                        BracketCur = 0;

                        ValueObjCarrier ChildCount = COUNT((MioneObjCarrier){
                            .Carrier = inBracket,
                            .CarrierLen = inBracketSize
                        });

                        ValueObj TheValue = {0};

                        if (ChildCount.CarrierLen==1){
                            if (FirstBracketIndex - 1 >=0)
                            {
                                ValueObj nearByValue;

                                switch (Pack[FirstBracketIndex - 1].ObjType)
                                {
                                    case VARIABLE:
                                        {
                                            nearByValue = ReturnVariablePtrFromLink(*Pack[FirstBracketIndex - 1].VariableLinkPtr)->Value;
                                            break;
                                        }
                                    case VALUE:
                                        {
                                            nearByValue = Pack[FirstBracketIndex - 1].Value;
                                            break;
                                        }
                                    default: exit(1);
                                }

                                if (nearByValue.ValueType == VALUE_TABLE_TYPE)
                                {
                                    for (int index = 0 ;index<nearByValue.Table.VariableObjCarrierPointer->CarrierLen;index++)
                                    {
                                        switch (ChildCount.Carrier[0].ValueType)
                                        {
                                        case VALUE_STRING_TYPE:
                                            {

                                                if (strcmp(nearByValue.Table.VariableObjCarrierPointer->Carrier[index].VariableName,ChildCount.Carrier[0].String) == 0 )
                                                    TheValue = nearByValue.Table.VariableObjCarrierPointer->Carrier[index].Value;
                                                break;
                                            }
                                        case VALUE_NUMBER_TYPE:
                                            {
                                                if (nearByValue.Table.VariableObjCarrierPointer->Carrier[index].VariablePlace == (unsigned)ChildCount.Carrier[0].Number)
                                                    TheValue = nearByValue.Table.VariableObjCarrierPointer->Carrier[index].Value;
                                                break;
                                            }
                                        default:
                                                exit(1);
                                        }
                                    }


                                }else exit(1);

                            }else
                            {
                                switch (ChildCount.Carrier[0].ValueType)
                                {

                                case VALUE_STRING_TYPE:
                                    {
                                        int inParentScope = 0;

                                        VariableLinkObj * VariableLinkPtr = ReturnVariableLinkPtrIfAlreadyExistedInScope(
                                            *Pack[0].PointerOfScope,
                                            ChildCount.Carrier[0].String,
                                            0,
                                            &inParentScope
                                            );

                                        if (VariableLinkPtr)
                                            TheValue = ReturnVariablePtrFromLink(*VariableLinkPtr)->Value;

                                        break;
                                    }
                                case VALUE_NUMBER_TYPE:
                                    {
                                        int inParentScope = 0;

                                        VariableLinkObj * VariableLinkPtr = ReturnVariableLinkPtrIfAlreadyExistedInScope(
                                            *Pack[0].PointerOfScope,
                                            0,
                                            (int)ChildCount.Carrier[0].Number,
                                            &inParentScope
                                            );


                                        if (VariableLinkPtr)
                                            TheValue = ReturnVariablePtrFromLink(*VariableLinkPtr)->Value;

                                        break;
                                    }
                                default:
                                        exit(1);
                                }


                            }
                        }else exit(1);

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

                    break;
                }
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

                        ValueObjCarrier ChildCount = COUNT((MioneObjCarrier){
                            .Carrier = inBracket,
                            .CarrierLen = inBracketSize
                        });

                        if (FirstBracketIndex > 0)
                        {
                             if (Pack[FirstBracketIndex - 1].ObjType == VARIABLE || Pack[FirstBracketIndex - 1].ObjType == VALUE)
                            {

                                if (Pack[FirstBracketIndex - 1].ObjType == VARIABLE)
                                {

                                    if (ReturnVariablePtrFromLink(*Pack[FirstBracketIndex - 1].VariableLinkPtr)->Value.ValueType == VALUE_FUNCTION_TYPE)
                                    {

                                        EventObj IMPLEMENTReturn =  IMPLEMENT((ToImplementObj){
                                            .Built =  *ReturnVariablePtrFromLink(*Pack[FirstBracketIndex - 1].VariableLinkPtr)->Value.Area.TrainObjCarrierPointer,
                                            .CallByValueCarrier = ChildCount
                                        });

                                        if (!IMPLEMENTReturn.ToState&1)
                                            exit(-5);

                                        ValueObjCarrier V = IMPLEMENTReturn.ReturnValues;

                                        if (!V.CarrierLen)
                                            exit(-6);

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
                                                .MioneObjectPosition = Pack[i-1].MioneObjectPosition
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
                                        EventObj IMPLEMENTReturn = IMPLEMENT((ToImplementObj){
                                            .Built =  *Pack[FirstBracketIndex - 1].Value.Area.TrainObjCarrierPointer,
                                            .CallByValueCarrier = ChildCount
                                        });



                                        if (!IMPLEMENTReturn.ToState&1)
                                            exit(-5);


                                        ValueObjCarrier V = IMPLEMENTReturn.ReturnValues;

                                        if (!V.CarrierLen)
                                            exit(-6);

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
                                                .MioneObjectPosition = Pack[i-1].MioneObjectPosition
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
                            int NewPackSize = 0;
                            MioneObj* NewPack = NULL;

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

                            signed FrontIndex = 0;
                            signed BackIndex = 0;

                            MioneObjCarrier Output = {0};

                            switch (Pack[i].Symbol.Identification)
                            {
                            case 1:
                                {
                                    if (CountIndex < 2)
                                    {
                                        CountLoop = _max(CountIndex,3);
                                        continue;
                                    }


                                    if (!(i - 1 >= 0 && i - 1 <= PackSize - 1))
                                        exit(2);

                                    if (!(i + 1 >= 0 && i + 1 <= PackSize - 1))
                                        exit(3);

                                    ValueObj Value1 = Pack[i-1].ObjType == VALUE ? Pack[i-1].Value : ReturnVariablePtrFromLink(*Pack[i-1].VariableLinkPtr)->Value;
                                    ValueObj Value2 = Pack[i+1].ObjType == VALUE ? Pack[i+1].Value : ReturnVariablePtrFromLink(*Pack[i+1].VariableLinkPtr)->Value;

                                    if (Value1.ValueType != VALUE_NUMBER_TYPE || Value2.ValueType != VALUE_NUMBER_TYPE)
                                        exit(5);


                                    FrontIndex = i - 2;
                                    BackIndex = i + 2;

                                    Output.CarrierLen = 1;
                                    Output.Carrier = malloc(sizeof(MioneObj));
                                    *Output.Carrier = (MioneObj){
                                        .VariableLinkPtr = Pack[i].VariableLinkPtr,
                                        .MioneObjectPosition = Pack[i].MioneObjectPosition,
                                        .ObjType = VALUE,
                                        .Value = (ValueObj){
                                            .ValueType = VALUE_NUMBER_TYPE,
                                            .Number = Value1.Number + Value2.Number
                                        },
                                    };

                                    break;
                                }

                            default:
                                {

                                }
                            }

                            unsigned NewPackSize = PackSize - (BackIndex - FrontIndex - 1) + Output.CarrierLen;
                            MioneObj * NewPack = malloc(NewPackSize * sizeof(MioneObj));
                            memcpy(
                                NewPack,
                                Pack,
                                sizeof(MioneObj) * (FrontIndex + 1)
                                );
                            memcpy(
                                NewPack + sizeof(MioneObj) * (FrontIndex + 1),
                                Output.Carrier,
                                sizeof(MioneObj) * Output.CarrierLen
                                );

                            if (PackSize >= BackIndex + 1)
                                memcpy(
                                    NewPack + sizeof(MioneObj) * (FrontIndex + 1 + Output.CarrierLen),
                                    Pack + BackIndex + 1,
                                    sizeof(MioneObj) * PackSize - BackIndex - 1
                                );

                            free(Pack);

                            Pack = NewPack;
                            PackSize = NewPackSize;

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
                if (Pack[i].ObjType == VALUE && Pack[i].Value.ValueType == VALUE_TABLE_TYPE && !Pack[i].Value.Table.VariableObjCarrierPointer)
                {
                    EventObj IMPLEMENTReturn = IMPLEMENT((ToImplementObj){
                        .Built = *Pack[i].Value.Table.TrainObjCarrierPointer
                    });

                    unsigned VarsSize = IMPLEMENTReturn.MajorVariables.CarrierLen;
                    VariableObj * Vars = IMPLEMENTReturn.MajorVariables.Carrier;

                    VariableObjCarrier NewTable;
                    NewTable.Carrier = NULL;
                    NewTable.CarrierLen = 0;

                    for (int TableChildIndex = 0; TableChildIndex<VarsSize ; TableChildIndex++)
                    {
                        NewTable.CarrierLen++;
                        NewTable.Carrier = realloc ( NewTable.Carrier, NewTable.CarrierLen*sizeof(VariableObj *));
                        NewTable.Carrier[NewTable.CarrierLen-1] = Vars[TableChildIndex];

                        if (Vars[TableChildIndex].VariablePlace)
                        {
                            for (int CTCIndex = 0; CTCIndex<NewTable.CarrierLen ; CTCIndex++)
                            {
                                if (NewTable.Carrier[CTCIndex].VariablePlace > NewTable.Carrier[NewTable.CarrierLen-1].VariablePlace && NewTable.Carrier[CTCIndex].VariablePlace )
                                {
                                    VariableObj Butter = NewTable.Carrier[CTCIndex];
                                    NewTable.Carrier[CTCIndex] = NewTable.Carrier[NewTable.CarrierLen-1];
                                    NewTable.Carrier[NewTable.CarrierLen-1] = Butter;
                                }
                            }
                        }
                    }

                    Pack[i].Value.Table.TrainObjCarrierPointer = NULL;
                    Pack[i].Value.Table.VariableObjCarrierPointer = malloc(sizeof(VariableObjCarrier));
                    *Pack[i].Value.Table.VariableObjCarrierPointer = NewTable;
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

    if (inBracket)
        free(inBracket);

    if (BracketCur) exit(1);

    return (MioneObjCarrier){
        .Carrier = Pack,
        .CarrierLen = PackSize,
    };
}
//
// Created by calle on 24-12-28.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "STDMIO.h"


VariableObj * * ReturnPointerOfVariablePtrIfAlreadyExistedInScope(
    const ScopeObj Scope,
    const char * VariableName,
    const int VariableIndex,
    int * inParentScope
    )
{
    VariableObj * * result = 0;

    for (
        unsigned int Index = 0;
        Index < Scope.PtrOfVariablePtrCarrier.CarrierLen;
        Index++
        )
        if (VariableIndex)
        {
            if (strcmp((*Scope.PtrOfVariablePtrCarrier.Carrier[Index])->VariableName,VariableName)==0)
                result = Scope.PtrOfVariablePtrCarrier.Carrier[Index];
        }else
            if ((*Scope.PtrOfVariablePtrCarrier.Carrier[Index])->VariablePlace==VariableIndex)
                result = Scope.PtrOfVariablePtrCarrier.Carrier[Index];
    if (result)
        return result;

    if (Scope.ParentScopePointer)
    {
        *inParentScope = 1;
        return ReturnPointerOfVariablePtrIfAlreadyExistedInScope(* Scope.ParentScopePointer,VariableName,VariableIndex,inParentScope);
    }

    return NULL;
}

static const char *NormalKeyword[] = {
    "function",
    "range",
    "end"
};

static const char *UnconnectableKeyword[] = {
    "{",
    "}",
};

EventObj CMO(
    const CaseObjCarrier Carrier,
    ScopeObj * ScopePointer
    )
{
    EventObj result = {0};

    const CaseObj * CaseCarrier = Carrier.Carrier;
    const unsigned int CaseCarrierLen = Carrier.CarrierLen;

    MioneObjCarrier ResultMioneObjCarrier = {0};
    MioneObjCarrier AreaMioneObjCarrier = {0};

    MioneObjCarrier * MioneObjCarrierPtr = &ResultMioneObjCarrier;

    signed NumberOfPack = 0;
    unsigned PackBy = 0;
    

    for (
        unsigned int CaseCarrierIndex = 0;
        CaseCarrierIndex < CaseCarrierLen;
        CaseCarrierIndex++
        )
    {
        char Paired = 0; //是否已經配對過了。

        const CaseObj ThisCase = CaseCarrier[CaseCarrierIndex];

        switch (ThisCase.ObjType) // 檢查是否匹配 BREAKER, HEAD ,PROMPT與SYMBOL，否則進行第二處理。
        {

        case CASE_BREAKER: //新增Breaker
            {
                MioneObjCarrierPtr->CarrierLen++;
                MioneObjCarrierPtr->Carrier = realloc(
                    MioneObjCarrierPtr->Carrier,
                    MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                );
                MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1].ObjType = 0;

                Paired = 1;

                break;
            }
        case CASE_NORMAL: //一般，該藍處裡 Head 與其他VALUE關鍵字
            {
                for (
                  unsigned int keywordDetectIndex = 0;
                  keywordDetectIndex < (unsigned)(sizeof(NormalKeyword)/sizeof(char*));
                  keywordDetectIndex++
                  )
                    if (strcmp(ThisCase.ObjName, NormalKeyword[keywordDetectIndex])==0)
                    {
                        Paired = 1;

                        switch (keywordDetectIndex)
                        {
                        case 0: //function
                            {
                                if (!NumberOfPack)
                                {
                                    MioneObjCarrierPtr = &AreaMioneObjCarrier;
                                    PackBy = VALUE_FUNCTION_TYPE;

                                    ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen++;
                                    ScopePointer->ChildrenScopePtrCarrierPointer->Carrier = realloc(
                                        ScopePointer->ChildrenScopePtrCarrierPointer->Carrier,
                                        sizeof(ScopeObj*) * ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen
                                        );
                                    ScopePointer->ChildrenScopePtrCarrierPointer->Carrier[
                                        ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen - 1
                                        ] =
                                            malloc(sizeof(ScopeObj));
                                    *ScopePointer->ChildrenScopePtrCarrierPointer->Carrier[
                                        ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen - 1
                                        ] = (ScopeObj){
                                                .ParentScopePointer = ScopePointer,
                                                .PtrOfVariablePtrCarrier = (PtrVariableObjPtrCarrier){0},
                                                .ChildrenScopePtrCarrierPointer = malloc(sizeof(ScopeObjPtrCarrier)),
                                            };
                                    *ScopePointer->ChildrenScopePtrCarrierPointer->Carrier[
                                        ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen - 1
                                        ]->ChildrenScopePtrCarrierPointer = (ScopeObjPtrCarrier){0};

                                    ScopePointer = ScopePointer->ChildrenScopePtrCarrierPointer->Carrier[
                                        ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen - 1
                                        ];
                                }
                                NumberOfPack++;

                                break;
                            }

                        case 1: //range
                            {
                                if (!NumberOfPack)
                                {
                                    MioneObjCarrierPtr = &AreaMioneObjCarrier;
                                    PackBy = VALUE_RANGE_TYPE;

                                    ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen++;
                                    ScopePointer->ChildrenScopePtrCarrierPointer->Carrier = realloc(
                                        ScopePointer->ChildrenScopePtrCarrierPointer->Carrier,
                                        sizeof(ScopeObj*) * ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen
                                        );
                                    ScopePointer->ChildrenScopePtrCarrierPointer->Carrier[
                                        ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen - 1
                                        ] =
                                            malloc(sizeof(ScopeObj));
                                    *ScopePointer->ChildrenScopePtrCarrierPointer->Carrier[
                                        ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen - 1
                                        ] = (ScopeObj){
                                            .ParentScopePointer = ScopePointer,
                                            .PtrOfVariablePtrCarrier = (PtrVariableObjPtrCarrier){0},
                                            .ChildrenScopePtrCarrierPointer = malloc(sizeof(ScopeObjPtrCarrier)),
                                        };
                                    *ScopePointer->ChildrenScopePtrCarrierPointer->Carrier[
                                        ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen - 1
                                        ]->ChildrenScopePtrCarrierPointer = (ScopeObjPtrCarrier){0};

                                    ScopePointer = ScopePointer->ChildrenScopePtrCarrierPointer->Carrier[
                                        ScopePointer->ChildrenScopePtrCarrierPointer->CarrierLen - 1
                                        ];
                                }
                                NumberOfPack++;

                                break;
                            }
                        case 2: //end
                            {
                                NumberOfPack--;
                                if (!NumberOfPack)
                                {
                                    if (PackBy == VALUE_FUNCTION_TYPE || PackBy == VALUE_RANGE_TYPE)
                                    {
                                        ScopePointer = ScopePointer->ParentScopePointer;

                                        MioneObjCarrierPtr = &ResultMioneObjCarrier;

                                        TrainObjCarrier * TrainObjCarrierPtr = malloc(sizeof(TrainObjCarrier));

                                        const EventObj ToMioneReturn = ToMione(AreaMioneObjCarrier);

                                        if (ToMioneReturn.ToState & EVENT_ERROR)
                                            exit(-1);

                                        *TrainObjCarrierPtr = ToMioneReturn.TrainCarrier;

                                        MioneObjCarrierPtr->CarrierLen++;
                                        MioneObjCarrierPtr->Carrier = realloc(
                                            MioneObjCarrierPtr->Carrier,
                                            MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                                        );


                                        MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1] = (MioneObj){
                                            .ObjType = VALUE,
                                            .PointerOfScope = ScopePointer,
                                            .MioneObjectPosition = ThisCase.CasePosition,
                                            .Value = (ValueObj){
                                                .ValueType = PackBy,
                                                .Area = (AreaObj){
                                                    .TrainObjCarrierPointer = TrainObjCarrierPtr
                                                }
                                            }
                                        };

                                        PackBy = 0;
                                    }else
                                        exit(-5);

                                }if (NumberOfPack < 0)
                                    exit(-2);

                                break;
                            }

                        default:
                            exit(-1);
                        }

                        break;
                    }

                if (Paired) break;


                for (
                    unsigned int HeadDetectIndex = 0;
                    Heads[HeadDetectIndex].Identification;
                    HeadDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, Heads[HeadDetectIndex].Name) == 0)
                    {
                        MioneObjCarrierPtr->CarrierLen++;
                        MioneObjCarrierPtr->Carrier = realloc(
                            MioneObjCarrierPtr->Carrier,
                            MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                        );
                        MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1] = (MioneObj){
                            .ObjType = HEAD,
                            .PointerOfScope = ScopePointer,
                            .MioneObjectPosition = ThisCase.CasePosition,
                            .Head = Heads[HeadDetectIndex]
                        };


                        Paired = 1;

                        break;
                    }
                }

                if (Paired) break;

            }
        case CASE_CONNECTABLE: //搭配著尚未分配的Normal Case。該藍處理Prompt 及 Symbol 與部分VALUE
        case CASE_UNCONNECTABLE:
            {
                  for (
                  unsigned int keywordDetectIndex = 0;
                  keywordDetectIndex < (unsigned)(sizeof(UnconnectableKeyword)/sizeof(char*));
                  keywordDetectIndex++
                  )
                    if (strcmp(ThisCase.ObjName, UnconnectableKeyword[keywordDetectIndex])==0)
                    {

                        Paired = 1;

                        switch (keywordDetectIndex)
                        {
                        case 0: // {
                            {
                                if (!NumberOfPack)
                                {
                                    MioneObjCarrierPtr = &AreaMioneObjCarrier;
                                    PackBy = VALUE_TABLE_TYPE;
                                }
                                NumberOfPack++;

                                break;
                            }

                        case 1: // }
                            {
                                NumberOfPack--;
                                if (!NumberOfPack)
                                {
                                    if (PackBy == VALUE_TABLE_TYPE)
                                    {
                                        MioneObjCarrierPtr = &ResultMioneObjCarrier;

                                        TrainObjCarrier * TrainObjCarrierPtr = malloc(sizeof(TrainObjCarrier));

                                        const EventObj ToMioneReturn = ToMione(AreaMioneObjCarrier);

                                        if (ToMioneReturn.ToState & EVENT_ERROR)
                                            exit(-1);

                                        *TrainObjCarrierPtr = ToMioneReturn.TrainCarrier;

                                        MioneObjCarrierPtr->CarrierLen++;
                                        MioneObjCarrierPtr->Carrier = realloc(
                                            MioneObjCarrierPtr->Carrier,
                                            MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                                        );


                                        MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1] = (MioneObj){
                                            .ObjType = VALUE,
                                            .PointerOfScope = ScopePointer,
                                            .MioneObjectPosition = ThisCase.CasePosition,
                                            .Value = (ValueObj){
                                                .ValueType = PackBy,
                                                .Table = (TableObj){
                                                    .TrainObjCarrierPointer = TrainObjCarrierPtr
                                                }
                                            }
                                        };

                                        PackBy = 0;
                                    }
                                    else
                                        exit(-3);

                                }

                                break;
                            }

                        default:
                            exit(-1);
                        }

                        break;
                    }

                if (Paired) break;


                for (
                    unsigned int PromptDetectIndex = 0;
                    Prompts[PromptDetectIndex].Identification;
                    PromptDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, Prompts[PromptDetectIndex].Name) == 0)
                    {
                        MioneObjCarrierPtr->CarrierLen++;
                        MioneObjCarrierPtr->Carrier = realloc(
                            MioneObjCarrierPtr->Carrier,
                            MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                        );
                        MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1] = (MioneObj){
                            .ObjType = PROMPT,
                            .PointerOfScope = ScopePointer,
                            .MioneObjectPosition = ThisCase.CasePosition,
                            .Prompt = Prompts[PromptDetectIndex]
                        };

                        Paired = 1;

                        break;
                    }
                }

                if (Paired) break;

                for (
                    unsigned int SymbolDetectIndex = 0;
                    Symbols[SymbolDetectIndex].Identification;
                    SymbolDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, Symbols[SymbolDetectIndex].Name) == 0)
                    {
                        MioneObjCarrierPtr->CarrierLen++;
                        MioneObjCarrierPtr->Carrier = realloc(
                            MioneObjCarrierPtr->Carrier,
                            MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                        );
                        MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1] = (MioneObj){
                            .ObjType = SYMBOL,
                            .PointerOfScope = ScopePointer,
                            .MioneObjectPosition = ThisCase.CasePosition,
                            .Symbol = Symbols[SymbolDetectIndex]
                        };

                        Paired = 1;

                        break;
                    }
                }

                if (Paired) break;
            }

        default: //二次處理
            {
                switch (ThisCase.ObjType)
                {
                case CASE_NORMAL: //配對成Variable
                    {
                        int inParentScope = 0;

                        VariableObj * * VariablePtr = ReturnPointerOfVariablePtrIfAlreadyExistedInScope(
                            *ScopePointer,
                            ThisCase.ObjName,
                            0,
                            &inParentScope
                            );

                        VariableObj * * PointerOfScopeVariablePtr = NULL;

                        if (VariablePtr) //old Variable
                        {
                            if (inParentScope) //new Variable for this scope
                            {
                                ScopePointer->PtrOfVariablePtrCarrier.CarrierLen++;
                                ScopePointer->PtrOfVariablePtrCarrier.Carrier = realloc(
                                    ScopePointer->PtrOfVariablePtrCarrier.Carrier,
                                    sizeof(VariableObj**) * ScopePointer->PtrOfVariablePtrCarrier.CarrierLen
                                    );
                                ScopePointer->PtrOfVariablePtrCarrier.Carrier[ScopePointer->PtrOfVariablePtrCarrier.CarrierLen-1] = malloc(sizeof(VariableObj*));
                                *ScopePointer->PtrOfVariablePtrCarrier.Carrier[ScopePointer->PtrOfVariablePtrCarrier.CarrierLen-1] = *VariablePtr;
                                PointerOfScopeVariablePtr =
                                    ScopePointer->PtrOfVariablePtrCarrier.Carrier[ScopePointer->PtrOfVariablePtrCarrier.CarrierLen-1];
                            }else
                                PointerOfScopeVariablePtr = VariablePtr;
                        }
                        else //new Variable for all
                        {
                            ScopePointer->PtrOfVariablePtrCarrier.CarrierLen++;
                            ScopePointer->PtrOfVariablePtrCarrier.Carrier = realloc(
                                ScopePointer->PtrOfVariablePtrCarrier.Carrier,
                                sizeof(VariableObj*) * ScopePointer->PtrOfVariablePtrCarrier.CarrierLen
                                );
                            ScopePointer->PtrOfVariablePtrCarrier.Carrier[ScopePointer->PtrOfVariablePtrCarrier.CarrierLen-1] = malloc(sizeof(VariableObj*));
                            *ScopePointer->PtrOfVariablePtrCarrier.Carrier[ScopePointer->PtrOfVariablePtrCarrier.CarrierLen-1] = malloc(sizeof(VariableObj));
                            **ScopePointer->PtrOfVariablePtrCarrier.Carrier[ScopePointer->PtrOfVariablePtrCarrier.CarrierLen-1] =
                                (VariableObj){
                                    .VariableName = ThisCase.ObjName,
                                    .VariablePlace = 0,
                                };

                            PointerOfScopeVariablePtr =
                                ScopePointer->PtrOfVariablePtrCarrier.Carrier[ScopePointer->PtrOfVariablePtrCarrier.CarrierLen-1];
                        }


                        printf("[VARIABLE `%s`]\n    VARIABLE POINTER POSITION:%p;\n    VARIABLE POSITION:%p;\n",ThisCase.ObjName,PointerOfScopeVariablePtr,*PointerOfScopeVariablePtr);

                        MioneObjCarrierPtr->CarrierLen++;
                        MioneObjCarrierPtr->Carrier = realloc(
                            MioneObjCarrierPtr->Carrier,
                            MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                        );
                        MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1] = (MioneObj){
                            .ObjType = VARIABLE,
                            .PointerOfScope = ScopePointer,
                            .PointerOfScopeVariablePtr = PointerOfScopeVariablePtr,
                            .MioneObjectPosition = ThisCase.CasePosition,
                        };

                        Paired = 1;

                        break;
                    }
                case CASE_CONNECTABLE: //跳出 之後Error Handle 嘻嘻
                case CASE_UNCONNECTABLE:
                    {
                        result.ToState |= EVENT_ERROR | EVENT_CMO_RETURN;

                        result.Error = (ErrorObj){
                            .Message = "Unknown Word wanted to be SYMBOL.",
                            .Code = "0",
                            .ErrorPosition = ThisCase.CasePosition
                        };

                        result.MioneCarrier = (*MioneObjCarrierPtr);

                        return result;
                    }

                case CASE_DOUBLE_STRING: //配對成 Value 的String
                case CASE_SINGLE_STRING:
                    {
                        MioneObjCarrierPtr->CarrierLen++;
                        MioneObjCarrierPtr->Carrier = realloc(
                            MioneObjCarrierPtr->Carrier,
                            MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                        );
                        MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1] = (MioneObj){
                            .ObjType = VALUE,
                            .PointerOfScope = ScopePointer,
                            .Value = (ValueObj){
                                .String = ThisCase.ObjName,
                                .ValueType = VALUE_STRING_TYPE,
                            },
                            .MioneObjectPosition = ThisCase.CasePosition,
                        };

                        Paired = 1;

                        break;
                    }
                case CASE_DECNUMBER:
                    {
                        MioneObjCarrierPtr->CarrierLen++;
                        MioneObjCarrierPtr->Carrier = realloc(
                            MioneObjCarrierPtr->Carrier,
                            MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                        );

                        MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1] = (MioneObj){
                            .ObjType = VALUE,
                            .PointerOfScope = ScopePointer,
                            .Value = (ValueObj){
                                .ValueType = VALUE_NUMBER_TYPE,
                                .Number = (double)atoi(ThisCase.ObjName)
                            },
                            .MioneObjectPosition = ThisCase.CasePosition,
                        };

                        break;
                    }
                case CASE_BINNUMBER:
                    {
                        double Number = 0;

                        MioneObjCarrierPtr->CarrierLen++;
                        MioneObjCarrierPtr->Carrier = realloc(
                            MioneObjCarrierPtr->Carrier,
                            MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                        );

                        for (unsigned i = 0; i < strlen(ThisCase.ObjName); i++)
                            Number += (ThisCase.ObjName[i] != '0') * 1<<(strlen(ThisCase.ObjName) - i - 1);

                        MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1] = (MioneObj){
                            .ObjType = VALUE,
                            .PointerOfScope = ScopePointer,
                            .Value = (ValueObj){
                                .ValueType = VALUE_NUMBER_TYPE,
                                .Number = Number
                            },
                            .MioneObjectPosition = ThisCase.CasePosition,
                        };

                        break;
                    }
                case CASE_HEXNUMBER:{
                        double Number = 0;

                        MioneObjCarrierPtr->CarrierLen++;
                        MioneObjCarrierPtr->Carrier = realloc(
                            MioneObjCarrierPtr->Carrier,
                            MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                        );

                        for (unsigned i = 0; i < strlen(ThisCase.ObjName); i++)
                        {
                            char unit = ThisCase.ObjName[i];

                            if (ThisCase.ObjName[i] >= '0' && ThisCase.ObjName[i] <= '9')
                                unit -= '0';
                            else if (ThisCase.ObjName[i] >= 'a' && ThisCase.ObjName[i] <= 'f')
                                unit -=  'a' - 10;
                            else if (ThisCase.ObjName[i] >= 'A' && ThisCase.ObjName[i] <= 'F')
                                unit -=  'A' - 10;

                            Number += unit * pow(16,strlen(ThisCase.ObjName) - i - 1);

                        }

                        MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1] = (MioneObj){
                            .ObjType = VALUE,
                            .PointerOfScope = ScopePointer,
                            .Value = (ValueObj){
                                .ValueType = VALUE_NUMBER_TYPE,
                                .Number = Number
                            },
                            .MioneObjectPosition = ThisCase.CasePosition,
                        };

                        break;
                }

                default:break;
                }
            };
        }
    }

    if (NumberOfPack)
        exit(-2);

    result.ToState |= EVENT_CMO_RETURN;
    result.MioneCarrier = ResultMioneObjCarrier;

    return result;
}
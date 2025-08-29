//
// Created by calle on 24-12-28.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "STDMIO.h"




static const char *NormalKeyword[] = {
    "function",
    "range",
    "end"
};

static const char *UnconnectableKeyword[] = {
    "{",
    "}",
};

CMOFunctionRespondObj CMO(
    const CaseObjCarrier Carrier,
    ScopeObj * ScopePointer
    )
{
    CMOFunctionRespondObj Result = {0};

    HeadObjCarrier HeadList = ReturnHeadList();
    SymbolObjCarrier SymbolList = ReturnSymbolList();
    PromptObjCarrier PromptList = ReturnPromptList();

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
                                                .VariableLinkPtrCarrier = (VariableLinkPtrObjCarrier){0},
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
                                            .VariableLinkPtrCarrier = (VariableLinkPtrObjCarrier){0},
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

                                        const MIONEFunctionRespondObj ToMioneReturn = ToMione(AreaMioneObjCarrier);

                                        if (ToMioneReturn.Event.Code)
                                            exit(1);

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
                            exit(-5);
                        }

                        break;
                    }

                if (Paired) break;


                for (
                    unsigned int HeadDetectIndex = 0;
                    HeadDetectIndex < HeadList.CarrierLen;
                    HeadDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, HeadList.Carrier[HeadDetectIndex].Name) == 0)
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
                            .Head = HeadList.Carrier[HeadDetectIndex]
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

                                        const MIONEFunctionRespondObj ToMioneReturn = ToMione(AreaMioneObjCarrier);

                                        if (ToMioneReturn.Event.Code)
                                            exit(1);

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
                            exit(-3);
                        }

                        break;
                    }

                if (Paired) break;


                for (
                    unsigned int PromptDetectIndex = 0;
                    PromptDetectIndex < PromptList.CarrierLen;
                    PromptDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, PromptList.Carrier[PromptDetectIndex].Name) == 0)
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
                            .Prompt = PromptList.Carrier[PromptDetectIndex]
                        };

                        Paired = 1;

                        break;
                    }
                }

                if (Paired) break;

                for (
                    unsigned int SymbolDetectIndex = 0;
                    SymbolDetectIndex < SymbolList.CarrierLen;
                    SymbolDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, SymbolList.Carrier[SymbolDetectIndex].Name) == 0)
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
                            .Symbol = SymbolList.Carrier[SymbolDetectIndex]
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

                        VariableLinkObj * VariableLinkPtr = ReturnVariableLinkPtrIfAlreadyExistedInScope(
                            *ScopePointer,
                            ThisCase.ObjName,
                            0,
                            &inParentScope
                            );

                        VariableLinkObj * ToAddOnVariableLinkPtr = 0;

                        if (VariableLinkPtr) //有連結到
                        {
                            if (inParentScope) //在母層
                            {
                                ScopePointer->VariableLinkPtrCarrier.CarrierLen++;
                                ScopePointer->VariableLinkPtrCarrier.Carrier = realloc(
                                    ScopePointer->VariableLinkPtrCarrier.Carrier,
                                    sizeof(VariableLinkObj*) * ScopePointer->VariableLinkPtrCarrier.CarrierLen
                                );
                                ScopePointer->VariableLinkPtrCarrier.Carrier[
                                    ScopePointer->VariableLinkPtrCarrier.CarrierLen - 1
                                    ] = malloc(sizeof(VariableLinkObj));

                                *ScopePointer->VariableLinkPtrCarrier.Carrier[
                                    ScopePointer->VariableLinkPtrCarrier.CarrierLen - 1
                                    ] = (VariableLinkObj){
                                        .PointToAnotherLink = VariableLinkPtr,
                                        .VariablePtr = 0
                                    };

                                ToAddOnVariableLinkPtr = ScopePointer->VariableLinkPtrCarrier.Carrier[
                                ScopePointer->VariableLinkPtrCarrier.CarrierLen - 1
                                    ];

                            }else //已存在於本層
                            {
                                ToAddOnVariableLinkPtr = VariableLinkPtr;
                            }
                        }else //無連結 理解為新變數
                        {
                            ScopePointer->VariableLinkPtrCarrier.CarrierLen++;
                            ScopePointer->VariableLinkPtrCarrier.Carrier = realloc(
                                ScopePointer->VariableLinkPtrCarrier.Carrier,
                                sizeof(VariableLinkObj*) * ScopePointer->VariableLinkPtrCarrier.CarrierLen
                            );
                            ScopePointer->VariableLinkPtrCarrier.Carrier[
                                ScopePointer->VariableLinkPtrCarrier.CarrierLen - 1
                                ] = malloc(sizeof(VariableLinkObj));

                            *ScopePointer->VariableLinkPtrCarrier.Carrier[
                                ScopePointer->VariableLinkPtrCarrier.CarrierLen - 1
                                ] = (VariableLinkObj){
                                    .PointToAnotherLink = 0,
                                    .VariablePtr = malloc(sizeof(VariableObj ))
                                };

                           *ScopePointer->VariableLinkPtrCarrier.Carrier[
                                ScopePointer->VariableLinkPtrCarrier.CarrierLen - 1
                               ]->VariablePtr = (VariableObj){
                                   .VariableName = ThisCase.ObjName,
                                   .VariablePlace = 0,
                                   .Value = (ValueObj){0}
                               };

                            ToAddOnVariableLinkPtr =
                                ScopePointer->VariableLinkPtrCarrier.Carrier[
                                    ScopePointer->VariableLinkPtrCarrier.CarrierLen - 1
                                ];
                        }

                        printf("[`%s` VARIABLE]\n"
                               "    IN PARENT:%d;\n"
                               "    FOUND LINK:%p;\n"
                               "    LINK TO LINK:%p;\n"
                               "    LINK TO VARIABLE:%p;\n"
                               "    LINK POSITION:%p;\n",
                            ThisCase.ObjName,
                            inParentScope,
                            VariableLinkPtr,
                            ToAddOnVariableLinkPtr->PointToAnotherLink,
                            ToAddOnVariableLinkPtr->VariablePtr,
                            ToAddOnVariableLinkPtr
                            );

                        MioneObjCarrierPtr->CarrierLen++;
                        MioneObjCarrierPtr->Carrier = realloc(
                            MioneObjCarrierPtr->Carrier,
                            MioneObjCarrierPtr->CarrierLen * sizeof(MioneObj)
                        );
                        MioneObjCarrierPtr->Carrier[MioneObjCarrierPtr->CarrierLen - 1] = (MioneObj){
                            .ObjType = VARIABLE,
                            .PointerOfScope = ScopePointer,
                            .VariableLinkPtr = ToAddOnVariableLinkPtr,
                            .MioneObjectPosition = ThisCase.CasePosition,
                        };

                        Paired = 1;

                        break;
                    }
                case CASE_CONNECTABLE: //跳出 之後Error Handle 嘻嘻
                case CASE_UNCONNECTABLE:
                    {
                        Result.Event = (EventObj){
                            .Code = 2,
                            .Message = "Unknown Word wanted to be SYMBOL.",
                            .EventPosition = ThisCase.CasePosition
                        };

                        Result.MioneCarrier = ResultMioneObjCarrier;

                        return Result;
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

    Result.MioneCarrier = ResultMioneObjCarrier;

    return Result;
}
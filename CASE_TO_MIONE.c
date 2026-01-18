//
// Created by calle on 24-12-28.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "STDMIO.h"

static const char * const NormalKeyword[] = {
    "function",
    "range",
    "end",
    "true",
    "false"
};

static const char * const UnconnectableKeyword[] = {
    "{",
    "}",
};

static MioneLayoutObj popLayoutFromLayoutCarrier(MioneLayoutObjCarrier * LayoutCarrierPtr)
{
    if (1>= LayoutCarrierPtr->CarrierLen) //這是神經病吧？
        exit(-121);

    const MioneLayoutObj Layout = LayoutCarrierPtr->Carrier[LayoutCarrierPtr->CarrierLen - 1];
    LayoutCarrierPtr->CarrierLen--;
    LayoutCarrierPtr->Carrier = realloc(
        LayoutCarrierPtr->Carrier,
        sizeof(MioneLayoutObj) * LayoutCarrierPtr->CarrierLen
        );


    return Layout;
}

static void pushLayoutIntoLayoutCarrier(MioneLayoutObjCarrier * LayoutCarrierPtr,const MioneLayoutObj NewLayout)
{
    LayoutCarrierPtr->CarrierLen++;
    LayoutCarrierPtr->Carrier = realloc(
        LayoutCarrierPtr->Carrier,
        LayoutCarrierPtr->CarrierLen * sizeof(MioneLayoutObj)
        );
    LayoutCarrierPtr->Carrier[LayoutCarrierPtr->CarrierLen - 1] = NewLayout;
}

static void pushMioneObjectIntoLayout(MioneLayoutObj * LayoutPtr,const MioneObj Object)
{

    LayoutPtr->MioneObjectsCarrier.CarrierLen++;
    LayoutPtr->MioneObjectsCarrier.Carrier = realloc(
        LayoutPtr->MioneObjectsCarrier.Carrier,
        LayoutPtr->MioneObjectsCarrier.CarrierLen * sizeof(MioneObj)
        );
    LayoutPtr->MioneObjectsCarrier.Carrier[LayoutPtr->MioneObjectsCarrier.CarrierLen - 1] = Object;
}

CMOFunctionRespondObj CMO(
    CMOFunctionRequestObj input
    )
{
    CaseObjCarrier CaseCarrierObj = input.CassCarrier;
    ScopeObj * ScopePointer = input.ScopePointer;

    CMOFunctionRespondObj Result = {0};
    Result.Event = input.EventTemplate;

    extern HeadObjCarrier HeadList;
    extern SymbolObjCarrier SymbolList;
    extern PromptObjCarrier PromptList;
    extern WeldObjCarrier WeldList;

    const CaseObj * CaseCarrier = CaseCarrierObj.Carrier;
    const unsigned int CaseCarrierLen = CaseCarrierObj.CarrierLen;

    MioneLayoutObjCarrier LayoutsCarrier = {0};

    pushLayoutIntoLayoutCarrier(&LayoutsCarrier,(MioneLayoutObj){0});

    for (
        unsigned int CaseCarrierIndex = 0;
        CaseCarrierIndex < CaseCarrierLen;
        CaseCarrierIndex++
        )
    {
        char Paired = 0; //是否已經配對過了。

        const CaseObj ThisCase = CaseCarrier[CaseCarrierIndex];

        switch (ThisCase.ObjType) // 檢查是否匹配 BREAKER, HEAD ,PROMPT,WELD與SYMBOL，否則進行第二處理。
        {
        case CASE_BREAKER: //新增Breaker
            {
                pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1],(MioneObj){0});

                Paired = 1;

                break;
            }
        case CASE_NORMAL: //一般，該藍處裡 Head ,Weld 與其他VALUE關鍵字
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
                                pushLayoutIntoLayoutCarrier(&LayoutsCarrier,(MioneLayoutObj){.MioneObjectsCarrier = (MioneObjCarrier){0}, .LayoutHandler = VALUE_FUNCTION_TYPE });

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

                                Paired = 1;

                                break;
                            }

                        case 1: //range
                            {
                                pushLayoutIntoLayoutCarrier(&LayoutsCarrier,(MioneLayoutObj){.MioneObjectsCarrier = (MioneObjCarrier){0}, .LayoutHandler = VALUE_RANGE_TYPE });

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

                                Paired = 1;

                                break;
                            }
                        case 2: //end
                            {
                                const MioneLayoutObj layout = popLayoutFromLayoutCarrier(&LayoutsCarrier);

                                if (layout.LayoutHandler != VALUE_FUNCTION_TYPE && layout.LayoutHandler != VALUE_RANGE_TYPE)
                                {
                                    Result.Event.Code = EVENT_LAYOUT_ERROR;
                                    Result.Event.Message = "Close of Layout `FUNCTION` or `RANGE` should be `END`";
                                    Result.Event.EventPosition = ThisCase.CasePosition;
                                    goto end;
                                }

                                ScopePointer = ScopePointer->ParentScopePointer;

                                TrainObjCarrier * TrainObjCarrierPtr = malloc(sizeof(TrainObjCarrier));

                                const MIONEFunctionRespondObj ToMioneReturn = ToMione((MIONEFunctionRequestObj){
                                    .MioneCarrier = layout.MioneObjectsCarrier,
                                    .EventTemplate = input.EventTemplate
                                });

                                if (ToMioneReturn.Event.Code)
                                {
                                    Result.Event = ToMioneReturn.Event;
                                    goto end;
                                }

                                *TrainObjCarrierPtr = ToMioneReturn.TrainCarrier;

                                pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1],(MioneObj){
                                    .ObjType = VALUE,
                                    .PointerOfScope = ScopePointer,
                                    .MioneObjectPosition = ThisCase.CasePosition,
                                    .Value = (ValueObj){
                                        .ValueType = layout.LayoutHandler,
                                        .Area = (AreaObj){
                                            .TrainObjCarrierPointer = TrainObjCarrierPtr
                                        }
                                    }
                                });

                                Paired = 1;

                                break;
                            }
                        case 3: //true
                            {
                                ValueObj Value = {0};
                                Value.db = 1;
                                Value.ValueType = VALUE_DB_TYPE;

                                pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1], (MioneObj){
                                    .ObjType = VALUE,
                                    .PointerOfScope = ScopePointer,
                                    .MioneObjectPosition = ThisCase.CasePosition,
                                    .Value = Value
                                });

                                Paired = 1;

                                break;
                            }
                        case 4: //false
                            {
                                ValueObj Value = {0};
                                Value.db = 0;
                                Value.ValueType = VALUE_DB_TYPE;

                                pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1], (MioneObj){
                                    .ObjType = VALUE,
                                    .PointerOfScope = ScopePointer,
                                    .MioneObjectPosition = ThisCase.CasePosition,
                                    .Value = Value
                                });

                                Paired = 1;

                                break;
                            }
                        default:
                            printf("the other keywords come up unexpectedly (NormalKeyword).\n");
                            exit(-5);
                        }

                        break;
                    }

                if (Paired) continue;

                for (
                    unsigned int HeadDetectIndex = 0;
                    HeadDetectIndex < HeadList.CarrierLen;
                    HeadDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, HeadList.Carrier[HeadDetectIndex].Name) == 0)
                    {
                        pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1],(MioneObj){
                            .ObjType = HEAD,
                            .PointerOfScope = ScopePointer,
                            .MioneObjectPosition = ThisCase.CasePosition,
                            .Head = HeadList.Carrier[HeadDetectIndex]
                        });

                        

                        Paired = 1;

                        break;
                    }
                }

                if (Paired) continue;

                for (
                    unsigned int WeldDetectIndex = 0;
                    WeldDetectIndex < WeldList.CarrierLen;
                    WeldDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, WeldList.Carrier[WeldDetectIndex].Name) == 0)
                    {
                        pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1],(MioneObj){
                            .ObjType = WELD,
                            .PointerOfScope = ScopePointer,
                            .MioneObjectPosition = ThisCase.CasePosition,
                            .Weld = WeldList.Carrier[WeldDetectIndex]
                        });

                        

                        Paired = 1;

                        break;
                    }
                }

                if (Paired) continue;


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
                                pushLayoutIntoLayoutCarrier(&LayoutsCarrier,(MioneLayoutObj){.MioneObjectsCarrier = (MioneObjCarrier){0}, .LayoutHandler = VALUE_TABLE_TYPE });


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

                                Paired = 1;

                                

                                break;
                            }

                        case 1: // }
                            {
                                const MioneLayoutObj layout = popLayoutFromLayoutCarrier(&LayoutsCarrier);

                                if (layout.LayoutHandler != VALUE_TABLE_TYPE)
                                {
                                    Result.Event.Code = EVENT_LAYOUT_ERROR;
                                    Result.Event.Message = "Close of Layout `{` should be `}`";
                                    Result.Event.EventPosition = ThisCase.CasePosition;
                                    goto end;
                                }

                                TrainObjCarrier * TrainObjCarrierPtr = malloc(sizeof(TrainObjCarrier));

                                const MIONEFunctionRespondObj ToMioneReturn = ToMione((MIONEFunctionRequestObj){
                                    .MioneCarrier = layout.MioneObjectsCarrier,
                                    .EventTemplate = input.EventTemplate
                                });

                                if (ToMioneReturn.Event.Code)
                                {
                                    Result.Event = ToMioneReturn.Event;
                                    goto end;
                                }


                                *TrainObjCarrierPtr = ToMioneReturn.TrainCarrier;

                                pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1],(MioneObj){
                                    .ObjType = VALUE,
                                    .PointerOfScope = ScopePointer,
                                    .MioneObjectPosition = ThisCase.CasePosition,
                                    .Value = (ValueObj){
                                        .ValueType = layout.LayoutHandler,
                                        .Table = (TableObj){
                                            .TrainObjCarrierPointer = TrainObjCarrierPtr
                                        }
                                    }
                                });

                                Paired = 1;

                                break;
                            }

                        default:
                            printf("the other keywords come up unexpectedly (UnconnectableKeyword).\n");
                            exit(-3);
                        }

                        break;
                    }

                if (Paired) continue;

                for (
                    unsigned int PromptDetectIndex = 0;
                    PromptDetectIndex < PromptList.CarrierLen;
                    PromptDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, PromptList.Carrier[PromptDetectIndex].Name) == 0)
                    {
                        pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1],(MioneObj){
                            .ObjType = PROMPT,
                            .PointerOfScope = ScopePointer,
                            .MioneObjectPosition = ThisCase.CasePosition,
                            .Prompt = PromptList.Carrier[PromptDetectIndex]
                        });


                        Paired = 1;

                        break;
                    }
                }

                if (Paired) continue;

                for (
                    unsigned int SymbolDetectIndex = 0;
                    SymbolDetectIndex < SymbolList.CarrierLen;
                    SymbolDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, SymbolList.Carrier[SymbolDetectIndex].Name) == 0)
                    {
                        pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1], (MioneObj){
                            .ObjType = SYMBOL,
                            .PointerOfScope = ScopePointer,
                            .MioneObjectPosition = ThisCase.CasePosition,
                            .Symbol = SymbolList.Carrier[SymbolDetectIndex]
                        });


                        Paired = 1;

                        

                        break;
                    }
                }

                if (Paired) continue;
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

                        VariableLinkObj * FinialVariableLinkPtr = 0;

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

                                FinialVariableLinkPtr = ScopePointer->VariableLinkPtrCarrier.Carrier[
                                ScopePointer->VariableLinkPtrCarrier.CarrierLen - 1
                                    ];

                            }else //已存在於本層
                                FinialVariableLinkPtr = VariableLinkPtr;
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
                                    .VariablePtr = malloc(sizeof(VariableObj))
                                };

                           *ScopePointer->VariableLinkPtrCarrier.Carrier[
                                ScopePointer->VariableLinkPtrCarrier.CarrierLen - 1
                               ]->VariablePtr = (VariableObj){
                                   .VariableName =
                                       memcpy(
                                           malloc(sizeof(char) * (strlen(ThisCase.ObjName) + 1)),
                                           ThisCase.ObjName,
                                           strlen(ThisCase.ObjName) + 1),
                                   .VariablePlace = 0,
                                   .Value = (ValueObj){0}
                               };

                            FinialVariableLinkPtr =
                                ScopePointer->VariableLinkPtrCarrier.Carrier[
                                    ScopePointer->VariableLinkPtrCarrier.CarrierLen - 1
                                ];
                        }

                        // printf("[`%s` VARIABLE]\n"
                        //        "    IN PARENT:%d;\n"
                        //        "    FOUND LINK:%p;\n"
                        //        "    LINK TO LINK:%p;\n"
                        //        "    LINK TO VARIABLE:%p;\n"
                        //        "    LINK POSITION:%p;\n",
                        //     ThisCase.ObjName,
                        //     inParentScope,
                        //     VariableLinkPtr,
                        //     FinialVariableLinkPtr->PointToAnotherLink,
                        //     FinialVariableLinkPtr->VariablePtr,
                        //     FinialVariableLinkPtr
                        //     );


                        pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1], (MioneObj){
                            .ObjType = VARIABLE,
                            .PointerOfScope = ScopePointer,
                            .VariableLinkPtr = FinialVariableLinkPtr,
                            .MioneObjectPosition = ThisCase.CasePosition,
                        });


                        Paired = 1;

                        break;
                    }
                case CASE_CONNECTABLE: //跳出 之後Error Handle 嘻嘻
                case CASE_UNCONNECTABLE:
                    {
                        Result.Event.Code = EVENT_MO_ERROR;
                        Result.Event.Message = "Unknown word tried to be SYMBOL.";
                        Result.Event.EventPosition = ThisCase.CasePosition;

                        Result.MioneCarrier = LayoutsCarrier.Carrier[0].MioneObjectsCarrier;

                        return Result;
                    }

                case CASE_DOUBLE_STRING: //配對成 Value 的String
                case CASE_SINGLE_STRING:
                    {
                        pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1], (MioneObj){
                            .ObjType = VALUE,
                            .PointerOfScope = ScopePointer,
                            .Value = (ValueObj){
                                .String = ThisCase.ObjName,
                                .ValueType = VALUE_STRING_TYPE,
                            },
                            .MioneObjectPosition = ThisCase.CasePosition,
                        });


                        Paired = 1;

                        break;
                    }
                case CASE_DECNUMBER:
                    {

                        pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1], (MioneObj){
                            .ObjType = VALUE,
                            .PointerOfScope = ScopePointer,
                            .Value = (ValueObj){
                                .ValueType = VALUE_NUMBER_TYPE,
                                .Number = (double)atoi(ThisCase.ObjName)
                            },
                            .MioneObjectPosition = ThisCase.CasePosition,
                        });

                        

                        break;
                    }
                case CASE_BINNUMBER:
                    {
                        double Number = 0;

                        for (unsigned i = 0; i < strlen(ThisCase.ObjName); i++)
                            Number += (ThisCase.ObjName[i] != '0') * 1<<(strlen(ThisCase.ObjName) - i - 1);

                        pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1], (MioneObj){
                            .ObjType = VALUE,
                            .PointerOfScope = ScopePointer,
                            .Value = (ValueObj){
                                .ValueType = VALUE_NUMBER_TYPE,
                                .Number = Number
                            },
                            .MioneObjectPosition = ThisCase.CasePosition,
                        });

                        

                        break;
                    }
                case CASE_HEXNUMBER:{
                        double Number = 0;

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

                        pushMioneObjectIntoLayout(&LayoutsCarrier.Carrier[LayoutsCarrier.CarrierLen - 1], (MioneObj){
                            .ObjType = VALUE,
                            .PointerOfScope = ScopePointer,
                            .Value = (ValueObj){
                                .ValueType = VALUE_NUMBER_TYPE,
                                .Number = Number
                            },
                            .MioneObjectPosition = ThisCase.CasePosition,
                        });

                        

                        break;
                }

                default:break;
                }
            };
        }

        free(ThisCase.ObjName);
    }
    // free(CaseCarrierObj.Carrier);

    if (LayoutsCarrier.CarrierLen!=1)
    {
        Result.Event.Code = EVENT_LAYOUT_ERROR;
        Result.Event.Message = "Layout not closed properly.";
        goto end;
    }

    end:

    Result.MioneCarrier = LayoutsCarrier.Carrier[0].MioneObjectsCarrier;

    free(LayoutsCarrier.Carrier);

    return Result;
}
//
// Created by calle on 24-12-28.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "STDMIO.h"


VariableObj * ReturnVariablePtrIfAlreadyExistedInScope(
    const ScopeObj Scope,
    const char * VariableName
    )
{
    VariableObj * result = 0;

    for (
        unsigned int VariableIndex = 0;
        VariableIndex < Scope.VariablePtrCarrier.CarrierLen;
        VariableIndex++
        )
        if (strcmp(Scope.VariablePtrCarrier.Carrier[VariableIndex]->VariableName,VariableName)==0)
            result = Scope.VariablePtrCarrier.Carrier[VariableIndex];

    return result;
}


MioneObjCarrier CMO(
    const CaseObjCarrier Carrier,
    ScopeObj * ScopePointer
    )
{
    const CaseObj * CaseCarrier = Carrier.Carrier;
    const unsigned int CaseCarrierLen = Carrier.CarrierLen;

    MioneObjCarrier ResultMioneObjCarrier = {0};

    for (
        unsigned int CaseCarrierIndex = 0;
        CaseCarrierIndex < CaseCarrierLen;
        CaseCarrierIndex++
        )
    {
        uint8_t Paired = 0; //是否已經配對過了。

        const CaseObj ThisCase = CaseCarrier[CaseCarrierIndex];

        switch (ThisCase.ObjType) // 檢查是否匹配 BREAKER, HEAD ,PROMPT與SYMBOL，否則進行第二處理。
        {
        case CASE_BREAKER: //新增Breaker
            {
                ResultMioneObjCarrier.CarrierLen++;
                ResultMioneObjCarrier.Carrier = realloc(
                    ResultMioneObjCarrier.Carrier,
                    ResultMioneObjCarrier.CarrierLen * sizeof(MioneObj)
                );
                ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1].ObjType = 0;

                Paired = 1;

                break;
            }
        case CASE_NORMAL: //一般，該藍處裡 Head
            {

                for (
                    unsigned int HeadDetectIndex = 0;
                    Heads[HeadDetectIndex].Identification;
                    HeadDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, Heads[HeadDetectIndex].Name) == 0)
                    {
                        ResultMioneObjCarrier.CarrierLen++;
                        ResultMioneObjCarrier.Carrier = realloc(
                            ResultMioneObjCarrier.Carrier,
                            ResultMioneObjCarrier.CarrierLen * sizeof(MioneObj)
                        );
                        ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1] = (MioneObj){
                            .ObjType = HEAD,
                            .MioneObjectPosition = ThisCase.CasePosition,
                            .Head = Heads[HeadDetectIndex]
                        };


                        Paired = 1;

                        break;
                    }
                }

            }
        case CASE_CONNECTABLE: //搭配著尚未分配的Normal Case。該藍處理Prompt 及 Symbol
        case CASE_UNCONNECTABLE:
            {
                for (
                    unsigned int PromptDetectIndex = 0;
                    Prompts[PromptDetectIndex].Identification;
                    PromptDetectIndex++
                    )
                {
                    if (strcmp(ThisCase.ObjName, Prompts[PromptDetectIndex].Name) == 0)
                    {
                        ResultMioneObjCarrier.CarrierLen++;
                        ResultMioneObjCarrier.Carrier = realloc(
                            ResultMioneObjCarrier.Carrier,
                            ResultMioneObjCarrier.CarrierLen * sizeof(MioneObj)
                        );
                        ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1] = (MioneObj){
                            .ObjType = PROMPT,
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
                        ResultMioneObjCarrier.CarrierLen++;
                        ResultMioneObjCarrier.Carrier = realloc(
                            ResultMioneObjCarrier.Carrier,
                            ResultMioneObjCarrier.CarrierLen * sizeof(MioneObj)
                        );
                        ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1] = (MioneObj){
                            .ObjType = SYMBOL,
                            .MioneObjectPosition = ThisCase.CasePosition,
                            .Symbol = Symbols[SymbolDetectIndex]
                        };

                        Paired = 1;

                        break;
                    }
                }

                if (Paired) break;
            }
        case CASE_DOUBLE_STRING: //配對成 Value 的String
        case CASE_SINGLE_STRING:
            {
                ResultMioneObjCarrier.CarrierLen++;
                ResultMioneObjCarrier.Carrier = realloc(
                    ResultMioneObjCarrier.Carrier,
                    ResultMioneObjCarrier.CarrierLen * sizeof(MioneObj)
                );
                ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1] = (MioneObj){
                    .ObjType = VALUE,
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
                extern void printIntegerObj();

                printIntegerObj(stringToNumber(ThisCase.ObjName).Integer);
                break;
            }
        default: //二次處理
            {
                switch (ThisCase.ObjType)
                {
                case CASE_NORMAL: //配對成Variable
                    {
                        VariableObj * VariablePtr = ReturnVariablePtrIfAlreadyExistedInScope(
                            *ScopePointer,
                            ThisCase.ObjName
                            );

                        if (!VariablePtr)
                        {
                            ScopePointer->VariablePtrCarrier.CarrierLen++;
                            ScopePointer->VariablePtrCarrier.Carrier = realloc(
                                ScopePointer->VariablePtrCarrier.Carrier,
                                sizeof(VariableObj*) * ScopePointer->VariablePtrCarrier.CarrierLen
                                );
                            VariablePtr = ScopePointer->VariablePtrCarrier.Carrier[ScopePointer->VariablePtrCarrier.CarrierLen-1] =
                                malloc(sizeof(VariableObj));
                            *ScopePointer->VariablePtrCarrier.Carrier[ScopePointer->VariablePtrCarrier.CarrierLen-1] =
                                (VariableObj){
                                    .VariableName = ThisCase.ObjName,
                                    .VariablePlace = 0,
                                };
                        }

                        ResultMioneObjCarrier.CarrierLen++;
                        ResultMioneObjCarrier.Carrier = realloc(
                            ResultMioneObjCarrier.Carrier,
                            ResultMioneObjCarrier.CarrierLen * sizeof(MioneObj)
                        );
                        ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1] = (MioneObj){
                            .ObjType = VARIABLE,
                            .VariablePointer = VariablePtr,
                            .MioneObjectPosition = ThisCase.CasePosition,
                        };

                        Paired = 1;

                        break;
                    }
                case CASE_CONNECTABLE: //跳出 之後Error Handle 嘻嘻
                case CASE_UNCONNECTABLE:
                    {
                        exit(-3);
                    }
                default:break;
                }
            };
        }
    }

    return ResultMioneObjCarrier;
}
//
// Created by calle on 24-12-28.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "STDMIO.h"


VariableObj * ReturnVariablePtrIfAlreadyExistedInScope(
    const ScopeObj Scope,
    const char * VariableName,
    int * inParentScope
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

    if (result)
        return result;

    if (Scope.ParentScopePointer)
    {
        *inParentScope = 1;
        return ReturnVariablePtrIfAlreadyExistedInScope(* Scope.ParentScopePointer,VariableName,inParentScope);
    }

    return NULL;
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

        default: //二次處理
            {
                switch (ThisCase.ObjType)
                {
                case CASE_NORMAL: //配對成Variable
                    {
                        int inParentScope = 0;

                        VariableObj * VariablePtr = ReturnVariablePtrIfAlreadyExistedInScope(
                            *ScopePointer,
                            ThisCase.ObjName,
                            &inParentScope
                            );

                        VariableObj * * PointerOfScopeVariablePtr = malloc(sizeof(VariableObj *));

                        if (VariablePtr) //old Variable
                        {
                            if (inParentScope) //new Variable for this scope
                            {
                                ScopePointer->VariablePtrCarrier.CarrierLen++;
                                ScopePointer->VariablePtrCarrier.Carrier = realloc(
                                    ScopePointer->VariablePtrCarrier.Carrier,
                                    sizeof(VariableObj*) * ScopePointer->VariablePtrCarrier.CarrierLen
                                    );
                                ScopePointer->VariablePtrCarrier.Carrier[ScopePointer->VariablePtrCarrier.CarrierLen-1] = VariablePtr;
                                *PointerOfScopeVariablePtr =
                                    ScopePointer->VariablePtrCarrier.Carrier[ScopePointer->VariablePtrCarrier.CarrierLen-1];
                            }else
                                *PointerOfScopeVariablePtr = VariablePtr;
                        }
                        else //new Variable for all
                        {
                            ScopePointer->VariablePtrCarrier.CarrierLen++;
                            ScopePointer->VariablePtrCarrier.Carrier = realloc(
                                ScopePointer->VariablePtrCarrier.Carrier,
                                sizeof(VariableObj*) * ScopePointer->VariablePtrCarrier.CarrierLen
                                );
                            ScopePointer->VariablePtrCarrier.Carrier[ScopePointer->VariablePtrCarrier.CarrierLen-1] = malloc(sizeof(VariableObj));
                            *ScopePointer->VariablePtrCarrier.Carrier[ScopePointer->VariablePtrCarrier.CarrierLen-1] =
                                (VariableObj){
                                    .VariableName = ThisCase.ObjName,
                                    .VariablePlace = 0,
                                };

                            *PointerOfScopeVariablePtr =
                                ScopePointer->VariablePtrCarrier.Carrier[ScopePointer->VariablePtrCarrier.CarrierLen-1];
                        }


                        ResultMioneObjCarrier.CarrierLen++;
                        ResultMioneObjCarrier.Carrier = realloc(
                            ResultMioneObjCarrier.Carrier,
                            ResultMioneObjCarrier.CarrierLen * sizeof(MioneObj)
                        );
                        ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1] = (MioneObj){
                            .ObjType = VARIABLE,
                            .PointerOfScopeVariablePtr = PointerOfScopeVariablePtr,
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
                        ResultMioneObjCarrier.CarrierLen++;
                        ResultMioneObjCarrier.Carrier = realloc(
                            ResultMioneObjCarrier.Carrier,
                            ResultMioneObjCarrier.CarrierLen * sizeof(MioneObj)
                        );

                        ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1] = (MioneObj){
                            .ObjType = VALUE,
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

                        ResultMioneObjCarrier.CarrierLen++;
                        ResultMioneObjCarrier.Carrier = realloc(
                            ResultMioneObjCarrier.Carrier,
                            ResultMioneObjCarrier.CarrierLen * sizeof(MioneObj)
                        );

                        for (unsigned i = 0; i < strlen(ThisCase.ObjName); i++)
                            Number += (ThisCase.ObjName[i] != '0') * 1<<(strlen(ThisCase.ObjName) - i - 1);

                        ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1] = (MioneObj){
                            .ObjType = VALUE,
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

                        ResultMioneObjCarrier.CarrierLen++;
                        ResultMioneObjCarrier.Carrier = realloc(
                            ResultMioneObjCarrier.Carrier,
                            ResultMioneObjCarrier.CarrierLen * sizeof(MioneObj)
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

                        ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1] = (MioneObj){
                            .ObjType = VALUE,
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

    return ResultMioneObjCarrier;
}
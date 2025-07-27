//
// Created by calle on 24-12-28.
//

#include <inttypes.h>
#include <tgmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>


#include "OBJECTS.h"
#include "SYMBOL_DEF.h"
#include "PROMPT_DEF.h"
#include "HeadFile/AllHeads.h"
#include "MIONE.h"
#include "ERR.h"
#include "NUMBER.h"


VariableObj * retVarUP(ScopeObj * SVUup,const wchar_t* Name,const int Place)
{
    VariableObj * ret = 0;

    if (!SVUup->VariablePtrCarrier.Carrier) return ret;

    for (int VariableUPIndex = 0;VariableUPIndex < SVUup->VariablePtrCarrier.CarrierLen;VariableUPIndex++)
        if (
          (SVUup->VariablePtrCarrier.Carrier[VariableUPIndex]->VariableName && wcscmp(SVUup->VariablePtrCarrier.Carrier[VariableUPIndex]->VariableName,Name) == 0) ||
          (SVUup->VariablePtrCarrier.Carrier[VariableUPIndex]->VariablePlace != 0 && SVUup->VariablePtrCarrier.Carrier[VariableUPIndex]->VariablePlace == Place)
          )
            ret =SVUup->VariablePtrCarrier.Carrier[VariableUPIndex];

    if (!ret && SVUup->ParentScopePointer) return retVarUP(SVUup->ParentScopePointer,Name,Place);

    return ret;
}


MioneObjCarrier CMO(
    const CaseObjCarrier Carrier
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
                ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1] = (MioneObj){0};

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

                        break;
                    }
                }

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

                        break;
                    }
                }

                break;
            }
        default: //二次處理
            {
                switch (ThisCase.ObjType)
                {
                case CASE_NORMAL: //配對成Variable
                    {
                        ResultMioneObjCarrier.CarrierLen++;
                        ResultMioneObjCarrier.Carrier = realloc(
                            ResultMioneObjCarrier.Carrier,
                            ResultMioneObjCarrier.CarrierLen * sizeof(MioneObj)
                        );
                        ResultMioneObjCarrier.Carrier[ResultMioneObjCarrier.CarrierLen - 1] = (MioneObj){
                            .ObjType = VARIABLE,
                            .MioneObjectPosition = ThisCase.CasePosition,
                        };

                        break;
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
                            .Val = (ValueObj){
                                .String = ThisCase.ObjName,
                                .ValueType = VALUE_STRING_TYPE
                            },
                            .MioneObjectPosition = ThisCase.CasePosition,
                        };

                        break;
                    }
                case CASE_CONNECTABLE: //跳出 之後Error Handle 嘻嘻
                case CASE_UNCONNECTABLE:
                    {
                        exit(-1);
                    }
                default:break;
                }

            };
        }
    }


    return ResultMioneObjCarrier;
}
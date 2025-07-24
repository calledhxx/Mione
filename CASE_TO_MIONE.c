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

        switch (ThisCase.ObjType)
        {
            case CASE_BREAKER:
            {
                printf("dam\n");

                break;
            }
        case CASE_NORMAL:
            {
                for (
                    unsigned int HeadDetectIndex = 0
                    ; Heads[HeadDetectIndex].Identification;
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
        case CASE_CONNECTABLE:
        case CASE_UNCONNECTABLE:
            {
                for (
                    unsigned int PromptDetectIndex = 0
                    ; Prompts[PromptDetectIndex].Identification;
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
                    unsigned int SymbolDetectIndex = 0
                    ; Symbols[SymbolDetectIndex].Identification;
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

            default:break;
        }
    }


    return ResultMioneObjCarrier;
}
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
    CaseObjCarrier Carrier,
    ScopeObj * SVUup)
{
    const CaseObj * CaseCarrier = Carrier.Carrier;
    const unsigned int CaseCarrierLen = Carrier.CarrierLen;

    for (
        unsigned int CaseCarrierIndex = 0;
        CaseCarrierIndex < CaseCarrierLen;
        CaseCarrierIndex++
        )
    {
        const CaseObj ThisCase = CaseCarrier[CaseCarrierIndex];


    }
    //todo 重寫
}
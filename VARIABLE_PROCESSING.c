#include <string.h>

#include "STDMIO.h"

VariableObj * ReturnVariablePtrFromLink(const VariableLinkObj Link)
{
    if (Link.PointToAnotherLink)
        return ReturnVariablePtrFromLink(*Link.PointToAnotherLink);

    if (Link.VariablePtr)
        return Link.VariablePtr;

    return 0;
}

VariableLinkObj * ReturnVariableLinkPtrIfAlreadyExistedInScope(
    const ScopeObj Scope,
    const char * VariableName,
    const int VariableIndex,
    int * inParentScope
)
{
    VariableLinkObj * result = 0;

    for (
        unsigned int Index = 0;
        Index < Scope.VariableLinkPtrCarrier.CarrierLen;
        Index++
        )
    {
        const VariableObj Variable =
            *ReturnVariablePtrFromLink(*Scope.VariableLinkPtrCarrier.Carrier[Index]);

        if (!VariableIndex)
        {
            if (strcmp(Variable.VariableName,VariableName)==0)
                result = Scope.VariableLinkPtrCarrier.Carrier[Index];
        }else
            if (Variable.VariablePlace==VariableIndex)
                result = Scope.VariableLinkPtrCarrier.Carrier[Index];
    }

    if (result)
        return result;

    if (Scope.ParentScopePointer)
    {
        *inParentScope = 1;

        return ReturnVariableLinkPtrIfAlreadyExistedInScope(
            * Scope.ParentScopePointer,
            VariableName,
            VariableIndex,
            inParentScope
            );
    }

    return NULL;
}
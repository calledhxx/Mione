extern VariableObj ** ReturnPointerOfVariablePtrIfAlreadyExistedInScope(
    const ScopeObj Scope,
    const char * VariableName,
    const int VariableIndex,
    int * inParentScope
);

extern  EventObj CMO(
    const CaseObjCarrier Carrier,
    ScopeObj * ScopePointer
);
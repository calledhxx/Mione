extern VariableLinkObj * ReturnVariableLinkPtrIfAlreadyExistedInScope(
    const ScopeObj Scope,
    const char * VariableName,
    const int VariableIndex,
    int * inParentScope
);

extern VariableObj * ReturnVariablePtrFromLink(const VariableLinkObj Link);

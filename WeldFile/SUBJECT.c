#include "../STDMIO.h"

WeldFunctionRespondObj SUBJECT(const WeldFunctionRequestObj * RequestPointer)
{
    const WeldFunctionRequestObj Request = *RequestPointer;
    WeldFunctionRespondObj result = {0};

    const MIONEFunctionRespondObj Train = ToMione((MIONEFunctionRequestObj){
        .EventTemplate = Request.EventTemplate,
        .MioneCarrier = Request.MioneCarrier,
    });

    const IMPLEMENTFunctionRespondObj Implement = IMPLEMENT((IMPLEMENTFunctionRequestObj){
        .EventTemplate = Request.EventTemplate,
        .Built = Train.TrainCarrier,
    });

    result.MioneCarrier.CarrierLen = Implement.MajorVariables.CarrierLen;
    result.MioneCarrier.Carrier = malloc(sizeof(MioneObj)* result.MioneCarrier.CarrierLen);

    for (unsigned i = 0; i < Implement.MajorVariables.CarrierLen; i++)
    {
        VariableLinkObj * VarLinkPtr = malloc(sizeof(VariableLinkObj));
        *VarLinkPtr = (VariableLinkObj){
            .PointToAnotherLink = 0,
            .VariablePtr = &Implement.MajorVariables.Carrier[i] //warning! : plenty unsafe
        };

        result.MioneCarrier.Carrier[i] = (MioneObj){
            .ObjType = VARIABLE,
            .PointerOfScope = 0,
            .MioneObjectPosition = {0},
            .VariableLinkPtr = VarLinkPtr
        };
    }

    return result;
}
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

    FreeTrainCarrier(Train.TrainCarrier);
    FreeVariableCarrier(Implement.MajorVariables);

    result.MioneCarrier.CarrierLen = Implement.Subjects.CarrierLen;
    result.MioneCarrier.Carrier = malloc(sizeof(MioneObj)* result.MioneCarrier.CarrierLen);

    for (unsigned i = 0; i < Implement.Subjects.CarrierLen; i++)
    {
        VariableLinkObj * LinkPtr = malloc(sizeof(VariableLinkObj));
        *LinkPtr = (VariableLinkObj){
            .VariablePtr = Implement.Subjects.Carrier[i],
            .PointToAnotherLink = 0
        }; //假元素
        result.MioneCarrier.Carrier[i] = (MioneObj){
            .ObjType = VARIABLE,
            .PointerOfScope = 0,
            .MioneObjectPosition = {0},
            .VariableLinkPtr = LinkPtr
        };
    }

    free(Implement.Subjects.Carrier);

    return result;
}
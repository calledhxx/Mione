#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#include "STDMIO.h"
#include "HeadFile/SET.h"
#include "HeadFile/SVV.h"

IMPLEMENTFunctionRespondObj IMPLEMENT(const IMPLEMENTFunctionRequestObj input)
{
    IMPLEMENTFunctionRespondObj Obj = {0};
    Obj.Event = input.EventTemplate;

    ValueAndVariableObjCarrier VAVCarrier = {0};

    const unsigned int SectionsSize = input.Built.CarrierLen;
    const TrainObj * Sections = input.Built.Carrier;


    for (unsigned int SectionIndex = 0; SectionIndex < SectionsSize; SectionIndex++)
    {
        const TrainObj thisSection = Sections[SectionIndex];

        if (thisSection.Carriages[0].CarriageManager.Head.Fuc)
        {
            const HeadFunctionRespondObj HeadReturn = thisSection.Carriages[0].CarriageManager.Head.Fuc(
                   &(HeadFunctionRequestObj){
                       .Train = thisSection,

                       .VariablePtrCarrier = (VariableObjPtrCarrier){
                           .Carrier = &Obj.MajorVariables.Carrier,
                           .CarrierLen = Obj.MajorVariables.CarrierLen
                       },

                       .CallByValueCarrier = input.CallByValueCarrier,

                       .EventTemplate = input.EventTemplate
                   });

            MainEventHandler(HeadReturn.Event);

            if (HeadReturn.Subjects.CarrierLen)
            {
                Obj.Subjects.Carrier = realloc(
                    Obj.Subjects.Carrier,
                    sizeof(VariableObj*) * (Obj.Subjects.CarrierLen + HeadReturn.Subjects.CarrierLen)
                );

                memcpy(
                        Obj.Subjects.Carrier + Obj.Subjects.CarrierLen,
                        HeadReturn.Subjects.Carrier,
                        HeadReturn.Subjects.CarrierLen * sizeof(VariableObj*)
                        );

                Obj.Subjects.CarrierLen += HeadReturn.Subjects.CarrierLen;
            }


            if (HeadReturn.MajorVariables.CarrierLen)
            {
                Obj.MajorVariables.Carrier = realloc(
                    Obj.MajorVariables.Carrier,
                    sizeof(VariableObj) * (Obj.MajorVariables.CarrierLen + HeadReturn.MajorVariables.CarrierLen)
                );

                memcpy(
                        Obj.MajorVariables.Carrier + Obj.MajorVariables.CarrierLen,
                        HeadReturn.MajorVariables.Carrier,
                        HeadReturn.MajorVariables.CarrierLen * sizeof(VariableObj)
                        );

                Obj.MajorVariables.CarrierLen += HeadReturn.MajorVariables.CarrierLen;

            }

            if (HeadReturn.ResetVariablesToValues.CarrierLen)
            {
                VAVCarrier.Carrier = realloc(
                              VAVCarrier.Carrier,
                          (VAVCarrier.CarrierLen + HeadReturn.ResetVariablesToValues.CarrierLen) * sizeof(ValueAndVariableObj)
                          );

                    memcpy(VAVCarrier.Carrier + VAVCarrier.CarrierLen,
                           HeadReturn.ResetVariablesToValues.Carrier,
                           HeadReturn.ResetVariablesToValues.CarrierLen * sizeof(ValueAndVariableObj)
                           );

                VAVCarrier.CarrierLen += HeadReturn.ResetVariablesToValues.CarrierLen;
            }


            if (HeadReturn.ReturnValues.CarrierLen)
            {
                Obj.ReturnValues.Carrier = realloc(
                               Obj.ReturnValues.Carrier,
                               (Obj.ReturnValues.CarrierLen + HeadReturn.ReturnValues.CarrierLen) * sizeof(ValueObj)
                               );

                Obj.ReturnValues.Carrier = memcpy(
                    Obj.ReturnValues.Carrier + Obj.ReturnValues.CarrierLen,
                    HeadReturn.ReturnValues.Carrier,
                    HeadReturn.ReturnValues.CarrierLen * sizeof(ValueObj)
                    );

                Obj.ReturnValues.CarrierLen += HeadReturn.ReturnValues.CarrierLen;

                goto end;
            }
        }
    }

    end:

    if (VAVCarrier.CarrierLen)
    {
        for (int i = 0; i < VAVCarrier.CarrierLen; i++)
            VAVCarrier.Carrier[i].VariablePointer->Value = VAVCarrier.Carrier[i].Value;

        free(VAVCarrier.Carrier);
    }

    return Obj;
}

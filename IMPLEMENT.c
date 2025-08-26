#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#include "STDMIO.h"
#include "HeadFile/SET.h"
#include "HeadFile/SVV.h"


IMPLEMENTReturnObj IMPLEMENT(const ToImplementObj toImplement)
{
    IMPLEMENTReturnObj Obj = {0};

    ValueAndVariableObjCarrier VAVCarrier = {0};

    const unsigned int SectionsSize = toImplement.Built.CarrierLen;
    const TrainObj * Sections = toImplement.Built.Carrier;


    for (unsigned int SectionIndex = 0; SectionIndex < SectionsSize; SectionIndex++)
    {
        uint8_t breakSectionCycle = 0;

        const TrainObj thisSection = Sections[SectionIndex];

        if (thisSection.Carriages[0].CarriageManager.Head.Fuc)
        {
            const HeadReturnObj HeadReturn = thisSection.Carriages[0].CarriageManager.Head.Fuc(
                   &(HeadCallObj){
                       .Train = thisSection,

                       .VariablePtrCarrier = (VariableObjPtrCarrier){
                           .Carrier = &Obj.Event.MajorVariables.Carrier,
                           .CarrierLen = Obj.Event.MajorVariables.CarrierLen
                       },

                       .CallByValueCarrier = toImplement.CallByValueCarrier,
                   });


            for (int i = 0; i < 32 ;i++)
            {
                uint8_t breakStateCycle = 0;
                const int cmp = 1<<i;

                if (HeadReturn.Event.ToState & cmp)
                {
                    switch (cmp)
                    {
                    case 0: break;

                    case EVENT_RETURN_VALUES:
                        {
                            Obj.Event.ToState |= 1;

                            Obj.Event.ReturnValues.Carrier = realloc(
                                Obj.Event.ReturnValues.Carrier,
                                (Obj.Event.ReturnValues.CarrierLen + HeadReturn.Event.ReturnValues.CarrierLen) * sizeof(ValueObj)
                                );

                            Obj.Event.ReturnValues.Carrier = memcpy(
                                Obj.Event.ReturnValues.Carrier + Obj.Event.ReturnValues.CarrierLen,
                                HeadReturn.Event.ReturnValues.Carrier,
                                HeadReturn.Event.ReturnValues.CarrierLen * sizeof(ValueObj)
                                );

                            Obj.Event.ReturnValues.CarrierLen += HeadReturn.Event.ReturnValues.CarrierLen;


                            breakStateCycle = 1;

                            break;
                        }
                    case EVENT_RESET_VARIABLE_TO_VALUE:
                        {
                             VAVCarrier.Carrier = realloc(
                                 VAVCarrier.Carrier,
                             (VAVCarrier.CarrierLen + HeadReturn.Event.ResetVariablesToValues.CarrierLen) * sizeof(ValueAndVariableObj)
                             );

                            VAVCarrier.Carrier =
                                memcpy(VAVCarrier.Carrier + VAVCarrier.CarrierLen,
                                       HeadReturn.Event.ResetVariablesToValues.Carrier,
                                       HeadReturn.Event.ResetVariablesToValues.CarrierLen * sizeof(ValueAndVariableObj)
                                       );

                            VAVCarrier.CarrierLen += HeadReturn.Event.ResetVariablesToValues.CarrierLen;

                            break;
                        }
                    case EVENT_MAJOR_VARIABLE:
                        {
                            Obj.Event.ToState |= EVENT_MAJOR_VARIABLE;

                            Obj.Event.MajorVariables.Carrier = realloc(
                                 Obj.Event.MajorVariables.Carrier,
                                 sizeof(VariableObj)*(Obj.Event.MajorVariables.CarrierLen + HeadReturn.Event.MajorVariables.CarrierLen)
                            );

                            Obj.Event.MajorVariables.Carrier =
                                memcpy(
                                    Obj.Event.MajorVariables.Carrier + Obj.Event.MajorVariables.CarrierLen,
                                    HeadReturn.Event.MajorVariables.Carrier,
                                    HeadReturn.Event.MajorVariables.CarrierLen * sizeof(VariableObj)
                                    );

                            Obj.Event.MajorVariables.CarrierLen += HeadReturn.Event.MajorVariables.CarrierLen;

                            break;
                        }

                    default:
                        {
                            exit(-5);
                        }
                    }
                }

                if (breakStateCycle)
                {
                    breakSectionCycle = 1;
                    break;
                }

            }
        }

        if (breakSectionCycle)
            break;
    }

    if (VAVCarrier.CarrierLen)
    {
        for (int i = 0; i < VAVCarrier.CarrierLen; i++)
            VAVCarrier.Carrier[i].VariablePointer->Value = VAVCarrier.Carrier[i].Value;

        free(VAVCarrier.Carrier);
    }


    return Obj;
}

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#include "STDMIO.h"
#include "HeadFile/SET.h"
#include "HeadFile/SVV.h"


EventObj IMPLEMENT(const ToImplementObj toImplement)
{
    EventObj Obj = {0};

    ValueAndVariableObjCarrier VAVCarrier = {0};

    const unsigned int SectionsSize = toImplement.Built.CarrierLen;
    const TrainObj * Sections = toImplement.Built.Carrier;


    for (unsigned int SectionIndex = 0; SectionIndex < SectionsSize; SectionIndex++)
    {
        uint8_t breakSectionCycle = 0;

        const TrainObj thisSection = Sections[SectionIndex];

        if (thisSection.Carriages[0].CarriageManager.Head.Fuc)
        {
            const EventObj HeadReturn = thisSection.Carriages[0].CarriageManager.Head.Fuc(
                   &(HeadCallObj){
                       .Train = thisSection,

                       .VariablePtrCarrier = (VariableObjPtrCarrier){
                           .Carrier = &Obj.MajorVariables.Carrier,
                           .CarrierLen = Obj.MajorVariables.CarrierLen
                       },

                       .CallByValueCarrier = toImplement.CallByValueCarrier,
                   });


            for (int i = 0; i < 32 ;i++)
            {
                uint8_t breakStateCycle = 0;
                const int cmp = 1<<i;

                if (HeadReturn.ToState & cmp)
                {
                    switch (cmp)
                    {
                    case 0: break;

                    case EVENT_RETURN_VALUES:
                        {
                            Obj.ToState |= 1;

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


                            breakStateCycle = 1;

                            break;
                        }
                    case EVENT_RESET_VARIABLE_TO_VALUE:
                        {
                             VAVCarrier.Carrier = realloc(
                                 VAVCarrier.Carrier,
                             (VAVCarrier.CarrierLen + HeadReturn.ResetVariablesToValues.CarrierLen) * sizeof(ValueAndVariableObj)
                             );

                            VAVCarrier.Carrier =
                                memcpy(VAVCarrier.Carrier + VAVCarrier.CarrierLen,
                                       HeadReturn.ResetVariablesToValues.Carrier,
                                       HeadReturn.ResetVariablesToValues.CarrierLen * sizeof(ValueAndVariableObj)
                                       );

                            VAVCarrier.CarrierLen += HeadReturn.ResetVariablesToValues.CarrierLen;

                            break;
                        }
                    case EVENT_MAJOR_VARIABLE:
                        {
                            Obj.ToState |= EVENT_MAJOR_VARIABLE;

                            Obj.MajorVariables.Carrier = realloc(
                                 Obj.MajorVariables.Carrier,
                                 sizeof(VariableObj)*(Obj.MajorVariables.CarrierLen + HeadReturn.MajorVariables.CarrierLen)
                            );

                            Obj.MajorVariables.Carrier =
                                memcpy(
                                    Obj.MajorVariables.Carrier + Obj.MajorVariables.CarrierLen,
                                    HeadReturn.MajorVariables.Carrier,
                                    HeadReturn.MajorVariables.CarrierLen * sizeof(VariableObj)
                                    );

                            Obj.MajorVariables.CarrierLen += HeadReturn.MajorVariables.CarrierLen;

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

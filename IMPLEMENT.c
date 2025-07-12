#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "HeadFile/SVV.h"


ImplementedObj IMPLEMENT(const ToImplementObj toImplement)
{
    ImplementedObj Obj = {0};
    Obj.ToState=0;
    Obj.VariableCarrier.Carrier = NULL;
    Obj.VariableCarrier.CarrierLen = 0;

    ValueAndVariableObjCarrier VAVCarrier;
    VAVCarrier.CarrierLen = 0;
    VAVCarrier.Carrier = NULL;

    const unsigned int SectionsSize = toImplement.Built.CarrierLen;
    const MioneSectionObj * Sections = toImplement.Built.Carrier;

    for (unsigned int SectionIndex = 0; SectionIndex < SectionsSize; SectionIndex++)
    {
        uint8_t breakSectionCycle = 0;

        const MioneSectionObj thisSection = Sections[SectionIndex];

        if (thisSection.HeadAction.Head.Fuc)
        {
            const PairObjCarrier PairCarrier = {
                .Carrier = thisSection.Pairs,
                .CarrierLen = thisSection.PairsSize
            };

            const HeadReturnObj HeadReturn = thisSection.HeadAction.Head.Fuc(
                   &(HeadCallObj){
                       .PairCarrier = (PairObjCarrier){
                           .Carrier = PairCarrier.Carrier,
                           .CarrierLen = PairCarrier.CarrierLen,
                       },

                       .VariablePtrCarrier = (VariableObjPtrCarrier){
                           .Carrier = &Obj.VariableCarrier.Carrier,
                           .CarrierLen = Obj.VariableCarrier.CarrierLen
                       },

                       .CallByValueCarrier = toImplement.CallByValueCarrier,
                   });


            for (int i = 0; HeadReturn.ToState - 1<<i > 1<<i ;i++)
            {
                uint8_t breakStateCycle = 0;
                const int cmp = 1<<i;

                if (HeadReturn.ToState & cmp)
                {
                    switch (cmp)
                    {
                    case 0: break;

                    case 1:
                        {
                            Obj.ToState |= 1;

                            Obj.ValueCarrier.Carrier = memcpy(
                                Obj.ValueCarrier.Carrier + Obj.ValueCarrier.CarrierLen,
                                HeadReturn.ValueCarrier.Carrier,
                                HeadReturn.ValueCarrier.CarrierLen * sizeof(ValueObj)
                                );

                            Obj.ValueCarrier.CarrierLen += HeadReturn.ValueCarrier.CarrierLen;

                            breakStateCycle = 1;

                            //its doesnt need to be memories copy,AH whatever
                            break;
                        }
                    case 2:
                        {
                            VAVCarrier.Carrier =
                                memcpy(VAVCarrier.Carrier + VAVCarrier.CarrierLen,
                                       HeadReturn.ValueAndVariableCarrier.Carrier,
                                       HeadReturn.ValueAndVariableCarrier.CarrierLen * sizeof(ValueAndVariableObj)
                                       );

                            VAVCarrier.CarrierLen += HeadReturn.ValueAndVariableCarrier.CarrierLen;

                            break;
                        }
                    case 4:
                        {
                            Obj.ToState |= 2;

                            Obj.VariableCarrier.Carrier =
                                memcpy(
                                    Obj.VariableCarrier.Carrier + Obj.VariableCarrier.CarrierLen,
                                    HeadReturn.VariableCarrier.Carrier,
                                    HeadReturn.VariableCarrier.CarrierLen * sizeof(VariableObj)
                                    );

                            Obj.VariableCarrier.CarrierLen += HeadReturn.VariableCarrier.CarrierLen;

                            break;
                        }

                    default:
                        {
                            exit(-3);
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

    for (int i = 0; i < VAVCarrier.CarrierLen; i++)
        VAVCarrier.Carrier[i].VariablePointer->Value = VAVCarrier.Carrier[i].Value;


    return Obj;
}

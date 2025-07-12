#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "HeadFile/SVV.h"


ImplementedObj IMPLEMENT(const ToImplementObj toImplement)
{

    ImplementedObj Obj = {0};
    Obj.ToState=0;

    VariableObjCarrier VariableCarrier;
    VariableCarrier.Carrier = NULL;
    VariableCarrier.CarrierLen = 0;

    ValueAndVariableObjCarrier VAVCarrier;
    VAVCarrier.CarrierLen = 0;
    VAVCarrier.Carrier = NULL;

    unsigned int SectionsSize = toImplement.Built.CarrierLen;
    MioneSectionObj * Sections = toImplement.Built.Carrier;

    for (int SectionIndex = 0; SectionIndex < SectionsSize; SectionIndex++)
    {
        MioneSectionObj thisSection = Sections[SectionIndex];

        if (thisSection.HeadAction.Head.Fuc)
        {
            PairObjCarrier PairCarrier = {
                .Carrier = thisSection.Pairs,
                .CarrierLen = thisSection.PairsSize
            };

            HeadReturnObj HeadReturn = thisSection.HeadAction.Head.Fuc(
                   &(HeadCallObj){
                       .PairCarrier = (PairObjCarrier){
                           .Carrier = PairCarrier.Carrier,
                           .CarrierLen = PairCarrier.CarrierLen,
                       },

                       .VariablePtrCarrier = (VariableObjPtrCarrier){
                           .Carrier = &VariableCarrier.Carrier,
                           .CarrierLen = VariableCarrier.CarrierLen
                       },

                       .CallByValueCarrier = toImplement.CallByValueCarrier,
                   });


            for (int i = 0;HeadReturn.ToState > 1<<i;i++)
            {
                const int cmp = 1<<i;

                if (!HeadReturn.ToState) break;

                if (HeadReturn.ToState & cmp)
                {
                    switch (cmp)
                    {
                    case 0: break;

                    case 1:
                        {
                            Obj.ToState|=1;
                            Obj.ValueCarrier = HeadReturn.ValueCarrier;
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
                            VariableCarrier.Carrier =
                                memcpy(
                                    VariableCarrier.Carrier + VariableCarrier.CarrierLen,
                                    HeadReturn.VariableCarrier.Carrier,
                                    HeadReturn.VariableCarrier.CarrierLen * sizeof(VariableObj)
                                    );

                            VariableCarrier.CarrierLen += HeadReturn.VariableCarrier.CarrierLen;

                            break;
                        }

                    default:
                        {
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < VAVCarrier.CarrierLen; i++)
        VAVCarrier.Carrier[i].VariablePointer->Value = VAVCarrier.Carrier[i].Value;

    if (VariableCarrier.CarrierLen)
    {
        Obj.ToState |= 2;
        Obj.VariableCarrier = VariableCarrier;
    }

    return Obj;
}

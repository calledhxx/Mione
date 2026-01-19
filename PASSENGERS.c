#include "STDMIO.h"
#include "WeldFile/SUBJECT.h"

PASSENGERRespondObj PASSENGERS(const PASSENGERRequestObj In)
{
    PASSENGERRespondObj result = {0};
    result.Event = In.EventTemplate;

    const PassengerObjCarrier OriginalObjects = In.PassenegrsCarrier;

    result.MioneCarrier.CarrierLen = OriginalObjects.CarrierLen;
    result.MioneCarrier.Carrier = malloc(sizeof(MioneObj)*result.MioneCarrier.CarrierLen);


    for (unsigned index = 0;index<OriginalObjects.CarrierLen;index++)
    {
        if (OriginalObjects.Carrier[index].IsIndirect)
        {
            const WeldFunctionRespondObj Welded = SUBJECT(&(WeldFunctionRequestObj){
                .MioneCarrier = OriginalObjects.Carrier[index].Indirect,
                .EventTemplate = (EventObj){0}
            });

            free(OriginalObjects.Carrier[index].Indirect.Carrier);

            if (Welded.MioneCarrier.CarrierLen)
            {
                if (Welded.MioneCarrier.CarrierLen != 1)
                    exit(3312);

                result.MioneCarrier.Carrier[index] = Welded.MioneCarrier.Carrier[0];
            }else
                result.MioneCarrier.Carrier[index] = (MioneObj){
                    .ObjType = VALUE,
                    .Value = (ValueObj){0}
                };
        }else
            result.MioneCarrier.Carrier[index] = OriginalObjects.Carrier[index].Direct;

    }


    return result;
}

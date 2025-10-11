#include "OBJECTS.h"

void FreeValueByTheirCarrier()
{

}

void FreeVariableByTheirCarrier(const VariableObjCarrier Carrier)
{
    //free name ,value of each variable

    for (unsigned i = 0; i < Carrier.CarrierLen;i++)
    {
        free(Carrier.Carrier[i].VariableName);

        switch (Carrier.Carrier[i].Value.ValueType)
        {
        case VALUE_FUNCTION_TYPE:
        case VALUE_RANGE_TYPE:
            {
                const AreaObj Area = Carrier.Carrier[i].Value.Area;
                for (
                    unsigned a = 0;
                    a<Area.TrainObjCarrierPointer->CarrierLen;
                    a++
                    )
                {
                    for (
                        unsigned b = 0;
                        b<Area.TrainObjCarrierPointer->Carrier[a].CarriageLen;
                        b++
                    )
                    {
                        for (
                            unsigned c = 0;
                            c<Area.TrainObjCarrierPointer->Carrier[a].Carriages[b].CarriagePassengersCarrier.CarrierLen;
                            c++
                        )
                            if (Area.TrainObjCarrierPointer->Carrier[a].Carriages[b].CarriagePassengersCarrier.Carrier[c].IsIndirect)
                                free(
                                    Area.TrainObjCarrierPointer->Carrier[a].Carriages[b].CarriagePassengersCarrier.Carrier[c].Indirect.Carrier
                                );

                        free(Area.TrainObjCarrierPointer->Carrier[a].Carriages[b].CarriagePassengersCarrier.Carrier);
                    }
                    free(Area.TrainObjCarrierPointer->Carrier[a].Carriages);
                }
                free(Area.TrainObjCarrierPointer->Carrier);
                free(Area.TrainObjCarrierPointer);
                break;
            }
        case VALUE_STRING_TYPE:
        case VALUE_TABLE_TYPE:
            {
                break;
            }
        default: break;
        }
    }
}
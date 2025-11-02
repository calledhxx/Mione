#include "OBJECTS.h"

void FreeValueCarrier()
{

}

void FreeVariableLinkMioneObjectCarrierOfWeldFunctionRespondObj(const MioneObjCarrier Carrier)
{
    printf("bbb %d\n",Carrier.CarrierLen);
    for (unsigned i = 0; i < Carrier.CarrierLen; i++)
    {
        printf("%d %d\n",Carrier.Carrier[i].ObjType,VARIABLE);
        if (Carrier.Carrier[i].ObjType == VARIABLE && Carrier.Carrier[i].PointerOfScope == 0)
        {
            printf ("fre 1 %p\n",Carrier.Carrier[i].VariableLinkPtr->VariablePtr);
            printf ("fre 2 %p\n",Carrier.Carrier[i].VariableLinkPtr);
            free(Carrier.Carrier[i].VariableLinkPtr->VariablePtr);
            free(Carrier.Carrier[i].VariableLinkPtr);
        }
    }

    printf("cccc %p\n",Carrier.Carrier);

    Carrier.Carrier[0] = (MioneObj){0};
    free(Carrier.Carrier);
}

void FreeTrainCarrier(const TrainObjCarrier Carrier)
{
    for (int i = 0; i < Carrier.CarrierLen; i++)
    {
        for (int j = 0; j < Carrier.Carrier[i].CarriageLen; j++)
        {
            for (int k = 0; k < Carrier.Carrier[i].Carriages[j].CarriagePassengersCarrier.CarrierLen; k++)
                if (Carrier.Carrier[i].Carriages[j].CarriagePassengersCarrier.Carrier[k].IsIndirect)
                    free(Carrier.Carrier[i].Carriages[j].CarriagePassengersCarrier.Carrier[k].Indirect.Carrier);
            free(Carrier.Carrier[i].Carriages[j].CarriagePassengersCarrier.Carrier);
        }
        free(Carrier.Carrier[i].Carriages);
    }

    free(Carrier.Carrier);
}

void FreeVariableCarrier(const VariableObjCarrier Carrier)
{
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

    free(Carrier.Carrier);
}
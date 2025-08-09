//
// Created by calle on 24-12-28.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "STDMIO.h"


static void ResetCarriage(CarriageObj* CarriagePointer)
{
    CarriagePointer->CarriageManager.ObjType = 0;
    CarriagePointer->CarriagePassengers.Carrier = NULL;
    CarriagePointer->CarriagePassengers.CarrierLen = 0;
}


static void ResetTrain(TrainObj* TrainPointer)
{
    TrainPointer->CarriageLen = 0;
    TrainPointer->Carriages = NULL;
}

static void SaveCarriageIntoTrain(
    TrainObj*TrainPointer,
    const CarriageObj Carriage
    )
{
    if (Carriage.CarriageManager.ObjType == 0 && Carriage.CarriagePassengers.CarrierLen == 0) return;

    TrainPointer->CarriageLen++;
    TrainPointer->Carriages = realloc(
        TrainPointer->Carriages,
        sizeof(CarriageObj) * TrainPointer->CarriageLen
        );
    TrainPointer->Carriages[TrainPointer->CarriageLen - 1] =
        Carriage;
}

static void SaveTrainIntoCarrier(
    TrainObjCarrier*CarrierPointer,
    const TrainObj Train
    )
{
    if (Train.CarriageLen == 0) return;

    CarrierPointer->CarrierLen++;
    CarrierPointer->Carrier = realloc(
        CarrierPointer->Carrier,
        sizeof(TrainObj) * CarrierPointer->CarrierLen
        );
    CarrierPointer->Carrier[CarrierPointer->CarrierLen - 1] =
        Train;
}

static void SavePassengerIntoCarriage(
    CarriageObj* CarriagePointer,
    const MioneObj Passenger
    )
{
    CarriagePointer->CarriagePassengers.CarrierLen++;
    CarriagePointer->CarriagePassengers.Carrier = realloc(
        CarriagePointer->CarriagePassengers.Carrier,
        sizeof(MioneObj) * CarriagePointer->CarriagePassengers.CarrierLen
    );
    CarriagePointer->CarriagePassengers.Carrier[CarriagePointer->CarriagePassengers.CarrierLen - 1] =
        Passenger;
}

TrainObjCarrier ToMione(const MioneObjCarrier ToBuildObj)
{
    const unsigned int ObjsSize = ToBuildObj.CarrierLen;
    const MioneObj* Objs = ToBuildObj.Carrier;

    TrainObjCarrier BuiltObj = {0};

    BuiltObj.CarrierLen = 0;
    BuiltObj.Carrier = NULL;

    TrainObj Train;
    ResetTrain(&Train);

    CarriageObj Carriage;
    ResetCarriage(&Carriage);

    for (int index = 0;index < ObjsSize;index++)
    {
        const MioneObj Mio = Objs[index];

        switch (Mio.ObjType)
        {
        case HEAD:
            {
                SaveCarriageIntoTrain(&Train,Carriage);
                SaveTrainIntoCarrier(&BuiltObj,Train);
                ResetCarriage(&Carriage);
                ResetTrain(&Train);

                Carriage.CarriageManager = Mio;

                break;
            }
        case PROMPT:
            {
                SaveCarriageIntoTrain(&Train,Carriage);
                ResetCarriage(&Carriage);

                Carriage.CarriageManager = Mio;

                break;
            }
        case SYMBOL:
        case VARIABLE:
        case VALUE:
            {
                SavePassengerIntoCarriage(&Carriage,Mio);

                break;
            }
        default:
            {
                SaveCarriageIntoTrain(&Train,Carriage);
                SaveTrainIntoCarrier(&BuiltObj,Train);
                ResetCarriage(&Carriage);
                ResetTrain(&Train);
            };
        }
    }

    SaveCarriageIntoTrain(&Train,Carriage);
    SaveTrainIntoCarrier(&BuiltObj,Train);

    return BuiltObj;
}
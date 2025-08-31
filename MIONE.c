//
// Created by calle on 24-12-28.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "STDMIO.h"
#include "HeadFile/SVV.h"


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

MIONEFunctionRespondObj ToMione(MIONEFunctionRequestObj input)
{
    MioneObjCarrier ToBuildObj = input.MioneCarrier;
    MIONEFunctionRespondObj Result = {0};
    Result.Event = input.EventTemplate;

    const unsigned int ObjsSize = ToBuildObj.CarrierLen;
    const MioneObj* Objs = ToBuildObj.Carrier;

    TrainObjCarrier BuiltObj = {0};

    BuiltObj.CarrierLen = 0;
    BuiltObj.Carrier = NULL;

    TrainObj Train;
    ResetTrain(&Train);

    CarriageObj Carriage;
    ResetCarriage(&Carriage);

    MioneObj LastMio = {0};

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
            {
                if (LastMio.ObjType == SYMBOL) //`)` +
                {
                    if (!(LastMio.Symbol.Identification == 10 || LastMio.Symbol.Identification == 11 || LastMio.Symbol.Identification == 12 || LastMio.Symbol.Identification == 13))
                        if (!(Mio.Symbol.SymbolCarry & SC_AfterSymbol || LastMio.Symbol.SymbolCarry & SC_BeforeSymbol))
                        {
                            SaveCarriageIntoTrain(&Train,Carriage);
                            SaveTrainIntoCarrier(&BuiltObj,Train);
                            ResetCarriage(&Carriage);
                            ResetTrain(&Train);


                            Carriage.CarriageManager = (MioneObj){
                                .ObjType = Mio.ObjType,
                                .Head = (HeadObj){
                                    .Fuc = SVV
                                }
                            };
                        }
                }else if (LastMio.ObjType == VARIABLE || LastMio.ObjType == VALUE) //VARIABLE || VALUE
                    if (!(Mio.Symbol.Identification == 10 || Mio.Symbol.Identification == 11 || Mio.Symbol.Identification == 12 || Mio.Symbol.Identification == 13))
                        if (!(Mio.Symbol.SymbolCarry & SC_AfterVariableOrValue))
                        {
                            SaveCarriageIntoTrain(&Train,Carriage);
                            SaveTrainIntoCarrier(&BuiltObj,Train);
                            ResetCarriage(&Carriage);
                            ResetTrain(&Train);

                            Carriage.CarriageManager = (MioneObj){
                                .ObjType = Mio.ObjType,
                                .Head = (HeadObj){
                                    .Fuc = SVV
                                }
                            };
                        }
            }

        case VARIABLE:
        case VALUE:
            {
                if (Mio.ObjType == VARIABLE || Mio.ObjType == VALUE)
                    if (LastMio.ObjType == SYMBOL)
                    {
                        if (!(LastMio.Symbol.SymbolCarry & SC_BeforeVariableOrValue))
                        {
                            SaveCarriageIntoTrain(&Train,Carriage);
                            SaveTrainIntoCarrier(&BuiltObj,Train);
                            ResetCarriage(&Carriage);
                            ResetTrain(&Train);

                            Carriage.CarriageManager = (MioneObj){
                                .ObjType = Mio.ObjType,
                                .Head = (HeadObj){
                                    .Fuc = SVV
                                }
                            };
                        }
                    }else if (LastMio.ObjType == VARIABLE || LastMio.ObjType == VALUE)
                    {
                        SaveCarriageIntoTrain(&Train,Carriage);
                        SaveTrainIntoCarrier(&BuiltObj,Train);
                        ResetCarriage(&Carriage);
                        ResetTrain(&Train);


                        Carriage.CarriageManager = (MioneObj){
                            .ObjType = Mio.ObjType,
                            .Head = (HeadObj){
                                .Fuc = SVV
                            }
                        };
                    }

                if (!Carriage.CarriageManager.ObjType)
                    Carriage.CarriageManager = (MioneObj){
                        .ObjType = Mio.ObjType,
                        .Head = (HeadObj){
                            .Fuc = SVV
                        }
                    };

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

        LastMio = Mio;
    }

    SaveCarriageIntoTrain(&Train,Carriage);
    SaveTrainIntoCarrier(&BuiltObj,Train);

    Result.TrainCarrier = BuiltObj;

    return Result;
}
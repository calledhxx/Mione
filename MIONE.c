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
    CarriagePointer->CarriagePassengersCarrier.Carrier = NULL;
    CarriagePointer->CarriagePassengersCarrier.CarrierLen = 0;
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
    if (Carriage.CarriageManager.ObjType == 0 && Carriage.CarriagePassengersCarrier.CarrierLen == 0) return;

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
    const PassengerObj Passenger
    )
{
    CarriagePointer->CarriagePassengersCarrier.CarrierLen++;
    CarriagePointer->CarriagePassengersCarrier.Carrier = realloc(
        CarriagePointer->CarriagePassengersCarrier.Carrier,
        sizeof(PassengerObj) * CarriagePointer->CarriagePassengersCarrier.CarrierLen
    );
    CarriagePointer->CarriagePassengersCarrier.Carrier[CarriagePointer->CarriagePassengersCarrier.CarrierLen - 1] =
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

    unsigned WeldLayoutCount = 0;
    unsigned OutestWeldIndex = -1;

    MioneObjCarrier MioneCarrier = {0};

    for (signed index = 0;index < ObjsSize;index++)
    {
        const MioneObj Mio = Objs[index];


        switch (Mio.ObjType)
        {
        case HEAD:
            {
                //end train
                if (WeldLayoutCount)
                {
                    if (index - OutestWeldIndex > 1)
                    {
                        WeldLayoutCount--;
                        if (!WeldLayoutCount)
                        {
                            MioneCarrier.CarrierLen = index - OutestWeldIndex;
                            MioneCarrier.Carrier = malloc(sizeof(MioneObj) * MioneCarrier.CarrierLen);

                            memcpy(MioneCarrier.Carrier,Objs + OutestWeldIndex + 1,sizeof(MioneObj) * MioneCarrier.CarrierLen);

                            SavePassengerIntoCarriage(&Carriage,(PassengerObj){
                                .IsIndirect = 1,
                                .Indirect = MioneCarrier
                            });
                        }
                    }
                }

                if (!WeldLayoutCount)
                {
                    SaveCarriageIntoTrain(&Train,Carriage);
                    SaveTrainIntoCarrier(&BuiltObj,Train);
                    ResetCarriage(&Carriage);
                    ResetTrain(&Train);

                    Carriage.CarriageManager = Mio;
                }


                break;
            }
        case PROMPT:
            {
                //end carriage
                if (!WeldLayoutCount)
                {
                    SaveCarriageIntoTrain(&Train,Carriage);
                    ResetCarriage(&Carriage);

                    Carriage.CarriageManager = Mio;
                }

                break;
            }
        case SYMBOL:
            {
                if (LastMio.ObjType == SYMBOL) //`)` +
                {
                    if (!(LastMio.Symbol.Identification == SYMBOL_FRONT_BRACKET || LastMio.Symbol.Identification == SYMBOL_BACK_BRACKET || LastMio.Symbol.Identification == SYMBOL_FRONT_PARENTHESES || LastMio.Symbol.Identification == SYMBOL_BACK_PARENTHESES))
                        if (!(Mio.Symbol.SymbolCarry & SC_AfterSymbol || LastMio.Symbol.SymbolCarry & SC_BeforeSymbol))
                        {
                            //end train
                            if (WeldLayoutCount)
                            {
                                WeldLayoutCount--;
                                if (!WeldLayoutCount)
                                {
                                    MioneCarrier.CarrierLen = index - OutestWeldIndex;
                                    MioneCarrier.Carrier = malloc(sizeof(MioneObj) * MioneCarrier.CarrierLen);

                                    memcpy(MioneCarrier.Carrier,Objs + OutestWeldIndex + 1,sizeof(MioneObj) * MioneCarrier.CarrierLen);
                                

                                    SavePassengerIntoCarriage(&Carriage,(PassengerObj){
                                        .IsIndirect = 1,
                                        .Indirect = MioneCarrier
                                    });
                                }
                            }

                            if (!WeldLayoutCount)
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
                }else if (LastMio.ObjType == VARIABLE || LastMio.ObjType == VALUE) //VARIABLE || VALUE
                {
                    if (!(Mio.Symbol.Identification == SYMBOL_FRONT_BRACKET || Mio.Symbol.Identification == SYMBOL_BACK_BRACKET || Mio.Symbol.Identification == SYMBOL_FRONT_PARENTHESES || Mio.Symbol.Identification == SYMBOL_BACK_PARENTHESES))
                        if (!(Mio.Symbol.SymbolCarry & SC_AfterVariableOrValue))
                        {
                            //end train
                            if (WeldLayoutCount)
                            {
                                WeldLayoutCount--;
                                if (!WeldLayoutCount)
                                {
                                    MioneCarrier.CarrierLen = index - OutestWeldIndex;
                                    MioneCarrier.Carrier = malloc(sizeof(MioneObj) * MioneCarrier.CarrierLen);

                                    memcpy(MioneCarrier.Carrier,Objs + OutestWeldIndex + 1,sizeof(MioneObj) * MioneCarrier.CarrierLen);
                                

                                    SavePassengerIntoCarriage(&Carriage,(PassengerObj){
                                        .IsIndirect = 1,
                                        .Indirect = MioneCarrier
                                    });
                                }
                            }

                            if (!WeldLayoutCount)
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
                            //end train
                            if (WeldLayoutCount)
                            {
                                WeldLayoutCount--;
                                if (!WeldLayoutCount)
                                {
                                    MioneCarrier.CarrierLen = index - OutestWeldIndex;
                                    MioneCarrier.Carrier = malloc(sizeof(MioneObj) * MioneCarrier.CarrierLen);

                                    memcpy(MioneCarrier.Carrier,Objs + OutestWeldIndex + 1,sizeof(MioneObj) * MioneCarrier.CarrierLen);


                                    SavePassengerIntoCarriage(&Carriage,(PassengerObj){
                                        .IsIndirect = 1,
                                        .Indirect = MioneCarrier
                                    });
                                }
                            }

                            if (!WeldLayoutCount)
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
                    }else if (LastMio.ObjType == VARIABLE || LastMio.ObjType == VALUE)
                    {
                        //end train
                        if (WeldLayoutCount)
                        {
                            WeldLayoutCount--;
                            if (!WeldLayoutCount)
                            {
                                MioneCarrier.CarrierLen = index - OutestWeldIndex;
                                MioneCarrier.Carrier = malloc(sizeof(MioneObj) * MioneCarrier.CarrierLen);

                                memcpy(MioneCarrier.Carrier,Objs + OutestWeldIndex + 1,sizeof(MioneObj) * MioneCarrier.CarrierLen);


                                SavePassengerIntoCarriage(&Carriage,(PassengerObj){
                                        .IsIndirect = 1,
                                        .Indirect = MioneCarrier
                                    });
                            }
                        }

                        if (!WeldLayoutCount)
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

                if (!WeldLayoutCount)
                {
                    if (!Carriage.CarriageManager.ObjType)
                        Carriage.CarriageManager = (MioneObj){
                            .ObjType = Mio.ObjType,
                            .Head = (HeadObj){
                                .Fuc = SVV
                            }
                        };

                    SavePassengerIntoCarriage(&Carriage,(PassengerObj){
                                        .IsIndirect = 0,
                                        .Direct = Mio
                                    });
                }

                break;
            }
        case WELD:
            {
                if (!WeldLayoutCount)
                {
                    OutestWeldIndex = index;
                }
                WeldLayoutCount++;

                break;
            }
        default:
            {

                //end train
                if (WeldLayoutCount)
                {
                    WeldLayoutCount--;
                    if (!WeldLayoutCount)
                    {
                        MioneCarrier.CarrierLen = index - OutestWeldIndex;
                        MioneCarrier.Carrier = malloc(sizeof(MioneObj) * MioneCarrier.CarrierLen);

                        memcpy(MioneCarrier.Carrier,Objs + OutestWeldIndex + 1,sizeof(MioneObj) * MioneCarrier.CarrierLen);
                                

                        SavePassengerIntoCarriage(&Carriage,(PassengerObj){
                                        .IsIndirect = 1,
                                        .Indirect = MioneCarrier
                                    });
                    }
                }

                if (!WeldLayoutCount)
                {
                    SaveCarriageIntoTrain(&Train,Carriage);
                    SaveTrainIntoCarrier(&BuiltObj,Train);
                    ResetCarriage(&Carriage);
                    ResetTrain(&Train);
                }
            };
        }

        LastMio = Mio;
    }

    if (WeldLayoutCount)
    {
        MioneCarrier.CarrierLen = ObjsSize - 1 - OutestWeldIndex;
        MioneCarrier.Carrier = malloc(sizeof(MioneObj) * MioneCarrier.CarrierLen);

        memcpy(MioneCarrier.Carrier,Objs + OutestWeldIndex + 1,sizeof(MioneObj) * MioneCarrier.CarrierLen);
                                

        SavePassengerIntoCarriage(&Carriage,(PassengerObj){
                                        .IsIndirect = 1,
                                        .Indirect = MioneCarrier
                                    });
    }

    SaveCarriageIntoTrain(&Train,Carriage);
    SaveTrainIntoCarrier(&BuiltObj,Train);

    Result.TrainCarrier = BuiltObj;

    return Result;
}
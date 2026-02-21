#include <stdlib.h>

#include "train.h"

#include "symbol.h"

void pushTrainIntoTrainCarrier(train_carrier_t * const TrainCarrierPtr,const train_t train)
{
    if (!train.train_type)
        return;

    TrainCarrierPtr->trains_length++;
    TrainCarrierPtr->trains=realloc(TrainCarrierPtr->trains,sizeof(train_t)*TrainCarrierPtr->trains_length);
    TrainCarrierPtr->trains[TrainCarrierPtr->trains_length - 1] = train;
}

void pushCarriageIntoTrain(train_t * const trainPtr,const carriage_t carriage)
{
    if (!carriage.carriage_type)
        return;

    trainPtr->carriage_carrier.carriages_length++;
    trainPtr->carriage_carrier.carriages = realloc(
        trainPtr->carriage_carrier.carriages,
        sizeof(carriage_t) * trainPtr->carriage_carrier.carriages_length
        );
    trainPtr->carriage_carrier.carriages[trainPtr->carriage_carrier.carriages_length-1]=
        carriage;
}

void pushPassengerIntoCarriage(carriage_t * const carriagePtr,const object_t passenger)
{
    if (!(passenger.token || passenger.object_type))
        return;

    carriagePtr->passengers.objects_length++;
    carriagePtr->passengers.objects = realloc(
        carriagePtr->passengers.objects,
        sizeof(object_t) * carriagePtr->passengers.objects_length
        );
    carriagePtr->passengers.objects[carriagePtr->passengers.objects_length - 1] =
        passenger;
}

void endTrain(train_carrier_t * const TrainCarrierPtr,train_t * const trainPtr)
{
    pushTrainIntoTrainCarrier(TrainCarrierPtr,*trainPtr);
    *trainPtr = (train_t){0};
}

void endCarriage(train_t * const trainPtr, carriage_t * const carriagePtr)
{
    pushCarriageIntoTrain(trainPtr,*carriagePtr);
    *carriagePtr = (carriage_t){0};
}

train_carrier_t object_to_train(object_carrier_t object_carrier)
{
    train_carrier_t train_carrier = {0};

    train_t train = {0};

    carriage_t carriage = {0};

    object_t LastObject = {0};

    for (unsigned i = 0;i<object_carrier.objects_length;i++)
    {
        object_t const ThisObject = object_carrier.objects[i];

        switch (ThisObject.object_type)
        {
        case OBJECT_HEAD:
            {
                endCarriage(&train, &carriage);
                endTrain(&train_carrier, &train);

                carriage.carriage_type = CARRIAGE_HEAD;
                carriage.conductor = ThisObject.token;

                break;
            }
        case OBJECT_PROMPT:
            {
                endCarriage(&train, &carriage);

                carriage.carriage_type = CARRIAGE_BODY;
                carriage.conductor = ThisObject.token;

                break;
            }
        case OBJECT_SYMBOL:
            {
                symbol_t const symbol = token_to_symbol(ThisObject.token);

                if (LastObject.object_type == OBJECT_VALUE || LastObject.object_type == OBJECT_VARIABLE)
                    if (!(symbol.connect_condition_flag & SYMBOL_CONNECT_CONDITION_FLAG_AFTER_VV))
                    {
                        endCarriage(&train, &carriage);
                        endTrain(&train_carrier, &train);

                        carriage.carriage_type = CARRIAGE_HEAD;
                        carriage.conductor = 0; //simple train
                    }

                if (LastObject.object_type == OBJECT_SYMBOL)
                    if (!(symbol.connect_condition_flag & SYMBOL_CONNECT_CONDITION_FLAG_AFTER_SYMBOL))
                        if (token_to_symbol(LastObject.token).connect_condition_flag & SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_SYMBOL)
                        {
                            if (!(symbol.connect_condition_flag & SYMBOL_CONNECT_CONDITION_FLAG_MODESTY))
                            {
                                endCarriage(&train, &carriage);
                                endTrain(&train_carrier, &train);

                                carriage.carriage_type = CARRIAGE_HEAD;
                                carriage.conductor = 0; //simple train
                            }
                        }else
                        {
                            endCarriage(&train, &carriage);
                            endTrain(&train_carrier, &train);

                            carriage.carriage_type = CARRIAGE_HEAD;
                            carriage.conductor = 0; //simple train
                        }

                pushPassengerIntoCarriage(&carriage,ThisObject);

                break;
            }
        case OBJECT_VARIABLE:
        case OBJECT_VALUE:
            {
                if (LastObject.object_type == OBJECT_SYMBOL)
                {
                    symbol_t const symbol = token_to_symbol(LastObject.token);

                    if (!(symbol.connect_condition_flag & SYMBOL_CONNECT_CONDITION_FLAG_BEFORE_VV))
                    {
                        endCarriage(&train, &carriage);
                        endTrain(&train_carrier, &train);

                        carriage.carriage_type = CARRIAGE_HEAD;
                        carriage.conductor = 0; //simple train
                    }
                }
                pushPassengerIntoCarriage(&carriage,ThisObject);

                break;
            }
        case OBJECT_WELD:
            {
                break;
            }
        default:
            {
                endCarriage(&train, &carriage);
                endTrain(&train_carrier, &train);

                carriage.carriage_type = CARRIAGE_HEAD;
                carriage.conductor = ThisObject.token;
            }
        }

        LastObject = ThisObject;
    }

    return train_carrier;
};
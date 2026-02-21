#ifndef TRAIN_H
#define TRAIN_H

enum carriage_type_e
{
    CARRIAGE_NONE,
    CARRIAGE_HEAD,
    CARRIAGE_BODY,
};

typedef struct carriage_s
{
    enum carriage_type_e carriage_type;
    object_carrier_t passengers;
    enum token_e conductor;
} carriage_t;

typedef struct carriage_carrier_s
{
    carriage_t * carriages;
    unsigned carriages_length;
} carriage_carrier_t;

enum train_type_e
{
    TRAIN_NONE,
    TRAIN_NORMAL,
    TRAIN_SIMPLE
};

typedef struct train_s
{
    carriage_carrier_t carriage_carrier;
    enum train_type_e train_type;
} train_t;

typedef struct train_carrier_s
{
    train_t * trains;
    unsigned trains_length;
}train_carrier_t;

train_carrier_t object_to_train(object_carrier_t object_carrier);

#endif //TRAIN_H

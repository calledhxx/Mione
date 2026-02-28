#ifndef INSTRUCT_H
#define INSTRUCT_H

enum instruct_e
{
    INSTRUCT_NONE,
    INSTRUCT_LOAD_VARIABLE,
    INSTRUCT_LOAD_VALUE,
    INSTRUCT_TO_VALUE,
    INSTRUCT_ADD,
    INSTRUCT_SUB,
};

typedef struct instruct_s
{
    char instruct;
    long long unsigned object;
} instruct_t;

typedef struct instruct_carrier_s
{
    instruct_t* instructs;
    unsigned instructs_length;
} instruct_carrier_t;

extern void pushInstructIntoCarrier(instruct_carrier_t * CarrierPtr,instruct_t instruct);
void pushInstructsIntoCarrier(instruct_carrier_t * CarrierPtr,instruct_carrier_t instructs);
instruct_carrier_t loadVV(const object_t * objPtr);
extern instruct_carrier_t train_to_instruct(train_carrier_t  train_carrier);

#endif //INSTRUCT_H
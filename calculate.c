#include <stdlib.h>

#include "main.h"

instruct_carrier_t calculate(object_carrier_t object_carrier)
{
    instruct_carrier_t result = {0};

    for (unsigned i = 0; i < object_carrier.objects_length; i++)
    {
        object_t const ThisObject = object_carrier.objects[i];

        switch (ThisObject.object_type)
        {
        case OBJECT_SYMBOL:
            {
                pushInstructIntoCarrier(&result,)
                break;
            }
        case OBJECT_VARIABLE:
            {

                break;
            }
        case OBJECT_VALUE:
            {

                break;
            }
        default:
            exit(5);
        }
    }

    return result;
}
//
// Created by calle on 24-12-28.
//

#include <inttypes.h>

#include "../OBJECTS.h"
#include "../REQUEST.h"
#include "../COUNT.h"

#include "../ERR.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#include "../PROMPT_DEF.h"

HeadReturnObj SET(const HeadCallObj * HeadCallObjectPointer)
{
    HeadReturnObj Result = {0};

    const HeadCallObj HeadCallObject = *HeadCallObjectPointer;

    const unsigned int PairsSize = HeadCallObject.PairCarrier.CarrierLen;
    const PairObj * Pairs = HeadCallObject.PairCarrier.Carrier;


    VariableObjPtrCarrier HeadSuffix = {0};

    ValueObjCarrier SetPromptSuffix = {0};

    int Registration = 0;


    for (unsigned int PairIndex = 0; PairIndex < PairsSize; PairIndex++)
    {
        const PairObj Pair = Pairs[PairIndex];

        switch (Pair.Host.ObjType)
        {
        case HEAD:
            {
                HeadSuffix = REQUEST(Pair.SourceCarrier);
                break;
            }
        case PROMPT:
            {
                switch (Pair.Host.Prompt.Identification)
                {
                case 1:
                    {
                        SetPromptSuffix = COUNT(Pair.SourceCarrier);
                        break;
                    }
                default: exit(-1);

                }

                Registration |= 1<<(Pair.Host.Prompt.Identification-1);

                break;
            }
        default: exit(-1);
        }
    }

    for (int i = 0; i < 32;i++)
    {
        const int cmp = 1<<i;

        switch (Registration & cmp)
        {
        case 0: continue;
        case 1<<(PROMPT_SET-1):
            {
                for (unsigned int HeadSuffixIndex = 0; HeadSuffixIndex < HeadSuffix.CarrierLen ; HeadSuffixIndex++)
                    *HeadSuffix.Carrier[HeadSuffixIndex] = (VariableObj){
                        .Value = HeadSuffixIndex < SetPromptSuffix.CarrierLen
                            ? SetPromptSuffix.Carrier[HeadSuffixIndex]
                            : (ValueObj){
                                .ValueType = 0
                            }
                    };

                break;
            }
        default: exit(-3);
        }
    }

    return Result;
}

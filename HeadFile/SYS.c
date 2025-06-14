//
// Created by calle on 2025/5/21.
//

#include <io.h>
#include <stdio.h>
#include "../OBJECTS.h"
#include "../REQUEST.h"
#include "../COUNT.h"
#include "../ERR.h"

HeadReturnObj SYS(HeadRequestObj * HeadRequestUP)
{
    HeadRequestObj HeadRequest = *HeadRequestUP;

    HeadReturnObj ToReturn = {0};

    PairObj * Pairs = HeadRequest.Pairs;
    int PairsSize = HeadRequest.PairsSize;

    int registeredPrompts = 0;

    CountObj CountedSuffixOfHead = {.ValueSize = 0};
    CountObj CountedSuffixOfPutPrompt = {.ValueSize = 0};


    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1)
        {
            CountedSuffixOfHead = COUNT(Pairs[i].Source, Pairs[i].SourceSize);

            if (CountedSuffixOfHead.ValueSize != 1) ErrCall("no", "M11176", NULL, Prompt.Line, Prompt.Column);
            if (CountedSuffixOfHead.Value[0].ValueType != VALUE_NOPOINTNUMBER_TYPE) ErrCall("no", "M11176", NULL, Prompt.Line, Prompt.Column);

        }
        if (Prompt.ObjType == 2)
        {

            switch (Prompt.Prompt.CurNumber)
            {
            case 10:
                CountedSuffixOfPutPrompt = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                if (CountedSuffixOfPutPrompt.ValueSize == 0) ErrCall("no", "M11176", NULL, Prompt.Line, Prompt.Column);
                break;
            default:
                ErrCall("unsupported prompt type","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }

            if (registeredPrompts&1<<Prompt.Prompt.CurNumber-1) ErrCall("This Prompt has been registered before.","M017",NULL,Prompt.Line,Prompt.Column);
            registeredPrompts |= 1<<Prompt.Prompt.CurNumber-1;
        }
    }


    if (!(registeredPrompts & 1<<10-1)) ErrCall("errr",NULL,NULL,NULL,NULL);

    //

    switch (CountedSuffixOfHead.Value[0].NPNumber)
    {
        case 1:
            {
                for (int i = 0; i < CountedSuffixOfPutPrompt.ValueSize; i++)
                    if (CountedSuffixOfPutPrompt.Value[i].String)
                        _write(
                            1,
                            CountedSuffixOfPutPrompt.Value[i].String,
                            (wcslen(CountedSuffixOfPutPrompt.Value[i].String)+1) * sizeof(wchar_t)
                            );
                    else
                        ErrCall(
                            "Suffix of put prompt wasn't STRING Value",
                            "M1009",
                            NULL,
                            -1,
                            -1
                            );


                break;
            }
        case 2:
            {

                break;
            }
        default:
            {
                ErrCall("unsupported system name","M111",NULL,0,0);
            }
    }

    return ToReturn;
};
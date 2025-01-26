//
// Created by calle on 25-1-26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../OBJECTS.h"
#include "../COUNT.h"
#include "../ERR.h"

HeadReturnObj IF(struct _PairObject*Pairs,int PairsSize){
    HeadReturnObj ToReturn;
    ToReturn.ToState = 0;


    VariableRequestUPObj Request = {.VariablesSize = 0};

    CountObj CountedDB = {.ValueSize = 0};
    CountObj CountedThenRange = {.ValueSize = 0};
    CountObj CountedElseRange = {.ValueSize = 0};



    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {
            CountedDB = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
            if(CountedDB.ValueSize != 1) ErrCall("IF Error","M9121321",NULL,Prompt.Line,Prompt.Column);

            int db = (CountedDB.Value[0].ValueType) ? ((CountedDB.Value[0].ValueType != 8) ? 1 : CountedDB.Value[0].db) :0;
            printf("%d IF\n",db);
        }
        if (Prompt.ObjType == 2)
        {
            switch (Prompt.Prompt.CurNumber)
            {
              case 3:
                  CountedThenRange = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                  if(CountedThenRange.ValueSize != 1) ErrCall("THEN Error","M9121321",NULL,Prompt.Line,Prompt.Column);
                  if(CountedThenRange.Value[0].ValueType != 5) ErrCall("THEN (RANGE) Error","M9121321",NULL,Prompt.Line,Prompt.Column);

                  break;
            case 4:
                CountedElseRange = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                if(CountedElseRange.ValueSize != 1) ErrCall("ELSE Error","M9121321",NULL,Prompt.Line,Prompt.Column);
                if(CountedElseRange.Value[0].ValueType != 5) ErrCall("ELSE (RANGE) Error","M9121321",NULL,Prompt.Line,Prompt.Column);


                break;

            default:
                ErrCall("unsupported prompt type","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }
        }
    }




    return ToReturn;
}

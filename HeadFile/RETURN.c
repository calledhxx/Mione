//
// Created by calle on 24-12-28.
//

#include "../OBJECTS.h"
#include "../REQUEST.h"
#include "../COUNT.h"
#include <stdio.h>

HeadReturnObj RETURN(struct _PairObject*Pairs,int PairsSize)
{
    HeadReturnObj ToReturn;

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {

            //VariableRequestObj Request = REQUEST(Pairs[i].Source, Pairs[i].SourceSize);

            printf("%d\n", Pairs[i].SourceSize);

            CountObj Counted = COUNT(Pairs[i].Source, Pairs[i].SourceSize);


            ToReturn.ToState = 1;
            ToReturn.Vs = (ValueReturnObj){
                .ValueSize = Counted.ValueSize,
               .Value = Counted.Value,
            };
        }
        if (Prompt.ObjType == 2)
        {



        }

    }

    return ToReturn;
}
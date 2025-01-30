#include <stdio.h>
#include <stdlib.h>

#include "../OBJECTS.h"
#include "../COUNT.h"

HeadReturnObj SVV(struct _PairObject*Pairs,int PairsSize)
{
    HeadReturnObj Re;
    Re.ToState = 0;

    int fHead = 0;

    printf("calledhxx\n");
    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;
        if (Prompt.ObjType ==2)
        {

        }else //偽Head
        {
            CountObj Counted = COUNT(Pairs[i].Source,Pairs[i].SourceSize);
            fHead=1;
            VariablesObj Vars;
            Vars.VarsSize = 0;
            Vars.Vars = malloc(Vars.VarsSize);
            for (int a = 0; a < Counted.ValueSize; a++)
            {
                Vars.VarsSize++;
                Vars.Vars = realloc(Vars.Vars,Vars.VarsSize*sizeof(VariableObj));
                Vars.Vars[Vars.VarsSize-1].Name = NULL;
                Vars.Vars[Vars.VarsSize-1].Val = Counted.Value[a];
            }
            Re.Vars = Vars;
        }

        for (int index = 0; index < Pairs[i].SourceSize; index++)
        {

        }
    }

    if (fHead)
    {
        Re.ToState = 4;
    }
    return Re;
}
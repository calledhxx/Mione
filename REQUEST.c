//
// Created by calle on 25-1-3.
//

#include "OBJECTS.h"
#include "ERR.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "COUNT.h"
#include "IMPLEMENT.h"
#include "SYMBOL_DEF.h"
#include "MIONE.h"


VariableRequestUPObj REQUEST(MioneObj*Pack,int PackSize)
{

    //todo
    VariableRequestUPObj Returns;

    Returns.VariablesSize = 0;
    Returns.VariableUPs = malloc(0);


    for (int i = 0; i < PackSize; i++)
    {
        Returns.VariablesSize++;
        Returns.VariableUPs = realloc(Returns.VariableUPs,sizeof(VariableObj*));
        Returns.VariableUPs[Returns.VariablesSize - 1] = Pack[i].VarUP;
    }

    return Returns;
}

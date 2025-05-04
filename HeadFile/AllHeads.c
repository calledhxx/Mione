//
// Created by calle on 24-12-28.
//
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "../OBJECTS.h"

#include "SET.h"
#include "RETURN.h"
#include "IF.h"
#include "FOR.h"

int ifPromptRegistered(int registeredPrompts,PromptObj prompt)
{
    int max = 0;
    for (int i = 0;;i++)
        if (pow(2,i) > registeredPrompts)
        {
            max = i-1;
            break;
        }

    for (int i = max;;i--)
    {
        int cmp = pow(2,i);

        if (!registeredPrompts) break;

        if (registeredPrompts - cmp>=0)
        {
            registeredPrompts -= cmp;
            if (pow(2,prompt.CurNumber-1) == cmp) return 0;
        }

    }
    return 1;
};

HeadObj Heads[]  = {
    (HeadObj){
        .Fuc = SET,
        .Name = "set",
        .CurNumber = 1,
    },
    (HeadObj){
        .Fuc = RETURN,
        .Name = "return",
        .CurNumber = 2
    },
    (HeadObj){
        .Fuc = IF,
        .Name = "if",
        .CurNumber = 3
    },
    (HeadObj){
        .Fuc = FOR,
        .Name = "for",
        .CurNumber = 4
    },

    //記得到MIONE.c新增可執行CurNumber
    (HeadObj){
        .CurNumber = -1
    }
};
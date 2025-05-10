//
// Created by calle on 24-12-28.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OBJECTS.h"
#include "MIONE.h"

#include <inttypes.h>

#include "HeadFile/AllHeads.h"
#include "HeadFile/SVV.h"
#include "ERR.h"
#include "PROMPT_DEF.h"

//THIS IS THE UGLIEST CODE I MADE

void newSection(MioneBuiltObj *BuiltObjUP)
{

    (*BuiltObjUP).SectionsSize++;
    (*BuiltObjUP).Sections = realloc((*BuiltObjUP).Sections, sizeof(MioneSectionObj) * (*BuiltObjUP).SectionsSize);

    (*BuiltObjUP).Sections[(*BuiltObjUP).SectionsSize-1].HeadAction = (MioneObj){ .Head = (HeadObj){ .Fuc = 0}};

    (*BuiltObjUP).Sections[(*BuiltObjUP).SectionsSize-1].Pairs = malloc(0);
    (*BuiltObjUP).Sections[(*BuiltObjUP).SectionsSize-1].PairsSize = 0;

    (*BuiltObjUP).Sections[(*BuiltObjUP).SectionsSize-1].PairsSize++;
    (*BuiltObjUP).Sections[(*BuiltObjUP).SectionsSize-1].Pairs = realloc(
        (*BuiltObjUP).Sections[(*BuiltObjUP).SectionsSize-1].Pairs,
        sizeof(PairObj) * (*BuiltObjUP).Sections[(*BuiltObjUP).SectionsSize-1].PairsSize
        );

    (*BuiltObjUP).Sections[(*BuiltObjUP).SectionsSize-1].Pairs[(*BuiltObjUP).Sections[(*BuiltObjUP).SectionsSize-1].PairsSize-1].Source = malloc(0);
    (*BuiltObjUP).Sections[(*BuiltObjUP).SectionsSize-1].Pairs[(*BuiltObjUP).Sections[(*BuiltObjUP).SectionsSize-1].PairsSize-1].SourceSize = 0;

}

MioneBuiltObj ToMione(const MioneToBuildObj ToBuildObj)
{
    const int ObjsSize = ToBuildObj.ObjsSize;
    const MioneObj* Objs = ToBuildObj.Objs;

    MioneBuiltObj BuiltObj = {0};

    BuiltObj.SectionsSize = 0;
    BuiltObj.Sections = malloc(0);

    for (int index = 0;index < ObjsSize;index++)
    {
        MioneObj Mio = Objs[index];

        if (Mio.ObjType == HEAD)
        {
            for (int i = 0; 1; i++)
            {
                if (Heads[i].CurNumber == -1) break;

                if (strcmp(Mio.Head.Name, Heads[i].Name) == 0)
                {
                    newSection(&BuiltObj);
                    BuiltObj.Sections[BuiltObj.SectionsSize-1].HeadAction = (MioneObj){ .Head = (HeadObj){ .Fuc = SVV}};
                    BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs[BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize-1].Prompt = Mio;
                    BuiltObj.Sections[BuiltObj.SectionsSize-1].HeadAction = Mio;


                    break;
                }
            }

        }

        if (Mio.ObjType == PROMPT)
        {
            if (!BuiltObj.Sections[BuiltObj.SectionsSize-1].HeadAction.Head.Fuc); //err;

            BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize++;
            BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs = realloc(
                BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs,
                sizeof(PairObj) * BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize
                );

            BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs[BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize-1].Source = malloc(0);
            BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs[BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize-1].SourceSize = 0;

            BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs[BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize-1].Prompt = Mio;
        }

        if (Mio.ObjType == SYMBOL || Mio.ObjType == VARIABLE || Mio.ObjType == VALUE) // SVV
        {

            if (!BuiltObj.SectionsSize)
            {
                newSection(&BuiltObj);
                BuiltObj.Sections[BuiltObj.SectionsSize-1].HeadAction = (MioneObj){ .Head = (HeadObj){ .Fuc = SVV}};
            }


            BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs[BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize-1].SourceSize++;
            BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs[BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize-1].Source = realloc(
                    BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs[BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize-1].Source,
                    BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs[BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize-1].SourceSize * sizeof(MioneObj)
                    );

            BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs[BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize-1].Source[BuiltObj.Sections[BuiltObj.SectionsSize-1].Pairs[BuiltObj.Sections[BuiltObj.SectionsSize-1].PairsSize-1].SourceSize-1] = Mio;
        }

        if (
            (index != 0 && Objs[index].ObjType == -1 && index+1<ObjsSize && Objs[index+1].ObjType >= SYMBOL)
            || (Mio.ObjType >= VARIABLE && index+1 < ObjsSize && Objs[index+1].ObjType >= VARIABLE)
            || (Mio.ObjType == SYMBOL && !Mio.Symbol.AfterConnectVV && index+1 < ObjsSize && Objs[index+1].ObjType >= VARIABLE)
            || (index+1 < ObjsSize && Mio.ObjType == Objs[index+1].ObjType && Mio.ObjType >= VARIABLE)
            )
        {
            newSection(&BuiltObj);
            BuiltObj.Sections[BuiltObj.SectionsSize-1].HeadAction = (MioneObj){ .Head = (HeadObj){ .Fuc = SVV}};

        }
    }

    return BuiltObj;
}
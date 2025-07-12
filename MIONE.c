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

void newSection(MioneSectionObjCarrier *BuiltObjPointer)
{

    (*BuiltObjPointer).CarrierLen++;
    (*BuiltObjPointer).Carrier = realloc((*BuiltObjPointer).Carrier, sizeof(MioneSectionObj) * (*BuiltObjPointer).CarrierLen);

    (*BuiltObjPointer).Carrier[(*BuiltObjPointer).CarrierLen-1].HeadAction = (MioneObj){ .Head = (HeadObj){ .Fuc = 0}};

    (*BuiltObjPointer).Carrier[(*BuiltObjPointer).CarrierLen-1].Pairs = malloc(0);
    (*BuiltObjPointer).Carrier[(*BuiltObjPointer).CarrierLen-1].PairsSize = 0;

    (*BuiltObjPointer).Carrier[(*BuiltObjPointer).CarrierLen-1].PairsSize++;
    (*BuiltObjPointer).Carrier[(*BuiltObjPointer).CarrierLen-1].Pairs = realloc(
        (*BuiltObjPointer).Carrier[(*BuiltObjPointer).CarrierLen-1].Pairs,
        sizeof(PairObj) * (*BuiltObjPointer).Carrier[(*BuiltObjPointer).CarrierLen-1].PairsSize
        );

    (*BuiltObjPointer).Carrier[(*BuiltObjPointer).CarrierLen-1].Pairs[(*BuiltObjPointer).Carrier[(*BuiltObjPointer).CarrierLen-1].PairsSize-1].Source = malloc(0);
    (*BuiltObjPointer).Carrier[(*BuiltObjPointer).CarrierLen-1].Pairs[(*BuiltObjPointer).Carrier[(*BuiltObjPointer).CarrierLen-1].PairsSize-1].SourceSize = 0;

}

MioneSectionObjCarrier ToMione(const MioneObjCarrier ToBuildObj)
{
    const unsigned int ObjsSize = ToBuildObj.CarrierLen;
    const MioneObj* Objs = ToBuildObj.Carrier;

    MioneSectionObjCarrier BuiltObj = {0};

    BuiltObj.CarrierLen = 0;
    BuiltObj.Carrier = NULL;

    for (int index = 0;index < ObjsSize;index++)
    {
        MioneObj Mio = Objs[index];

        if (Mio.ObjType == HEAD)
        {
            for (int i = 0; 1; i++)
            {
                if (Heads[i].Identification == -1) break;

                if (wcscmp(Mio.Head.Name, Heads[i].Name) == 0)
                {
                    newSection(&BuiltObj);
                    BuiltObj.Carrier[BuiltObj.CarrierLen-1].HeadAction = (MioneObj){ .Head = (HeadObj){ .Fuc = SVV}};
                    BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs[BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize-1].Prompt = Mio;
                    BuiltObj.Carrier[BuiltObj.CarrierLen-1].HeadAction = Mio;


                    break;
                }
            }

        }

        if (Mio.ObjType == PROMPT)
        {
            if (!BuiltObj.Carrier[BuiltObj.CarrierLen-1].HeadAction.Head.Fuc); //err;

            BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize++;
            BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs = realloc(
                BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs,
                sizeof(PairObj) * BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize
                );

            BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs[BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize-1].Source = malloc(0);
            BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs[BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize-1].SourceSize = 0;

            BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs[BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize-1].Prompt = Mio;
        }

        if (Mio.ObjType == SYMBOL || Mio.ObjType == VARIABLE || Mio.ObjType == VALUE) // SVV
        {

            if (!BuiltObj.CarrierLen)
            {
                newSection(&BuiltObj);
                BuiltObj.Carrier[BuiltObj.CarrierLen-1].HeadAction = (MioneObj){ .Head = (HeadObj){ .Fuc = SVV}};
            }


            BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs[BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize-1].SourceSize++;
            BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs[BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize-1].Source = realloc(
                    BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs[BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize-1].Source,
                    BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs[BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize-1].SourceSize * sizeof(MioneObj)
                    );

            BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs[BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize-1].Source[BuiltObj.Carrier[BuiltObj.CarrierLen-1].Pairs[BuiltObj.Carrier[BuiltObj.CarrierLen-1].PairsSize-1].SourceSize-1] = Mio;
        }

        if (
            (index != 0 && Objs[index].ObjType == -1 && index+1<ObjsSize && Objs[index+1].ObjType >= SYMBOL)
            || (Mio.ObjType >= VARIABLE && index+1 < ObjsSize && Objs[index+1].ObjType >= VARIABLE)
            || (Mio.ObjType == SYMBOL && !Mio.Symbol.AfterConnectVV && index+1 < ObjsSize && Objs[index+1].ObjType >= VARIABLE)
            || (index+1 < ObjsSize && Mio.ObjType == Objs[index+1].ObjType && Mio.ObjType >= VARIABLE)
            )
        {
            newSection(&BuiltObj);
            BuiltObj.Carrier[BuiltObj.CarrierLen-1].HeadAction = (MioneObj){ .Head = (HeadObj){ .Fuc = SVV}};

        }
    }

    return BuiltObj;
}
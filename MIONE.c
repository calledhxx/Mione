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

MioneBuiltObj ToMione(const MioneToBuildObj ToBuildObj)
{
    const int ObjsSize = ToBuildObj.ObjsSize;
    const MioneObj* Objs = ToBuildObj.Objs;

    MioneBuiltObj BuiltObj = {0};

    BuiltObj.SectionsSize = 1;
    BuiltObj.Sections = malloc(sizeof(MioneSectionObj));

    BuiltObj.Sections[0].PairsSize = 0;
    BuiltObj.Sections[0].Pairs = malloc(0);

    for (int index = 0;index < ObjsSize;index++)
    {
        MioneObj Mio = Objs[index];

       if (Mio.ObjType == SYMBOL && strcmp(Mio.Symbol.Name, ";") == 0)
       {
           int * SectionSizeUP = &BuiltObj.SectionsSize;
           MioneSectionObj ** SectionsUP =  &BuiltObj.Sections;
           (*SectionSizeUP)++;
           *SectionsUP = realloc(*SectionsUP, (*SectionSizeUP) * sizeof(MioneSectionObj));

           MioneSectionObj * thisSectionUP =  &(*SectionsUP)[*SectionSizeUP-1];

           thisSectionUP->HeadAction.Head.Fuc = 0;

           PairObj ** PairsUP = &(thisSectionUP->Pairs);
           int * PairsSizeUP = &(thisSectionUP->PairsSize);

           *PairsUP = malloc(0);
           *PairsSizeUP = 0;

           continue;
       }

        if (Mio.ObjType == HEAD) // Head
        {
            for (int i = 0; 1; i++)
            {
                if (Heads[i].CurNumber == -1) break;
                if (strcmp(Mio.Head.Name, Heads[i].Name) == 0)
                {
                    int * SectionSizeUP = &BuiltObj.SectionsSize;
                    MioneSectionObj ** SectionsUP =  &BuiltObj.Sections;

                    MioneSectionObj * thisSectionUP =  &(*SectionsUP)[*SectionSizeUP-1];
                    thisSectionUP->HeadAction = Mio;

                    PairObj ** PairsUP = &(thisSectionUP->Pairs);
                    int * PairsSizeUP = &(thisSectionUP->PairsSize);

                    (*PairsSizeUP)++;
                    *PairsUP = realloc(*PairsUP, (*PairsSizeUP) * sizeof(PairObj));
                    (*PairsUP)[*PairsSizeUP-1].Prompt = Mio;

                    PairObj * thisPairUP = &((*PairsUP)[*PairsSizeUP-1]);

                    MioneObj ** SourceUP = &(thisPairUP->Source);
                    int * SourceSizeUP = &(thisPairUP->SourceSize);

                    *SourceUP = malloc(0);
                    *SourceSizeUP = 0;
                }
            }

        }

        if (Mio.ObjType == PROMPT) // PROMPT
        {
            int * SectionSizeUP = &BuiltObj.SectionsSize;
            MioneSectionObj ** SectionsUP =  &BuiltObj.Sections;

            const MioneSectionObj * thisSectionUP =  &(*SectionsUP)[*SectionSizeUP-1];

            PairObj ** PairsUP = &(thisSectionUP->Pairs);
            int * PairsSizeUP = &(thisSectionUP->PairsSize);

            (*PairsSizeUP)++;
            *PairsUP = realloc(*PairsUP, (*PairsSizeUP) * sizeof(PairObj));

            PairObj * thisPairUP = &((*PairsUP)[*PairsSizeUP-1]);

            thisPairUP->Prompt = Mio;

            MioneObj ** SourceUP = &(thisPairUP->Source);
            int * SourceSizeUP = &(thisPairUP->SourceSize);

            *SourceUP = malloc(0);
            *SourceSizeUP = 0;
        }

        if (Mio.ObjType == SYMBOL || Mio.ObjType == VARIABLE || Mio.ObjType == VALUE) // SVV
        {
            int * SectionSizeUP = &BuiltObj.SectionsSize;
            MioneSectionObj ** SectionsUP =  &BuiltObj.Sections;

            MioneSectionObj * thisSectionUP =  &(*SectionsUP)[*SectionSizeUP-1];

            PairObj ** PairsUP = &(thisSectionUP->Pairs);
            int * PairsSizeUP = &(thisSectionUP->PairsSize);

            PairObj * thisPairUP = &((*PairsUP)[*PairsSizeUP-1]);

            if (!thisSectionUP->HeadAction.Head.Fuc)
            {
                thisSectionUP->HeadAction.Head.Fuc = SVV;

                (*PairsSizeUP)++;
                *PairsUP = realloc(*PairsUP, (*PairsSizeUP) * sizeof(PairObj));

                thisPairUP = &((*PairsUP)[*PairsSizeUP-1]);

                MioneObj ** SourceUP = &(thisPairUP->Source);
                int * SourceSizeUP = &(thisPairUP->SourceSize);

                *SourceUP = malloc(0);
                *SourceSizeUP = 0;
            }

            MioneObj ** SourceUP = &(thisPairUP->Source);
            int * SourceSizeUP = &(thisPairUP->SourceSize);

            (*SourceSizeUP)++;
            *SourceUP = realloc( *SourceUP, *SourceSizeUP * sizeof(MioneObj));
            (*SourceUP)[(*SourceSizeUP)-1] = Mio;
        }

        if (
        // ObjsSize - 1 == index ||
       ((index != ObjsSize - 1) &&
           (Objs[index+1].ObjType == HEAD ||
           (Mio.ObjType == Objs[index+1].ObjType && (Objs[index+1].ObjType == SYMBOL ? !Objs[index+1].Symbol.CanConnect : 1)) ||
           (Mio.ObjType == VARIABLE &&  Objs[index+1].ObjType == VALUE) ||  (Mio.ObjType == VALUE &&  Objs[index+1].ObjType == VARIABLE)||
           (Mio.ObjType == SYMBOL && Mio.Symbol.AfterConnect == 0 && (Objs[index+1].ObjType == VARIABLE || Objs[index+1].ObjType == VALUE))
               )
           )&& (Mio.ObjType != PROMPT)
       )
         {
            int * SectionSizeUP = &BuiltObj.SectionsSize;
            MioneSectionObj ** SectionsUP =  &BuiltObj.Sections;
            (*SectionSizeUP)++;
            *SectionsUP = realloc(*SectionsUP, (*SectionSizeUP) * sizeof(MioneSectionObj));

            MioneSectionObj * thisSectionUP =  &(*SectionsUP)[*SectionSizeUP-1];

            PairObj ** PairsUP = &(thisSectionUP->Pairs);
            int * PairsSizeUP = &(thisSectionUP->PairsSize);

            thisSectionUP->HeadAction.Head.Fuc = 0;

            *PairsUP = malloc(0);
            *PairsSizeUP = 0;
         }
    }
    return BuiltObj;
}
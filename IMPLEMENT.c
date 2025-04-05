#include <stdio.h>
#include "HeadFile/SVV.h"

#include "OBJECTS.h"

ImplementedObj IMPLEMENT(const ToImplementObj toImplement)
{
    ImplementedObj Obj;

    int SectionsSize = toImplement.Built.SectionsSize;
    MioneSectionObj * Sections = toImplement.Built.Sections;

    printf("org address %p\n",SVV);

    for (int SectionIndex = 0; SectionIndex < SectionsSize; SectionIndex++)
    {
        printf("fuc address %p .", Sections[SectionIndex].HeadAction.Head.Fuc);

        PairObj * Pairs = Sections[SectionIndex].Pairs;
        int PairsSize = Sections[SectionIndex].PairsSize;

        for (int PairIndex = 0; PairIndex < PairsSize; PairIndex++)
        {
            printf("Pair %d ",Pairs[PairIndex].SourceSize);
        }

        printf("\n");
    }

    return Obj;
}

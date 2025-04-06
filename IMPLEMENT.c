#include <stdio.h>
#include "HeadFile/SVV.h"

#include "OBJECTS.h"

ImplementedObj IMPLEMENT(const ToImplementObj toImplement)
{
    ImplementedObj Obj;

    int SectionsSize = toImplement.Built.SectionsSize;
    MioneSectionObj * Sections = toImplement.Built.Sections;


    for (int SectionIndex = 0; SectionIndex < SectionsSize; SectionIndex++)
    {
        MioneSectionObj thisSection = Sections[SectionIndex];

        PairObj * Pairs = thisSection.Pairs;
        int PairsSize = thisSection.PairsSize;

        thisSection.HeadAction.Head.Fuc(
               &(HeadRequestObj){
                   .Pairs =Pairs,
                   .PairsSize = PairsSize,
               });

    }

    return Obj;
}

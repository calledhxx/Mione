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
        printf("address %p %d\n", Sections[SectionIndex].HeadAction.Head.Fuc,Sections[SectionIndex].PairsSize);
    }

    return Obj;
}

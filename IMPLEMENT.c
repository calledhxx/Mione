#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "HeadFile/SVV.h"

#include "OBJECTS.h"

int ClearLocalVariables()
{
    return 0;
};

ImplementedObj IMPLEMENT(const ToImplementObj toImplement)
{
    ImplementedObj Obj = {0};
    Obj.ToState=0;

    DefinedVariablesCaseObj DVs;
    DVs.DefinedVariables = malloc(0);
    DVs.DefinedVariablesSize = 0;

    int SectionsSize = toImplement.Built.SectionsSize;
    MioneSectionObj * Sections = toImplement.Built.Sections;

    for (int SectionIndex = 0; SectionIndex < SectionsSize; SectionIndex++)
    {
        MioneSectionObj thisSection = Sections[SectionIndex];

        if (thisSection.HeadAction.Head.Fuc)
        {

            PairObj * Pairs = thisSection.Pairs;
            int PairsSize = thisSection.PairsSize;

            HeadReturnObj HeadReturn = thisSection.HeadAction.Head.Fuc(
                   &(HeadRequestObj){
                       .Pairs =Pairs,
                       .PairsSize = PairsSize,
                   });

            for (int i = 0;;i++)
            {
                const int cmp = pow(2,i);

                if (HeadReturn.ToState - cmp >=0)
                {
                    HeadReturn.ToState=-cmp;

                    switch (cmp)
                    {
                    case 0: break;

                    case 1:
                        {
                            Obj.ToState=+1;
                            Obj.Values = HeadReturn.Values;
                            break;
                        }
                    case 4:
                        {
                            DVs = HeadReturn.VAVs;
                            break;
                        }

                    default:
                        {

                        }
                    }
                }else break;

            }

        }
    }

    return Obj;
}

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "HeadFile/SVV.h"

#include "OBJECTS.h"
#include "PROMPT_DEF.h"

ImplementedObj IMPLEMENT(const ToImplementObj toImplement)
{

    ImplementedObj Obj = {0};
    Obj.ToState=0;

    VariablesObj Vars;
    Vars.Vars = malloc(0);
    Vars.VarsSize = 0;

    DefinedVariablesCaseObj ImplementVAVs;
    ImplementVAVs.DefinedVariablesSize = 0;
    ImplementVAVs.DefinedVariables = malloc(0);

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

                       .VariablesUP = &Vars.Vars,
                       .VariablesUPSizeUP = &Vars.VarsSize,
                   });

            int max = 0;
            for (int i = 0;;i++)
                if (pow(2,i-1) > HeadReturn.ToState)
                {
                    max = i-1;
                    break;
                }


            for (int i = 0;max>i;i++)
            {
                const int cmp = pow(2,i);

                if (!HeadReturn.ToState) break;

                if (HeadReturn.ToState & cmp)
                {
                    switch (cmp)
                    {
                    case 0: break;

                    case 1:
                        {
                            Obj.ToState=+1;
                            Obj.Values = HeadReturn.Values;
                            break;
                        }
                    case 2:
                        {

                            for (int j = 0; j < HeadReturn.VAVs.DefinedVariablesSize; j++)
                            {
                                ImplementVAVs.DefinedVariablesSize++;
                                ImplementVAVs.DefinedVariables = realloc(ImplementVAVs.DefinedVariables,ImplementVAVs.DefinedVariablesSize*sizeof(DefinedVariableObj));
                                ImplementVAVs.DefinedVariables[ImplementVAVs.DefinedVariablesSize-1] = HeadReturn.VAVs.DefinedVariables[j];
                            }
                            break;
                        }
                    case 4:
                        {
                            for (int j=0;j<HeadReturn.Vars.VarsSize;j++)
                            {
                                Vars.VarsSize++;
                                Vars.Vars = realloc(Vars.Vars,Vars.VarsSize*sizeof(DefinedVariableObj));
                                Vars.Vars[Vars.VarsSize-1] = HeadReturn.Vars.Vars[j];
                            }
                            break;
                        }

                    default:
                        {
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < ImplementVAVs.DefinedVariablesSize; i++)
        ImplementVAVs.DefinedVariables[i].TheDefinedVarUP->Val = ImplementVAVs.DefinedVariables[i].Value;

    if (Vars.VarsSize)
    {
        Obj.ToState=+2;
        Obj.Vars = Vars;
    }

    return Obj;
}

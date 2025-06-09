//
// Created by calle on 24-12-28.
//

#include <inttypes.h>

#include "../OBJECTS.h"
#include "../REQUEST.h"
#include "../COUNT.h"

#include "../ERR.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#include "../PROMPT_DEF.h"

HeadReturnObj SET(HeadRequestObj * HeadRequestUP)
{
    HeadRequestObj HeadRequest = *HeadRequestUP;

    PairObj * Pairs = HeadRequest.Pairs;
    int PairsSize = HeadRequest.PairsSize;

    HeadReturnObj ToReturn = {0};
    ToReturn.ToState = 0;
    ToReturn.VAVs.DefinedVariables = malloc(0);
    ToReturn.VAVs.DefinedVariablesSize = 0;

    VariableRequestUPObj RequestedSuffixOfHead = {.VariablesSize = 0};
    CountObj CountedSuffixOfSetPrompt = {.ValueSize = 0};

    int registeredPrompts = 0;

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {
            RequestedSuffixOfHead = REQUEST(Pairs[i].Source, Pairs[i].SourceSize);
            if (RequestedSuffixOfHead.VariablesSize == 0) ErrCall("no", "M11176", NULL, Prompt.Line, Prompt.Column);
        }
        if (Prompt.ObjType == 2)
        {

            switch (Prompt.Prompt.CurNumber)
            {
            case 1:
                CountedSuffixOfSetPrompt = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                if (CountedSuffixOfSetPrompt.ValueSize!=RequestedSuffixOfHead.VariablesSize) ErrCall("Variables couldn't be paired with values","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            case 2:
                if (Pairs[i].SourceSize) ErrCall("`host` CAN NOT BE SET TO ANY SOURCE","M123",NULL,Prompt.Line,Prompt.Column);
                break;
            default:
                ErrCall("unsupported prompt type","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }

            if (registeredPrompts&1<<Prompt.Prompt.CurNumber-1) ErrCall("This Prompt has been registered before.","M017",NULL,Prompt.Line,Prompt.Column);
            registeredPrompts |= 1<<Prompt.Prompt.CurNumber-1;
        }
    }

    if (!RequestedSuffixOfHead.VariablesSize) ErrCall("no REQUEST?","M111",NULL,Pairs[0].Source[0].Line,Pairs[0].Source[0].Column);


    int max = 0;
    for (int i = 0;;i++)
        if (1<<i-1 > registeredPrompts)
        {
            max = i-1;
            break;
        }


    for (int i = 0;max>i;i++)
    {
        const int cmp = 1<<i;

        if (!registeredPrompts) break;

        if (registeredPrompts & cmp)
        {
            switch (i+1)
            {
                case 1:
                    {
                        for(int CountedSuffixOfSetPromptIndex = 0; CountedSuffixOfSetPromptIndex < CountedSuffixOfSetPrompt.ValueSize; CountedSuffixOfSetPromptIndex++)
                        {
                            RequestedSuffixOfHead.VariableUPs[CountedSuffixOfSetPromptIndex]->Val = CountedSuffixOfSetPrompt.Value[CountedSuffixOfSetPromptIndex];
                        }

                        ToReturn.ToState|=4;

                        VariableObj * Vars = malloc(0);
                        for (int VariableIndex = 0; VariableIndex < RequestedSuffixOfHead.VariablesSize; VariableIndex++)
                        {
                            Vars = realloc(Vars, (VariableIndex + 1) * sizeof(VariableObj));
                            Vars[VariableIndex] = *(RequestedSuffixOfHead.VariableUPs[VariableIndex]);
                        }
                        ToReturn.Vars = (VariablesObj){
                            .VarsSize = RequestedSuffixOfHead.VariablesSize,
                            .Vars = Vars
                        };
                        break;
                    }
                case 2:
                    {
                        extern ScopeObj MainSVU;
                        extern VariableObj * retVarUP(ScopeObj * SVUup,const wchar_t* Name,const int Place);

                        ToReturn.ToState|=2;

                        for (int RequestIndex = 0; RequestIndex < RequestedSuffixOfHead.VariablesSize; RequestIndex++)
                        {
                            ValueObj V = (ValueObj){.ValueType = 0};

                            VariableObj * ret = retVarUP(Pairs[0].Prompt.ScopeUP,RequestedSuffixOfHead.VariableUPs[RequestIndex]->Name,0);

                            V = ret->Val;


                            ret->Val = (ValueObj){
                                .ValueType = 0
                            };

                            ToReturn.VAVs.DefinedVariablesSize++;
                            ToReturn.VAVs.DefinedVariables = realloc( ToReturn.VAVs.DefinedVariables, ToReturn.VAVs.DefinedVariablesSize * sizeof(DefinedVariableObj));
                            ToReturn.VAVs.DefinedVariables[ToReturn.VAVs.DefinedVariablesSize-1] = (DefinedVariableObj){
                                .Value = V,
                                .TheDefinedVarUP = ret
                            };
                        }
                        break;

                    }
                default:
                    {

                    }
            }
        }

    }
    return ToReturn;
}

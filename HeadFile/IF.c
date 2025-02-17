//
// Created by calle on 25-1-26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../OBJECTS.h"
#include "../COUNT.h"
#include "../MIONE.h"
#include "../ERR.h"
#include "../MTC.h"

HeadReturnObj IF(struct _PairObject*Pairs,int PairsSize){
    HeadReturnObj ToReturn;
    ToReturn.ToState = 0;


    VariableRequestUPObj Request = {.VariablesSize = 0};

    CountObj CountedDB = {.ValueSize = 0};
    CountObj CountedThenRange = {.ValueSize = 0};
    CountObj CountedElseRange = {.ValueSize = 0};

    int db = 0;

    int then = 0,_else = 0;

    for (int i = 0; i < PairsSize; i++)
    {
        MioneObj Prompt = Pairs[i].Prompt;

        if (Prompt.ObjType == 1) //Head代替Prompt
        {
            CountedDB = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
            if(CountedDB.ValueSize != 1) ErrCall("After `if` HEAD should be only a SOURCE","M020",NULL,Prompt.Line,Prompt.Column);

            db = (CountedDB.Value[0].ValueType) ? ((CountedDB.Value[0].ValueType != 8) ? 1 : CountedDB.Value[0].db) :0;
        }
        if (Prompt.ObjType == 2)
        {
            switch (Prompt.Prompt.CurNumber)
            {
              case 3:
                  CountedThenRange = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                  if(CountedThenRange.ValueSize != 1) ErrCall("THEN Error","M9121321",NULL,Prompt.Line,Prompt.Column);
                  if(CountedThenRange.Value[0].ValueType != 5) ErrCall("THEN (RANGE) Error","M9121321",NULL,Prompt.Line,Prompt.Column);

                  then= 1;

                  break;
            case 4:
                CountedElseRange = COUNT(Pairs[i].Source, Pairs[i].SourceSize);
                if(CountedElseRange.ValueSize != 1) ErrCall("ELSE Error","M9121321",NULL,Prompt.Line,Prompt.Column);
                if(CountedElseRange.Value[0].ValueType != 5) ErrCall("ELSE (RANGE) Error","M9121321",NULL,Prompt.Line,Prompt.Column);

                _else= 1;

                break;

            default:
                ErrCall("This Prompt is not supported by IF","M111",NULL,Prompt.Line,Prompt.Column);
                break;
            }
        }
    }

    if(then || _else){
      if(_else ? _else != then : 0) ErrCall("IF-THEN-ELSE Error","M9121321",NULL,Pairs[0].Prompt.Line,Pairs[0].Prompt.Column);


      if(then == db || _else != db){

      	  ValueObj RangeArea = db?CountedThenRange.Value[0]:CountedElseRange.Value[0];

          ThreadObj orgThread = Threads.Threads[nowThreadIn];

          Threads.Threads[nowThreadIn].Fuc = Range;
          Threads.Threads[nowThreadIn].IndexUP = malloc(sizeof(int));
          *Threads.Threads[nowThreadIn].IndexUP = 0;
          Threads.Threads[nowThreadIn].Objs = RangeArea.Area.Area;
          Threads.Threads[nowThreadIn].ObjsSize = RangeArea.Area.Size;

          Threads.Threads[nowThreadIn].Request = NULL;
          Threads.Threads[nowThreadIn].RequestSize = 0;


          Threads.Threads[nowThreadIn].EndLoaclUP = malloc(sizeof(DefinedVarAndValueObj*));
          *Threads.Threads[nowThreadIn].EndLoaclUP = malloc(0);

          Threads.Threads[nowThreadIn].EndLoaclSizeUP = malloc(sizeof(int));
          *Threads.Threads[nowThreadIn].EndLoaclSizeUP = 0;


          Threads.Threads[nowThreadIn].isChild = 1;

          MioneReturnObj R = MTC(Threads.ThreadsSize-1).Return;

          Threads.Threads[nowThreadIn] = orgThread;


          int States[] =  {
             1
         };

          for (int StateIndex = 0; StateIndex<(sizeof(States)/sizeof(int)); StateIndex++)
          {
              if (R.ToState >= States[StateIndex])
              {
                  R.ToState = R.ToState-States[StateIndex];

                  switch (States[StateIndex]){
                  case 1:
                      ToReturn.ToState = ToReturn.ToState+1;
                      ToReturn.Vs = R.Vs;
                      break;
                  }
              }
          }
      }
    }
    return ToReturn;
}

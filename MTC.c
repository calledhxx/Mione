//
// Created by calle on 25-2-5.
//


#include <stdio.h>
#include <stdlib.h>

#include "OBJECTS.h"
#include "MIONE.h"

ThreadsObj Threads = {
    .ThreadsSize = 0,
    .Threads = 0
};

int nowThreadIn = 0;

//TODO

void MTC(int StartAt){


    nowThreadIn = StartAt;

    while(1){
        if (Threads.ThreadsSize) {
          for (; nowThreadIn < Threads.ThreadsSize; nowThreadIn++){
//         printf("Truned to THREAD %d %d\n", nowThreadIn,Threads.ThreadsSize);

            if((Threads.Threads[nowThreadIn].ObjsSize-1) >= *(Threads.Threads[nowThreadIn].IndexUP)){
                MioneReturnObj RunningReturned = {0};

                for (;;){
                    if(Threads.Threads[nowThreadIn].Fuc == mione) {
                        RunningReturned = mione(
                    Threads.Threads[nowThreadIn].Objs,
                    Threads.Threads[nowThreadIn].ObjsSize,
                          Threads.Threads[nowThreadIn]
                    );
                        break;
                    };


                    if(Threads.Threads[nowThreadIn].Fuc == Range){
                       RunningReturned = Range(
                           Threads.Threads[nowThreadIn].Objs,
                           Threads.Threads[nowThreadIn].ObjsSize,
                           Threads.Threads[nowThreadIn]
                           );
                        break;

                    }

                    if(Threads.Threads[nowThreadIn].Fuc == Function) {
                       RunningReturned = Function(
                            Threads.Threads[nowThreadIn].Objs,
                            Threads.Threads[nowThreadIn].ObjsSize,
                            Threads.Threads[nowThreadIn].Request,
                            Threads.Threads[nowThreadIn].RequestSize,
                            Threads.Threads[nowThreadIn]
                            );
                        break;
                    }

                    if(Threads.Threads[nowThreadIn].Fuc == Table) {
                        RunningReturned = Table(
                            Threads.Threads[nowThreadIn].Objs,
                            Threads.Threads[nowThreadIn].ObjsSize,
                            Threads.Threads[nowThreadIn].VariablesUP,
                            Threads.Threads[nowThreadIn].VariablesUPSizeUP,
                            Threads.Threads[nowThreadIn]
                            );
                        break;
                    }
                    break;
                }


                int States[] =  {
                    1
                };

                for (int StateIndex = 0; StateIndex<(sizeof(States)/sizeof(int)); StateIndex++)
                {
                    if (RunningReturned.ToState >= States[StateIndex])
                    {
                        RunningReturned.ToState = RunningReturned.ToState-States[StateIndex];

                        switch (States[StateIndex]){
                        case 1:
                            *(Threads.Threads[nowThreadIn].IndexUP) = (Threads.Threads[nowThreadIn].ObjsSize-1);
                            Threads.Threads[nowThreadIn].MioneReturnUP = &RunningReturned;
                            break;
                        }
                    }
                }


            }else{

                if(Threads.Threads[nowThreadIn].OrgThreadObjUP) {
                    if (Threads.Threads[nowThreadIn].AbleToRepair == 1)
                        Threads.Threads[nowThreadIn] = *Threads.Threads[nowThreadIn].OrgThreadObjUP;
                    else
                        *Threads.Threads[nowThreadIn].IndexUP = 0;
                }else
                {
                    ThreadsObj newThreads;
                    newThreads.ThreadsSize = 0;
                    newThreads.Threads = malloc(0);

                    for (int i = 0; i < Threads.ThreadsSize; i++) if(nowThreadIn!=i){
                        newThreads.ThreadsSize++;
                        newThreads.Threads = realloc(newThreads.Threads, (newThreads.ThreadsSize) * sizeof(ThreadObj));
                        newThreads.Threads[newThreads.ThreadsSize-1] = Threads.Threads[i];
                    }else
                    {
                        //todo FREE
                        free(Threads.Threads[i].Objs);
                        free(Threads.Threads[i].IndexUP);
                    }

                    free(Threads.Threads);
                    Threads = newThreads;
                };
            }
        };
        }else
        {
            free(Threads.Threads);
            return;
        };

        nowThreadIn = 0;
    }

    //printf("MTC done\n");
}

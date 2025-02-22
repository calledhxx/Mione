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

ThreadReturnObj MTC(int StartAt){
    ThreadReturnObj toReturn = {0};

    nowThreadIn = StartAt;

    while(1){
        if (Threads.ThreadsSize) {
          for (; nowThreadIn < Threads.ThreadsSize; nowThreadIn++){
//         printf("Truned to THREAD %d %d\n", nowThreadIn,Threads.ThreadsSize);

            if((Threads.Threads[nowThreadIn].ObjsSize-1) >= *(Threads.Threads[nowThreadIn].IndexUP)){
                MioneReturnObj RunningReturned;

                for (;;){
                    if(Threads.Threads[nowThreadIn].Fuc == mione) {
                        RunningReturned = Threads.Threads[nowThreadIn].Fuc(
                    Threads.Threads[nowThreadIn].Objs,
                    Threads.Threads[nowThreadIn].ObjsSize,
                          Threads.Threads[nowThreadIn]
                    );
                        break;
                    };


                    if(Threads.Threads[nowThreadIn].Fuc == Range){
                        RunningReturned = Threads.Threads[nowThreadIn].Fuc(
                           Threads.Threads[nowThreadIn].Objs,
                           Threads.Threads[nowThreadIn].ObjsSize,
                           Threads.Threads[nowThreadIn]
                           );
                        break;

                    }

                    if(Threads.Threads[nowThreadIn].Fuc == Function) {
                        RunningReturned = Threads.Threads[nowThreadIn].Fuc(
                            Threads.Threads[nowThreadIn].Objs,
                            Threads.Threads[nowThreadIn].ObjsSize,
                            Threads.Threads[nowThreadIn].Request,
                            Threads.Threads[nowThreadIn].RequestSize,
                            Threads.Threads[nowThreadIn]
                            );
                        break;
                    }

                    if(Threads.Threads[nowThreadIn].Fuc == Table) {
                        RunningReturned = Threads.Threads[nowThreadIn].Fuc(
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
                            toReturn.Return = RunningReturned;
                            return toReturn;
                        }
                    }
                }


            }else{

                toReturn.Return = Threads.Threads[nowThreadIn].Return;
                if(Threads.Threads[nowThreadIn].isChild) {
//                    printf("THREAD %d return to Parent\n", nowThreadIn);
                    return toReturn;
                };
//                printf("THREAD %d is done\n", nowThreadIn);

                ThreadsObj newThreads;
                newThreads.ThreadsSize = 0;
                newThreads.Threads = malloc(0);

                for (int i = 0; i < Threads.ThreadsSize; i++) if(nowThreadIn!=i){
                    newThreads.ThreadsSize++;
                    newThreads.Threads = realloc(newThreads.Threads, (newThreads.ThreadsSize) * sizeof(ThreadObj));
                    newThreads.Threads[newThreads.ThreadsSize-1] = Threads.Threads[i];
                }

                Threads = newThreads;

                return toReturn;
            }
        };
        }else return toReturn;

        nowThreadIn = 0;
    }

    //printf("MTC done\n");
}

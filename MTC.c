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

ThreadReturnObj MTC(){
    ThreadReturnObj toReturn = {0};

    while(1){
        nowThreadIn = 0;

        for (; nowThreadIn < Threads.ThreadsSize; nowThreadIn++){

            if((Threads.Threads[nowThreadIn].ObjsSize-1) > *(Threads.Threads[nowThreadIn].IndexUP)){

                MioneReturnObj RunningReturned;

                if(Threads.Threads[nowThreadIn].Fuc == mione) RunningReturned = Threads.Threads[nowThreadIn].Fuc(
                Threads.Threads[nowThreadIn].Objs,
                Threads.Threads[nowThreadIn].ObjsSize,
                      Threads.Threads[nowThreadIn]
                );

                if(Threads.Threads[nowThreadIn].Fuc == Range) RunningReturned = Threads.Threads[nowThreadIn].Fuc(
                      Threads.Threads[nowThreadIn].Objs,
                      Threads.Threads[nowThreadIn].ObjsSize
                      );


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
                ThreadsObj newThreads;
                newThreads.ThreadsSize = 0;
                newThreads.Threads = malloc(0);
                for (int i = 0; i < Threads.ThreadsSize; i++) if(nowThreadIn!=i){
                    newThreads.ThreadsSize++;
                    newThreads.Threads = realloc(newThreads.Threads, (newThreads.ThreadsSize) * sizeof(ThreadObj));
                    newThreads.Threads[newThreads.ThreadsSize-1] = Threads.Threads[i];
                }

                toReturn.Return = Threads.Threads[nowThreadIn].Return;

                Threads = newThreads;

                return toReturn;
            }
        }
    }
}

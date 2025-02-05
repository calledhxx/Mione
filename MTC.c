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

void MTC(){
    for (; nowThreadIn < Threads.ThreadsSize; nowThreadIn++){
      if(Threads.Threads[nowThreadIn].ObjsSize > Threads.Threads[nowThreadIn].Index){

        if(Threads.Threads[nowThreadIn].Fuc == mione) Threads.Threads[nowThreadIn].Fuc(
        Threads.Threads[nowThreadIn].Objs,
        Threads.Threads[nowThreadIn].ObjsSize,
              Threads.Threads[nowThreadIn]
        );

      }else{
          ThreadsObj newThreads;
          newThreads.Threads = malloc(0);
          for (int i = 0; i < Threads.ThreadsSize; i++) if(nowThreadIn!=i){
              newThreads.ThreadsSize++;
              newThreads.Threads = realloc(newThreads.Threads, ( newThreads.ThreadsSize) * sizeof(ThreadObj));
              newThreads.Threads[ newThreads.ThreadsSize-1] = Threads.Threads[i];
          }
      }
    }
}
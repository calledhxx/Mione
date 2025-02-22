//
// Created by calle on 24-11-16.
//



#include <stdio.h>
#include "main.h"




void ErrCall(char*Reason,char*ErrCode,char*Tip,int Line,int Column){
    printf("\n");

    printf("\033[1;37;45m" " # MIONE FAILED :( \033[1;35;43m %s " "\033[0m" "\n",ErrCode);
    if (Reason) printf("    \033[1;37;41m" " $ REASON  " "\033[1;37;46m" " %s " "\033[0m" "\n",Reason);
    if (Line) printf("    \033[1;37;41m" " $ LINE "  "\033[1;37;46m" " %d " "\033[0m" "\n" ,Line);
    if (Column) printf("    \033[1;37;41m" " $ COLUMN "  "\033[1;37;46m" " %d " "\033[0m" "\n" ,Column);
    if (Tip) printf("    \033[1;37;42m" " @ TIP "  "\033[1;36;40m" " %s " "\033[0m" "\n" ,Tip);
    printf("\033[1;37;45m" " MIONE " "\033[1;37;44m" " %s " "\033[0m" "\n",MIONE_VERSION);

    exit(1);
}



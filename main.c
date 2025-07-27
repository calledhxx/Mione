#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "OBJECTS.h"
#include "FILE_TO_CASE.h"
#include "CASE_TO_MIONE.h"
#include "IMPLEMENT.h"
#include "MIONE.h"

/// 每個EXIT函數對應著那個地方需做錯誤回傳，其呼叫者須做ERROR HANDLE。

ScopeObj MainScope = {0};


int main(const int OptionsSize,char **Options)
{
    FILE *f = fopen("D:\\Mione\\index.mio", "r"); //never read binary again...


    if (f == NULL) exit(-1);

    CaseObjCarrier CaseCarrier = FCO(f,0);

    MioneObjCarrier MioneCarrier = CMO(CaseCarrier,&MainScope);

    MioneSectionObjCarrier Built =ToMione(MioneCarrier);

    ImplementedObj Implement = IMPLEMENT((ToImplementObj){
        .Built = Built
    });

    printf("Hello, Mione!\n");

    return 0;
}

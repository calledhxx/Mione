#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "STDMIO.h"

/// 每個EXIT函數對應著那個地方需做錯誤回傳，其呼叫者須做ERROR HANDLE。

ScopeObj MainScope = {0};

int main(const int OptionsSize,char **Options)
{
    FILE *f = fopen("D:\\Mione\\index.mio", "r"); //never read binary again...

    if (f == NULL) return -1;

    const EventObj FCOReturn = FCO(f);
    //第一步，先將source code轉為case物件。

    if (FCOReturn.ToState & EVENT_ERROR)
        return -1;

    const EventObj CMOReturn = CMO(FCOReturn.CaseCarrier,&MainScope);
    //第二步，將case物件轉為Mione物件。

    if (CMOReturn.ToState & EVENT_ERROR)
        return -1;

    const EventObj ToMioneReturn = ToMione(CMOReturn.MioneCarrier);
    //第三步，將Mione物件轉為程式句。

    if (ToMioneReturn.ToState & EVENT_ERROR)
        return -1;

    const EventObj IMPLEMENTReturn = IMPLEMENT((ToImplementObj){
        .Built = ToMioneReturn.TrainCarrier
    });
    //第四步，執行程式句。

    if (IMPLEMENTReturn.ToState & EVENT_ERROR)
        return -1;

    printf("Hello, Mione!\n");
    //正確執行完的回應

    return 0;
}

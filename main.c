#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "STDMIO.h"

/// 每個EXIT函數對應著那個地方需做錯誤回傳，其呼叫者須做ERROR HANDLE。

ScopeObj MainScope = {0};

int main(const int OptionsSize,char **Options)
{
    FILE *f = fopen("D:\\Mione\\index.mio", "r"); //never read binary again...

    MainScope.ChildrenScopePtrCarrierPointer = malloc(sizeof(ScopeObjPtrCarrier));
    *MainScope.ChildrenScopePtrCarrierPointer = (ScopeObjPtrCarrier){0};

    if (f == NULL) return -1;

    const FCOFunctionRespondObj FCOReturn = FCO(f);
    //第一步，先將source code轉為case物件。

    if (FCOReturn.Event.Code)
        return 1;

    const CMOFunctionRespondObj CMOReturn = CMO(FCOReturn.CaseCarrier,&MainScope);
    //第二步，將case物件轉為Mione物件。

    if (CMOReturn.Event.Code)
        return 2;

    const MIONEFunctionRespondObj ToMioneReturn = ToMione(CMOReturn.MioneCarrier);
    //第三步，將Mione物件轉為程式句。

    if (ToMioneReturn.Event.Code)
        return 3;

    const IMPLEMENTFunctionRespondObj IMPLEMENTReturn = IMPLEMENT((ToImplementObj){
        .Built = ToMioneReturn.TrainCarrier
    });
    //第四步，執行程式句。

    if (IMPLEMENTReturn.Event.Code)
        return 4;

    printf("Hello, Mione!\n");
    //正確執行完的回應

    return 0;
}

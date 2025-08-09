#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "STDMIO.h"

/// 每個EXIT函數對應著那個地方需做錯誤回傳，其呼叫者須做ERROR HANDLE。

ScopeObj MainScope = {0};


int main(const int OptionsSize,char **Options)
{
    FILE *f = fopen("D:\\Mione\\index.mio", "r"); //never read binary again...

    if (f == NULL) exit(-1);

    MainScope.VariablePtrCarrier.CarrierLen = 0;
    MainScope.VariablePtrCarrier.Carrier = NULL;
    //

    const CaseObjCarrier CaseCarrier = FCO(f,0);
    //第一步，先將source code轉為case物件。

    const MioneObjCarrier MioneCarrier = CMO(CaseCarrier,&MainScope);
    //第二步，將case物件轉為Mione物件。

    const TrainObjCarrier Built = ToMione(MioneCarrier);
    //第三步，將Mione物件轉為程式句。

    const ImplementedObj Implement = IMPLEMENT((ToImplementObj){
        .Built = Built
    });
    //第四步，執行程式句。

    printf("Hello, Mione!\n");
    //正確執行完的回應

    return 0;
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "STDMIO.h"

/// 每個EXIT函數對應著那個地方需做錯誤回傳，其呼叫者須做ERROR HANDLE。

#ifndef WIN32
#error "NO"
#endif

HeadObjCarrier HeadList = {0};
SymbolObjCarrier SymbolList = {0};
PromptObjCarrier PromptList = {0};
WeldObjCarrier WeldList = {0};

//cl main.c FILE_TO_CASE.c CASE_TO_MIONE.c HeadFile/SET.c HeadFile/SVV.c HeadFile/GET.c HeadFile/ENTER.c HeadFile/LIBRARY.c HeadFile/INCLUDE.c HeadFile/RETURN.c HeadFile/IF.c HeadFile/FOR.c HeadFile/WHILE.c HeadFile/AllHeads.c SYMBOL_DEF.c RESOURCE.c MIONE.c CONTAINER.c PROMPT_DEF.c IMPLEMENT.c COMPUTATION.c VARIABLE_PROCESSING.c EVENT_HANDLER.c WeldFile/AllWeld.c WeldFile/SUBJECT.c PASSENGERS.c /Fe:Mione.exe

int main(const int OptionsSize,char **Options)
{
    ScopeObj MainScope = {0};

    FILE *f = fopen("D:/Mione/index.mio", "r"); //never read binary again...

    MainScope.ChildrenScopePtrCarrierPointer = malloc(sizeof(ScopeObjPtrCarrier));
    *MainScope.ChildrenScopePtrCarrierPointer = (ScopeObjPtrCarrier){0};

    HeadList = ReturnHeadList();
    SymbolList = ReturnSymbolList();
    PromptList = ReturnPromptList();
    WeldList = ReturnWelds();

    if (f == NULL) return -1;

    const EventObj EventTemplate = (EventObj){
        .Address = "D:/Mione/index.mio"
    };

    const FCOFunctionRespondObj FCOReturn = FCO(
        (FCOFunctionRequestObj){
            .f = f,
            .EventTemplate = EventTemplate
        });
    //第一步，先將source code轉為case物件。

    MainEventHandler(FCOReturn.Event);



    const CMOFunctionRespondObj CMOReturn = CMO((CMOFunctionRequestObj){
        .EventTemplate = EventTemplate,
        .CassCarrier = FCOReturn.CaseCarrier,
        .ScopePointer = &MainScope
    });
    //第二步，將case物件轉為Mione物件。

    MainEventHandler(CMOReturn.Event);


    const MIONEFunctionRespondObj ToMioneReturn = ToMione((MIONEFunctionRequestObj){
        .MioneCarrier = CMOReturn.MioneCarrier,
        .EventTemplate = EventTemplate
    });
    //第三步，將Mione物件轉為程式句。

    MainEventHandler(ToMioneReturn.Event);



    const IMPLEMENTFunctionRespondObj IMPLEMENTReturn = IMPLEMENT((IMPLEMENTFunctionRequestObj){
        .EventTemplate = EventTemplate,
        .Built = ToMioneReturn.TrainCarrier,
        .CallByValueCarrier = (ValueObjCarrier){0}
    });
    //第四步，執行程式句。

    MainEventHandler(IMPLEMENTReturn.Event);


    printf("Hello, Mione!\n");
    //正確執行完的回應

    free(MainScope.ChildrenScopePtrCarrierPointer);

    return 0;
}
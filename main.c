#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "STDMIO.h"

#ifndef WIN32
#error "NO"
#endif

HeadObjCarrier HeadList = {0};
SymbolObjCarrier SymbolList = {0};
PromptObjCarrier PromptList = {0};
WeldObjCarrier WeldList = {0};

///cl main.c FILE_TO_CASE.c CASE_TO_MIONE.c HeadFile/SET.c HeadFile/SVV.c HeadFile/GET.c HeadFile/ENTER.c HeadFile/LIBRARY.c HeadFile/INCLUDE.c HeadFile/RETURN.c HeadFile/IF.c HeadFile/FOR.c HeadFile/WHILE.c HeadFile/AllHeads.c SYMBOL_DEF.c RESOURCE.c MIONE.c CONTAINER.c PROMPT_DEF.c IMPLEMENT.c COMPUTATION.c VARIABLE_PROCESSING.c EVENT_HANDLER.c WeldFile/AllWeld.c WeldFile/SUBJECT.c PASSENGERS.c MEMORY.c /Fe:Mione.exe

int main(const int OptionsSize,char **Options)
{
    // CreateLine();
    // Allocate(0,0);

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

    //第一步，先將source code轉為case物件。
    const FCOFunctionRespondObj FCOReturn = FCO(
        (FCOFunctionRequestObj){
            .f = f,
            .EventTemplate = EventTemplate
        });

    fclose(f);

    MainEventHandler(FCOReturn.Event);

    //第二步，將case物件轉為Mione物件。
    const CMOFunctionRespondObj CMOReturn = CMO((CMOFunctionRequestObj){
        .EventTemplate = EventTemplate,
        .CassCarrier = FCOReturn.CaseCarrier,
        .ScopePointer = &MainScope
    });

    free(FCOReturn.CaseCarrier.Carrier);

    MainEventHandler(CMOReturn.Event);

    //第三步，將Mione物件轉為程式句。
    const MIONEFunctionRespondObj ToMioneReturn = ToMione((MIONEFunctionRequestObj){
        .MioneCarrier = CMOReturn.MioneCarrier,
        .EventTemplate = EventTemplate
    });

    MainEventHandler(ToMioneReturn.Event);

    //第四步，執行程式句。
    const IMPLEMENTFunctionRespondObj IMPLEMENTReturn = IMPLEMENT((IMPLEMENTFunctionRequestObj){
        .EventTemplate = EventTemplate,
        .Built = ToMioneReturn.TrainCarrier,
        .CallByValueCarrier = (ValueObjCarrier){0}
    });

    MainEventHandler(IMPLEMENTReturn.Event);



    // printf("Hello, Mione!\n");
    //正確執行完的回應

    free(MainScope.ChildrenScopePtrCarrierPointer);

    return 0;
}
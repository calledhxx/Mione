#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "OBJECTS.h"
#include "FILE_TO_CASE.h"
#include "CASE_TO_MIONE.h"
#include "IMPLEMENT.h"
#include "MIONE.h"


ScopeObj MainSVU = {0};

int main(const int OptionsSize,char **Options)
{
    FILE *f = _wfopen(L"D:\\Mione\\index.mio",L"r");

    if (f == NULL) exit(-1);

    CaseObjCarrier CaseCarrier = FCO(f);

    MioneObjCarrier MioneCarrier = CMO(CaseCarrier,1,0,&MainSVU);

    MioneSectionObjCarrier Built =ToMione(MioneCarrier);

    ImplementedObj Implement = IMPLEMENT((ToImplementObj){
        .Built = Built
    });

    /*
     11110000 00001111
     11111111 00001111
     11111111 11110000
     00001111 11110000
     A <=> B
     */

    printf("Hello, Mione!\n");

    return 0;
}

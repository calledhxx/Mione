#include <stdint.h>
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
    FILE *f = NULL;

    f = _wfopen(L"D:\\Mione\\index.mio",L"r");

    if (f == NULL) exit(-1);

    CaseObjCarrier CaseCarrier = FCO(f);

    MioneObjCarrier MioneCarrier = CMO(CaseCarrier,1,0,&MainSVU);

    MioneSectionObjCarrier Built =ToMione(MioneCarrier);

    IMPLEMENT((ToImplementObj){
        .Built = Built
    });

    return 0;
}

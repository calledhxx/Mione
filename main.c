#include <stdint.h>
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

    if (f != NULL)
    {
        int CaseObjSize = 0;
        CaseObj * CASES = FCO(f,&CaseObjSize);

        int MioObjSize = 0;
        MioneObj * MioObj = CMO(CASES,CaseObjSize,&MioObjSize,1,0,&MainSVU);

        MioneBuiltObj Built =ToMione((MioneToBuildObj){
            .Objs = MioObj,
            .ObjsSize = MioObjSize
        });

        IMPLEMENT((ToImplementObj){
            .Built = Built
        });

    }
    return 0;
}

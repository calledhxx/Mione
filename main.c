


#ifdef _WIN32

#endif

#ifdef __APPLE__

#endif

#ifdef __linux__

#endif

#include "main.h"
#include <unistd.h>

#include <wchar.h>



ScopeObj MainSVU = {0};

int main(const int OptionsSize,char **Options)
{
    FILE *f = NULL;

    f = _wfopen(L"D:\\Mione\\index.mio",L"r");

    if (f != NULL)
    {
        //MAIN
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


        printf("\n # Well done. Have a good day.");

    }else
    {
        //todo  
    }

    return 0;
}

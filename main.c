


#ifdef _WIN32

#endif

#ifdef __APPLE__

#endif

#ifdef __linux__

#endif

#include "main.h"
#include <unistd.h>



ScopeVariableUPsObj MainSVU = {0};

int main(const int OptionsSize,char **Options)
{
    FILE *f = NULL;

    f = fopen("D:\\Mione\\index.mio","r");

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

        printf("\n # Well down. Have a good day.");

    }else
    {
        //todo  
    }

    return 0;
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "OBJECTS.h"
#include "FILE_TO_CASE.h"
#include "CASE_TO_MIONE.h"
#include "IMPLEMENT.h"
#include "MIONE.h"


ScopeObj MainSVU = {0};

int main(const int OptionsSize,char **Options)
{
    FILE *f = 0;

    if (OptionsSize == 2 && strcmp(Options[1],"line")==0)
    {
        f = stdin;

        if (f == NULL) exit(-1);

        while (1)
        {
            fwrite(">>  ",1,3,stdout);

            CaseObjCarrier CaseCarrier = FCO(f,1);

            MioneObjCarrier MioneCarrier = CMO(CaseCarrier,1,0,&MainSVU);

            MioneSectionObjCarrier Built = ToMione(MioneCarrier);

            ImplementedObj Implement = IMPLEMENT((ToImplementObj){
                .Built = Built
            });
        }
    }
    else
    {
        f = _wfopen(L"D:\\Mione\\index.mio", L"r"); //never read binary again...

        if (f == NULL) exit(-1);

        CaseObjCarrier CaseCarrier = FCO(f,0);

        MioneObjCarrier MioneCarrier = CMO(CaseCarrier,1,0,&MainSVU);

        MioneSectionObjCarrier Built =ToMione(MioneCarrier);

        ImplementedObj Implement = IMPLEMENT((ToImplementObj){
            .Built = Built
        });
    }


    printf("Hello, Mione!\n");

    return 0;
}

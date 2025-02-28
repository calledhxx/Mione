


#ifdef _WIN32

#endif

#ifdef __APPLE__

#endif

#ifdef __linux__

#endif

#include "main.h"


//
    //

DefineVariableObj * Dvo;
int DvoSize;

int main(const int OptionsSize,char **Options)
{
    FILE *f = NULL;

    if (OptionsSize>1)
    {
        int Mode = 0;
        for (int i = 0;i<OptionsSize;i++)
        {
            int isChoosingMode = Options[i][0]=='-';

            if (Mode)
            {
                switch (Mode)
                {
                case 1:
                    f = fopen(Options[i],"r");
                    if (f){}else
                    {
                        ErrCall("No file found","FNF",NULL,0,0);
                    }
                    break;
                }
                Mode = 0;
            }

            if (isChoosingMode)
            {
                if (strcmp(strlwr(Options[i]),"-f")==0)  Mode = 1;
            }
        }
    }else
    {
        f = fopen("D:\\Mione\\index.mio","r");
    }


    if (f != NULL)
    {
        //MAIN
        int CaseObjSize = 0;
        CaseObj * CASES = FCO(f,&CaseObjSize);

        int MioObjSize = 0;

        Dvo = malloc(0);
        DvoSize = 0;

        MioneObj * MioObj = CMO(CASES,CaseObjSize,&MioObjSize,1,0,&Dvo,&DvoSize);

        mione(MioObj,MioObjSize);

        printf("\n # Well down. Have a good day.");

    }else
    {
        //todo  
    }

    return 0;
}




#ifdef _WIN32

#endif

#ifdef __APPLE__

#endif

#ifdef __linux__

#endif

#include "main.h"

//

#define HEAD 1
#define PROMPT 2
#define SYMBOL 3
#define VALUE 4
#define VARIABLE  5

#define VALUE_STRING_TYPE 1
#define VALUE_NOPOINTNUMBER_TYPE 2
#define VALUE_POINTNUMBER_TYPE 3
#define VALUE_FUNCTION_TYPE 4
#define VALUE_RANGE_TYPE 5
#define VALUE_LIGHTS_TYPE 6
#define VALUE_DB_TYPE 7

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
                        ErrCall("No file found","M001",NULL,0,0);
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

    }else
    {
        //todo  
    }

    return 0;
}

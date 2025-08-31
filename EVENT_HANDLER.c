#include "STDMIO.h"

void MainEventHandler(const EventObj Event)
{
    if (Event.Code)
    {
        fwrite("[Mione failed] ",1,15,stdout);

        switch (Event.Code)
        {
        case EVENT_ERROR_NO_FILE_FOUND:
            {
                printf("%d ",Event.Code);

                fwrite("[Mione failed] ",1,15,stdout);

                break;
            }
        default:
            exit(1);
        }
    }else
        return;
}
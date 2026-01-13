#include "STDMIO.h"



void MainEventHandler(const EventObj Event)
{
    switch (Event.Code)
    {
    case 0:
        return;
    case EVENT_ERROR_UNKNOWN_SUPER_CHAR_PARENT:
        {
            printf(
                "%s:%d:%d~%d:%d %d \"%s\"",
                   Event.Address,
                   Event.EventPosition.CaseStartLine,
                   Event.EventPosition.CaseStartColumn,
                   Event.EventPosition.CaseEndLine,
                   Event.EventPosition.CaseEndColumn,
                   Event.Code,
                   Event.Message
                   );

            break;
        }
    default:
        exit(10);
    }


    exit(-123);
}
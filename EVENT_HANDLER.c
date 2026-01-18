#include "STDMIO.h"



void MainEventHandler(const EventObj Event)
{
    switch (Event.Code)
    {
    case 0:
        return;
    case EVENT_SUPER_CHAR_ERROR:
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
    case EVENT_NO_FILE_FOUND:
        {
            printf(
                "%d \"%s\"\n",
                Event.Code,
                Event.Message
                );

            break;
        }
    case EVENT_MO_ERROR:
        {
            printf(
                "%s %d \"%s\"\n",
                Event.Address,
                Event.Code,
                Event.Message
                );
            break;
        }
    case EVENT_LAYOUT_ERROR:
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
        {
            printf("EVENT NOT PROCESSED.\n");
            exit(10);
        }
    }

    printf("--EVENT HANDLER\n");
    exit(-123);
}
#include "object.h"
#include "weld.h"

static keyword_t WeldKeyword[] = {
    {
        .name = "subject",
        .token = TOKEN_WELD_SUBJECT,
    },
    {0}
};
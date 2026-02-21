#include "main.h"


static keyword_t WeldKeywordArray[] = {
    {
        .name = "subject",
        .token = TOKEN_WELD_SUBJECT,
    },
    {0}
};

keyword_t * WeldKeyword = WeldKeywordArray;
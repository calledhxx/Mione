#include "object.h"
#include "head.h"

static keyword_t HeadKeywordArray[] = {
    {
        .name = "set",
        .token = TOKEN_HEAD_SET,
    },
{0}
};

keyword_t * HeadKeyword = HeadKeywordArray;
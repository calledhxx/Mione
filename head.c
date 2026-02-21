#include "main.h"

static keyword_t HeadKeywordArray[] = {
    {
        .name = "set",
        .token = TOKEN_HEAD_SET,
        .instance = &(head_t){

        }
    },
{0}
};

keyword_t * HeadKeyword = HeadKeywordArray;
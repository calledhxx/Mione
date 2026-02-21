#include "main.h"

static  keyword_t PromptKeywordArray[] = {
    {
        .name = "=",
        .token = TOKEN_PROMPT_EQUAL,
    },
{0}
};

keyword_t * PromptKeyword = PromptKeywordArray;
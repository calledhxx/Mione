#ifndef HEAD_H
#define HEAD_H

extern keyword_t * HeadKeyword;

typedef struct head_s
{
    void * (*function)(void *);
} head_t;

extern head_t token_to_head(const int token);

#endif

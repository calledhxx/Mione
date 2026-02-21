#ifndef WORD_H
#define WORD_H

#include <stdio.h>

enum character_type_e
{
    CHAR_NULL = 0,
    CHAR_NORMAL = 1,
    CHAR_NUMBER = 2,
    CHAR_DQ = 3,
    CHAR_SQ = 4,
    CHAR_CONNECTABLE = 9,
    CHAR_UNCONNECTABLE = 10,
    CHAR_SPACE = 11,
    CHAR_BS = 12,
    CHAR_NEWLINE = 13,
    CHAR_SEMICOLON = 14
};

enum word_type_e
{
    WORD_NORMAL,
    WORD_SINGLE_STRING,
    WORD_DOUBLE_STRING,
    WORD_DECNUMBER,
    WORD_HEXNUMBER,
    WORD_BINNUMBER,
    WORD_CONNECTABLE,
    WORD_UNCONNECTABLE,
    WORD_BREAKER,
} ;

typedef struct word_s
{
    char * word;
    enum word_type_e word_type;
} word_t;

typedef struct word_carrier_s
{
    word_t * words;
    unsigned words_length;
} word_carrier_t;

word_carrier_t stream_to_word(FILE * F,char END_FLAG);


#endif

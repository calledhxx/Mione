#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

static const char Connectable[] = {
    '*',
    '/',
    '+',
    '=',
    '^',
    '<',
    '>',
    '.'

};

static const char Unconnectable[] = {
    '(',
    ')',
    '[',
    ']',
    '{',
    '}',
    '-',
    ',',
    ':',
    '@',
};

enum word_type_e CheckCharType(const char Char)
{
    if (Char == EOF || Char == 0) return CHAR_NULL;

    if ((Char >= 'a' && Char <= 'z') || (Char >= 'A' && Char <= 'Z')) return CHAR_NORMAL;

    if (Char >= '0' && Char <= '9') return CHAR_NUMBER;

    if (Char == '"') return CHAR_DQ;

    if (Char == '\'') return CHAR_SQ;

    for (int i = 0; i < sizeof(Connectable)/sizeof(Connectable[0]); i++)
        if (Connectable[i] == Char)  return CHAR_CONNECTABLE;

    for (int i = 0; i < sizeof(Unconnectable)/sizeof(Unconnectable[0]); i++)
        if (Unconnectable[i] == Char) return CHAR_UNCONNECTABLE;

    if (Char == ' ' || Char == '\t') return CHAR_SPACE;

    if (Char == '\\') return CHAR_BS;

    if (Char == '\n') return CHAR_NEWLINE;

    if (Char == ';') return CHAR_SEMICOLON;

    return 1;
}

word_carrier_t stream_to_word(FILE * F,const char END_FLAG)
{
    word_carrier_t CaseCarriers = {0};

    char * CaseName = NULL;
    unsigned int CaseNameLen = 0;

    unsigned int CharIndex = 0; //字元的偏移位

    char ThisChar = 0; //目前字元
    char LastChar = 0; //上個字元

    enum word_type_e ThisCharType = 0; //目前字元類型 包括被覆蓋的類型
    enum word_type_e LastCharType = 0; //上個字元類型 包括被覆蓋的類型

    char HandleType = 0; //處理類型

    char StringHandleChar = 0; //字串類型 `'` 與 `"` 的編碼

    unsigned int CaseStartLine = 1; //Case開始行號
    unsigned int CaseStartColumn = 1; //Case開始列號

    unsigned int ProcessingLine = 1;
    unsigned int ProcessingColumn = 1;

    // Super Character 處理
    unsigned int SuperCharHandlerIndex = 0; //Super Character 控制字元(即 `\` )的位置

    char * SuperCharParentName = NULL;
    unsigned int SuperCharParentNameLen = 0;

    char SuperCharParentType = 0; //Super Character Parent 的類型

    char * SuperCharChildName = NULL;
    unsigned int SuperCharChildNameLen = 0;

    char SuperCharCollect = 0; //Super Character 的收集狀態。1 : 已收集完Parent ; 2 : 已收集完Child

    char * toCMPChar = 0; //Super Character Parent名稱判斷
    // !

    // Number 特殊處理
    char RecordingNumber = 0; //正在紀錄數字 在有表達數字時 0表一般數字; 2表二進制; 3表十六進制;
    char DotExist = 0; //紀錄小數點是否存在
    // !

    // 註解
    char CommentChar = 0; //註解類型，以註解開頭的第二符號為主要記錄對象。
    // !

    do
    {
        ThisChar = fgetc(F);
        ThisCharType = CheckCharType(ThisChar);

        switch (HandleType) //串一定要有休止符 才能使用Handle處裡 如：`"Hello World"`的尾端 `"`
        {
        case 0: //一般
            {
                //自元分節
                if (CharIndex)
                    if (!(
                        (LastCharType == CHAR_CONNECTABLE && LastChar == '.' && CaseNameLen == 1 && ThisCharType == CHAR_NUMBER && !DotExist)
                        ||
                        (LastCharType == CHAR_NUMBER && ThisChar == '.' && !DotExist)
                        ||
                        (LastCharType == CHAR_NORMAL && ThisCharType == CHAR_NUMBER)
                        ||
                        (LastCharType == CHAR_NUMBER && LastChar == '0' && CaseNameLen == 1 && ThisCharType == CHAR_NORMAL && (ThisChar == 'x' || ThisChar == 'b'))
                        ||
                        (RecordingNumber == 2 && ThisCharType == CHAR_NORMAL)
                    ))
                        if (CaseNameLen && LastCharType != ThisCharType){
                            unsigned int CaseType = WORD_NORMAL;

                            switch (LastCharType)
                            {
                            case CHAR_NUMBER:
                                {
                                    if (RecordingNumber == 0)
                                        CaseType = WORD_DECNUMBER;
                                    else if (RecordingNumber == 1)
                                        CaseType = WORD_BINNUMBER;
                                    else if (RecordingNumber == 2)
                                        CaseType = WORD_HEXNUMBER;

                                    break;
                                }
                            case CHAR_CONNECTABLE:
                                {
                                    CaseType = WORD_CONNECTABLE;

                                    break;
                                }
                            case CHAR_SEMICOLON:
                                {
                                    CaseType = WORD_BREAKER;


                                    break;
                                }

                                default:break;
                            }

                            CaseNameLen++;
                            CaseName = realloc(
                                CaseName,
                                CaseNameLen*sizeof(char)
                                );
                            CaseName[CaseNameLen-1] = 0;



                            CaseCarriers.words_length++;
                            CaseCarriers.words = realloc(
                                CaseCarriers.words,
                                CaseCarriers.words_length*sizeof(word_t)
                            );
                            CaseCarriers.words[CaseCarriers.words_length-1] = (word_t){
                                .word = CaseName,
                                .word_type = CaseType,
                            };


                            RecordingNumber = 0; //重置數字紀錄
                            DotExist = 0;

                            CaseNameLen = 0;
                            // free(CaseName);
                            CaseName = NULL;
                        }

                //單字元處裡
                switch (ThisCharType)
                {
                case CHAR_NULL: break;
                case CHAR_NORMAL:
                    {
                        if (!CaseNameLen)
                        {
                            CaseStartColumn = ProcessingColumn;
                            CaseStartLine = ProcessingLine;
                        }

                        if (
                            LastCharType == CHAR_NUMBER && LastChar == '0' && CaseNameLen == 1
                            &&
                            (ThisChar == 'x' || ThisChar == 'b')
                            )
                        {
                            RecordingNumber = ThisChar == 'x' ? 2 : 1;
                            ThisCharType = CHAR_NUMBER;

                        }else
                        {
                            CaseNameLen++;
                            CaseName = realloc(
                                CaseName,
                                CaseNameLen*sizeof(char)
                                );
                            CaseName[CaseNameLen-1] = ThisChar;

                        }


                        if (RecordingNumber == 2)
                            if (
                                (ThisChar >= 'A' && ThisChar <= 'F')
                                ||
                                (ThisChar >= 'a' && ThisChar <= 'f')
                                )
                                ThisCharType = CHAR_NUMBER;

                        break;
                    }
                case CHAR_NUMBER:
                    {
                        if (!CaseNameLen)
                        {
                            CaseStartColumn = ProcessingColumn;
                            CaseStartLine = ProcessingLine;
                        }

                        if (LastCharType == CHAR_CONNECTABLE && CaseNameLen == 1 && LastChar == '.' && !DotExist)
                            DotExist = 1;

                        if (LastCharType == CHAR_NORMAL)
                            ThisCharType = CHAR_NORMAL;

                        if (RecordingNumber == 1)
                            if (!(ThisChar == '0' || ThisChar == '1'))
                            {
                                CaseNameLen++;
                                CaseName = realloc(
                                    CaseName,
                                    CaseNameLen*sizeof(char)
                                    );
                                CaseName[CaseNameLen-1] = 0;

                                CaseCarriers.words_length++;
                                CaseCarriers.words = realloc(
                                    CaseCarriers.words,
                                    CaseCarriers.words_length*sizeof(word_t)
                                );

                                CaseCarriers.words[CaseCarriers.words_length-1] = (word_t){
                                    .word = CaseName,
                                    .word_type = WORD_DECNUMBER,
                                };



                                RecordingNumber = 0; //重置數字紀錄

                                CaseNameLen = 0;
                                //free(CaseName);
                                CaseName = NULL;
                            }


                        CaseNameLen++;
                        CaseName = realloc(
                            CaseName,
                            CaseNameLen*sizeof(char)
                            );
                        CaseName[CaseNameLen-1] = ThisChar;

                        break;
                    }

                case CHAR_CONNECTABLE:
                    {
                        if (LastChar == '/')
                            if (ThisChar == '/' || ThisChar == '*')
                            {
                                HandleType = 3;
                                CommentChar = ThisChar;

                                CaseNameLen = 0;
                                //free(CaseName);
                                CaseName = NULL;

                                continue;
                            }

                        if (!CaseNameLen)
                        {
                            CaseStartColumn = ProcessingColumn;
                            CaseStartLine = ProcessingLine;
                        }

                        CaseNameLen++;
                        CaseName = realloc(
                            CaseName,
                            CaseNameLen*sizeof(char)
                            );
                        CaseName[CaseNameLen-1] = ThisChar;


                        if (ThisChar == '.' && !DotExist && LastCharType == CHAR_NUMBER && CaseNameLen != 1)
                        {
                            DotExist = 1;
                            ThisCharType = CHAR_NUMBER;
                        }


                        break;
                    }

                case CHAR_UNCONNECTABLE:
                    {
                        CaseStartColumn = ProcessingColumn;
                            CaseStartLine = ProcessingLine;

                            CaseNameLen+=2;
                            CaseName = realloc(
                                CaseName,
                                CaseNameLen*sizeof(char)
                                );

                            CaseName[0] = ThisChar;
                            CaseName[1] = 0;

                            CaseCarriers.words_length++;
                            CaseCarriers.words = realloc(
                                CaseCarriers.words,
                                CaseCarriers.words_length*sizeof(word_t)
                            );

                            CaseCarriers.words[CaseCarriers.words_length-1] = (word_t){
                                .word = CaseName,
                                .word_type = WORD_UNCONNECTABLE,
                            };


                            CaseNameLen = 0;
                            //free(CaseName);
                            CaseName = NULL;

                        break;
                    }

                case CHAR_SEMICOLON:
                    {
                        CaseNameLen++;
                        CaseName = realloc(
                            CaseName,
                            CaseNameLen*sizeof(char)
                            );
                        CaseName[CaseNameLen-1] = ThisChar;

                        break;
                    }


                //字串Handler
                case CHAR_DQ:
                case CHAR_SQ:
                    {
                        HandleType = 1; //開始處理字串
                        StringHandleChar = ThisChar;

                        break;
                    }
                default:break;
                }

                break;
            }
        case 1: //字串
            {
                //單字元處裡
                switch (ThisCharType)
                {
                case CHAR_DQ:
                case CHAR_SQ:
                    {

                        if (StringHandleChar == ThisChar)
                        {
                            HandleType = 0;

                            CaseNameLen++;
                            CaseName = realloc(
                                CaseName,
                                CaseNameLen*sizeof(char)
                                );
                            CaseName[CaseNameLen-1] = 0;

                            CaseCarriers.words_length++;
                            CaseCarriers.words = realloc(
                                CaseCarriers.words,
                                CaseCarriers.words_length*sizeof(word_t)
                            );

                            CaseCarriers.words[CaseCarriers.words_length-1] = (word_t){
                                .word = CaseName,
                                .word_type = StringHandleChar == '\'' ? WORD_SINGLE_STRING : WORD_DOUBLE_STRING,
                            };

                            StringHandleChar = 0;

                            CaseNameLen = 0;
                            //free(CaseName);
                            CaseName = NULL;

                            break;
                        }
                    }
                case CHAR_NORMAL:
                case CHAR_NUMBER:
                case CHAR_CONNECTABLE:
                case CHAR_UNCONNECTABLE:
                case CHAR_SPACE:
                    {

                        if (!CaseNameLen)
                        {
                            CaseStartColumn = ProcessingColumn;
                            CaseStartLine = ProcessingLine;
                        }

                        CaseNameLen++;
                        CaseName = realloc(
                            CaseName,
                            CaseNameLen*sizeof(char)
                            );
                        CaseName[CaseNameLen-1] = ThisChar;

                        break;
                    }


                case CHAR_BS:
                    {
                        HandleType = 2;
                        SuperCharHandlerIndex = CharIndex;

                        break;
                    }

                default:break;
                }



                break;
            }
        case 2: //Super Character
            {
                if (SuperCharCollect == 0)
                {
                    switch (ThisCharType)
                    {
                    case CHAR_NORMAL:
                        {
                            SuperCharParentNameLen++;
                            SuperCharParentName = realloc(
                                SuperCharParentName,
                                SuperCharParentNameLen*sizeof(char)
                                );
                            SuperCharParentName[SuperCharParentNameLen-1] = ThisChar;

                            break;
                        }
                    case CHAR_UNCONNECTABLE:
                        {
                            if (ThisChar == '(')
                            {
                                SuperCharCollect = 1;
                                break;
                            }
                        }

                    default:
                        {
                            SuperCharCollect = 2;
                            break;
                        }
                    }

                    toCMPChar = realloc(
                        toCMPChar,
                        (SuperCharParentNameLen+1)*sizeof(char)
                        );
                    toCMPChar = memcpy(
                        toCMPChar,
                        SuperCharParentName,
                        SuperCharParentNameLen*sizeof(char)
                        );
                    toCMPChar[SuperCharParentNameLen] = 0;

                    if (strcmp(toCMPChar,"n") == 0)
                    {
                        SuperCharParentType = 1;
                        SuperCharCollect = 2;
                    }else if (strcmp(toCMPChar,"a") == 0)
                    {
                        SuperCharParentType = 2;
                        SuperCharCollect = 1;

                        break;
                    }

                    free(toCMPChar);
                }

                if (SuperCharCollect == 1){
                    switch (ThisCharType)
                    {
                    case CHAR_NORMAL:
                    case CHAR_NUMBER:
                        {
                            SuperCharChildNameLen++;
                            SuperCharChildName = realloc(
                                SuperCharChildName,
                                SuperCharChildNameLen*sizeof(char)
                                );
                            SuperCharChildName[SuperCharChildNameLen-1] = ThisChar;

                            break;
                        }

                    case CHAR_UNCONNECTABLE:
                        {
                            if (ThisChar == ')')
                                SuperCharCollect = 2;

                            break;
                        }

                    default:
                        {
                            exit(400);
                        };
                    }
                }

                if (SuperCharCollect == 2)
                {
                    if (SuperCharParentNameLen)
                    {
                        switch (SuperCharParentType)
                        {
                        case 1:
                            {
                                if (!CaseNameLen)
                                {
                                    CaseStartColumn = ProcessingColumn;
                                    CaseStartLine = ProcessingLine;
                                }

                                CaseNameLen++;
                                CaseName = realloc(
                                    CaseName,
                                    CaseNameLen*sizeof(char)
                                    );
                                CaseName[CaseNameLen-1] = '\n';

                                break;
                            }
                        case 2:
                            {
                                if (!CaseNameLen)
                                {
                                    CaseStartColumn = ProcessingColumn;
                                    CaseStartLine = ProcessingLine;
                                }

                                CaseNameLen++;
                                CaseName = realloc(
                                    CaseName,
                                    CaseNameLen*sizeof(char)
                                    );
                                CaseName[CaseNameLen-1] = SuperCharChildName[0];

                                break;
                            }
                        default:
                            {
                                exit(300);

                            };
                        }
                    }

                    if (SuperCharChildNameLen)
                    {

                    }

                    HandleType = 1;

                    SuperCharHandlerIndex =
                       SuperCharParentName =
                           SuperCharParentNameLen =
                               SuperCharParentType =
                                   SuperCharChildName =
                                       SuperCharChildNameLen =
                                           SuperCharCollect = 0;

                }
                break;
            }
        case 3: //註解
            {
                if (
                    (ThisChar == '\n' && CommentChar == '/')
                    ||
                    (LastChar == '*' && ThisChar == '/' && CommentChar == '*')
                    )
                {
                    CommentChar = 0;
                    HandleType = 0;
                }

                break;
            }
        default:
            {
               exit(200);


            };
        }

        if (ThisCharType == CHAR_NEWLINE)
        {
            ProcessingLine++;
            ProcessingColumn=1;
            continue;
        }


        if (ThisChar == END_FLAG)
            break;

        ProcessingColumn++;

        CharIndex++;
        LastChar = ThisChar;
        LastCharType = ThisCharType;
    }while (1);


    free(SuperCharChildName);
    free(SuperCharParentName);

    if (CommentChar == '*')
    {
        exit(100);
    }

    end:
    
    return CaseCarriers;
}
//
// Created by calle on 24-12-28.
//

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "STDMIO.h"


int CheckCharType(const char Char)
{
    if (Char == EOF) return CT_NULL;

    if ((Char >= 'a' && Char <= 'z') || (Char >= 'A' && Char <= 'Z')) return CT_NORMAL;

    if (Char >= '0' && Char <= '9') return CT_NUMBER;

    if (Char == '"') return CT_DQ;

    if (Char == '\'') return CT_SQ;

    if (Char == '#') return CT_SHARP;

    static char CanConnectWithAnotherSymbol[] = {
        '*',
        '/',
        '+',
        '=',
        '^'
    };

    static char CanNotConnectWithAnotherSymbol[] = {
        '(',
        ')',
        '[',
        ']',
        '{',
        '}',
        '-',
        '.',
        ',',
        ':',
        '@',
        '<',
        '>',
    };

    for (int i = 0; i < sizeof(CanConnectWithAnotherSymbol)/sizeof(CanConnectWithAnotherSymbol[0]); i++)
        if (CanConnectWithAnotherSymbol[i] == Char)  return CT_CONNECTABLE;

    for (int i = 0; i < sizeof(CanNotConnectWithAnotherSymbol)/sizeof(CanNotConnectWithAnotherSymbol[0]); i++)
        if (CanNotConnectWithAnotherSymbol[i] == Char) return CT_UNCONNECTABLE;


    if (Char == ' ') return CT_SPACE;

    if (Char == '\\') return CT_BS;

    if (Char == '\n') return CT_NEWLINE;

    if (Char == ';') return CT_SEMICOLON;

    return 1;
}

FCOFunctionRespondObj FCO(FCOFunctionRequestObj input)
{
    FILE *F = input.f;

    FCOFunctionRespondObj Result = {0};
    Result.Event = input.EventTemplate;
    
    CaseObjCarrier CaseCarriers = {0};

    char * CaseName = NULL;
    unsigned int CaseNameLen = 0;

    unsigned int CharIndex = 0; //字元的偏移位

    char ThisChar = 0; //目前字元
    char LastChar = 0; //上個字元

    uint8_t ThisCharType = 0; //目前字元類型 包括被覆蓋的類型
    uint8_t LastCharType = 0; //上個字元類型 包括被覆蓋的類型

    uint8_t HandleType = 0; //處理類型

    uint8_t StringHandleChar = 0; //字串類型 `'` 與 `"` 的編碼

    unsigned int CaseStartLine = 0; //Case開始行號
    unsigned int CaseStartColumn = 0; //Case開始列號

    unsigned int ProcessingLine = 1;
    unsigned int ProcessingColumn = 1;


    // Super Character 處理
    unsigned int SuperCharHandlerIndex = 0; //Super Character 控制字元(即 `\` )的位置

    char * SuperCharParentName = NULL;
    unsigned int SuperCharParentNameLen = 0;

    uint8_t SuperCharParentType = 0; //Super Character Parent 的類型

    char * SuperCharChildName = NULL;
    unsigned int SuperCharChildNameLen = 0;

    uint8_t SuperCharCollect = 0; //Super Character 的收集狀態。1 : 已收集完Parent ; 2 : 已收集完Child

    char * toCMPChar = 0; //Super Character Parent名稱判斷

    // !

    // Number 特殊處理
    uint8_t RecordingNumber = 0; //正在紀錄數字 在有表達數字時 0表一般數字; 2表二進制; 3表十六進制;
    //

    do
    {
        ThisChar = fgetwc(F);
        ThisCharType = CheckCharType(ThisChar);

        switch (HandleType) //串一定要有休止符 才能使用Handle處裡 如：`"Hello World"`的尾端 `"`
        {
        case 0: //一般
            {
                //自元分節
                if (CharIndex)
                    if (!(
                        (LastCharType == CT_NORMAL && ThisCharType == CT_NUMBER)
                        ||
                        (LastCharType == CT_SHARP && ThisCharType == CT_NORMAL)
                        ||
                        (LastCharType == CT_SHARP && ThisCharType == CT_NUMBER)
                        ||
                        (LastCharType == CT_NUMBER && LastChar == '0' && CaseNameLen == 1 && ThisCharType == CT_NORMAL && (ThisChar == 'x' || ThisChar == 'b'))
                        ||
                        (RecordingNumber == 2 && ThisCharType == CT_NORMAL)
                    ))
                        if (CaseNameLen && LastCharType != ThisCharType){
                            unsigned int CaseType = CASE_NORMAL;

                            switch (LastCharType)
                            {
                            case CT_NUMBER:
                                {
                                    if (RecordingNumber == 0)
                                        CaseType = CASE_DECNUMBER;
                                    else if (RecordingNumber == 1)
                                        CaseType = CASE_BINNUMBER;
                                    else if (RecordingNumber == 2)
                                        CaseType = CASE_HEXNUMBER;

                                    break;
                                }
                            case CT_CONNECTABLE:
                                {
                                    CaseType = CASE_CONNECTABLE;

                                    break;
                                }
                            case CT_SEMICOLON:
                                {
                                    CaseType = CASE_BREAKER;


                                    break;
                                }

                            case CT_SHARP:
                                {
                                    CaseType = CASE_PHRASE;

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

                            CaseCarriers.CarrierLen++;
                            CaseCarriers.Carrier = realloc(
                                CaseCarriers.Carrier,
                                CaseCarriers.CarrierLen*sizeof(CaseObj)
                            );
                            CaseCarriers.Carrier[CaseCarriers.CarrierLen-1] = (CaseObj){
                                .ObjType = CaseType,
                                .ObjName = CaseName,

                                .CasePosition = (CasePositionObj){
                                    .CaseStartLine = CaseStartLine,
                                    .CaseEndLine = ProcessingLine,

                                    .CaseStartColumn = CaseStartColumn,
                                    .CaseEndColumn = ProcessingColumn,
                                }


                            };


                            RecordingNumber = 0; //重置數字紀錄


                            CaseName = NULL;
                            CaseNameLen = 0;
                        }

                //單字元處裡
                switch (ThisCharType)
                {
                case CT_NULL: break;
                case CT_NORMAL:
                    {
                        if (!CaseNameLen)
                        {
                            CaseStartColumn = ProcessingColumn;
                            CaseStartLine = ProcessingLine;
                        }

                        if (LastCharType == CT_SHARP)
                            ThisCharType = CT_SHARP;


                        if (
                            LastCharType == CT_NUMBER && LastChar == '0' && CaseNameLen == 1
                            &&
                            (ThisChar == 'x' || ThisChar == 'b')
                            )
                        {
                            RecordingNumber = ThisChar == 'x' ? 2 : 1;
                            ThisCharType = CT_NUMBER;

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
                                ThisCharType = CT_NUMBER;

                        break;
                    }
                case CT_NUMBER:
                    {
                        if (!CaseNameLen)
                        {
                            CaseStartColumn = ProcessingColumn;
                            CaseStartLine = ProcessingLine;
                        }


                        if (LastCharType == CT_NORMAL)
                            ThisCharType = CT_NORMAL;

                        if (LastCharType == CT_SHARP)
                            ThisCharType = CT_SHARP;

                        if (RecordingNumber == 1)
                            if (!(ThisChar == '0' || ThisChar == '1'))
                            {
                                CaseNameLen++;
                                CaseName = realloc(
                                    CaseName,
                                    CaseNameLen*sizeof(char)
                                    );
                                CaseName[CaseNameLen-1] = 0;

                                CaseCarriers.CarrierLen++;
                                CaseCarriers.Carrier = realloc(
                                    CaseCarriers.Carrier,
                                    CaseCarriers.CarrierLen*sizeof(CaseObj)
                                );
                                CaseCarriers.Carrier[CaseCarriers.CarrierLen-1] = (CaseObj){
                                    .ObjType = CASE_DECNUMBER,
                                    .ObjName = CaseName,

                                    .CasePosition = (CasePositionObj){
                                        .CaseStartLine = CaseStartLine,
                                        .CaseEndLine = ProcessingLine,

                                        .CaseStartColumn = CaseStartColumn,
                                        .CaseEndColumn = ProcessingColumn,
                                    }


                                };


                                RecordingNumber = 0; //重置數字紀錄

                                CaseName = NULL;
                                CaseNameLen = 0;
                            }


                        CaseNameLen++;
                        CaseName = realloc(
                            CaseName,
                            CaseNameLen*sizeof(char)
                            );
                        CaseName[CaseNameLen-1] = ThisChar;

                        break;
                    }

                case CT_SHARP:
                case CT_CONNECTABLE:
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

                case CT_UNCONNECTABLE:
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

                        CaseCarriers.CarrierLen++;
                        CaseCarriers.Carrier = realloc(
                            CaseCarriers.Carrier,
                            CaseCarriers.CarrierLen*sizeof(CaseObj)
                        );
                        CaseCarriers.Carrier[CaseCarriers.CarrierLen-1] = (CaseObj){
                            .ObjType = CASE_UNCONNECTABLE,
                            .ObjName = CaseName,

                            .CasePosition = (CasePositionObj){
                                .CaseStartLine = CaseStartLine,
                                .CaseEndLine = CaseStartLine+1,

                                .CaseStartColumn = CaseStartColumn,
                                .CaseEndColumn = CaseStartColumn+1,
                            }


                        };



                        CaseNameLen = 0;
                        CaseName = NULL;

                        break;
                    }

                case CT_SEMICOLON:
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
                case CT_DQ:
                case CT_SQ:
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
                case CT_DQ:
                case CT_SQ:
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

                            CaseCarriers.CarrierLen++;
                            CaseCarriers.Carrier = realloc(
                                CaseCarriers.Carrier,
                                CaseCarriers.CarrierLen*sizeof(CaseObj)
                            );
                            CaseCarriers.Carrier[CaseCarriers.CarrierLen-1] = (CaseObj){
                                .ObjType = StringHandleChar == '\'' ? CASE_SINGLE_STRING : CASE_DOUBLE_STRING,
                                .ObjName = CaseName,

                                .CasePosition = (CasePositionObj){
                                    .CaseStartLine = CaseStartLine,
                                    .CaseEndLine = ProcessingLine,

                                    .CaseStartColumn = CaseStartColumn,
                                    .CaseEndColumn = ProcessingColumn,
                                }


                            };


                            StringHandleChar = 0;

                            CaseName = NULL;
                            CaseNameLen = 0;

                            break;
                        }
                    }
                case CT_NORMAL:
                case CT_NUMBER:
                case CT_SHARP:
                case CT_CONNECTABLE:
                case CT_UNCONNECTABLE:
                case CT_SPACE:
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


                case CT_BS:
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
                    case CT_NORMAL:
                        {
                            SuperCharParentNameLen++;
                            SuperCharParentName = realloc(
                                SuperCharParentName,
                                SuperCharParentNameLen*sizeof(char)
                                );
                            SuperCharParentName[SuperCharParentNameLen-1] = ThisChar;

                            break;
                        }
                    case CT_UNCONNECTABLE:
                        {
                            if (ThisChar == '<')
                            {
                                SuperCharCollect = 1;
                                break;
                            }
                        }

                    default:
                        {
                            //不收集Child了 :<
                            SuperCharCollect = 2;
                            break;
                        }
                    }

                    toCMPChar = malloc(
                        (SuperCharParentNameLen+1)*sizeof(char)
                        );
                    toCMPChar = memcpy(
                        toCMPChar,
                        SuperCharParentName,
                        SuperCharParentNameLen*sizeof(char)
                        );
                    toCMPChar[SuperCharParentNameLen] = 0;

                    if (strcmp(toCMPChar,L"n") == 0)
                    {
                        SuperCharParentType = 1;
                        SuperCharCollect = 2;
                    }else if (strcmp(toCMPChar,L"a") == 0)
                    {
                        SuperCharParentType = 2;
                        SuperCharCollect = 1;

                        break;
                    }
                }

                if (SuperCharCollect == 1){
                    switch (ThisCharType)
                    {
                    case CT_NORMAL:
                    case CT_NUMBER:
                        {
                            SuperCharChildNameLen++;
                            SuperCharChildName = realloc(
                                SuperCharChildName,
                                SuperCharChildNameLen*sizeof(char)
                                );
                            SuperCharChildName[SuperCharChildNameLen-1] = ThisChar;

                            break;
                        }

                    case CT_UNCONNECTABLE:
                        {
                            if (ThisChar == '>')
                                SuperCharCollect = 2;

                            break;
                        }

                    default:
                        {
                            Result.Event.Code = 1;
                            Result.Event.Message = "Unknown Char.";
                            Result.Event.EventPosition = (CasePositionObj){
                                    .CaseEndColumn = ProcessingColumn,
                                    .CaseStartColumn = CaseStartColumn,
                                    .CaseEndLine = ProcessingLine,
                                    .CaseStartLine = CaseStartLine,
                                };

                            Result.CaseCarrier = CaseCarriers;

                            return Result;
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
                                Result.Event.Code = 2;
                                Result.Event.Message = "Unknown Super char parent type.";
                                Result.Event.EventPosition = (CasePositionObj){
                                    .CaseEndColumn = ProcessingColumn,
                                    .CaseStartColumn = CaseStartColumn,
                                    .CaseEndLine = ProcessingLine,
                                    .CaseStartLine = CaseStartLine,
                                };

                                Result.CaseCarrier = CaseCarriers;

                                return Result;
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
                                           SuperCharCollect =
                                               toCMPChar = 0;

                }
                break;
            }
        default:
            {
                Result.Event.Code = 2;
                Result.Event.Message = "Unknown HandleType..";
                Result.Event.EventPosition = (CasePositionObj){
                    .CaseEndColumn = ProcessingColumn,
                    .CaseStartColumn = CaseStartColumn,
                    .CaseEndLine = ProcessingLine,
                    .CaseStartLine = CaseStartLine,
                };

                Result.CaseCarrier = CaseCarriers;

                return Result;
            };
        }


        if (ThisCharType == CT_NEWLINE)
        {
            ProcessingLine++;
            ProcessingColumn=1;
            continue;
        }

        if (ThisChar == EOF)
            break;

        ProcessingColumn++;

        CharIndex++;
        LastChar = ThisChar;
        LastCharType = ThisCharType;
    }while (1);

    Result.CaseCarrier = CaseCarriers;

    return Result;
}
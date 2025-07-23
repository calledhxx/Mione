//
// Created by calle on 24-12-28.
//

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "ERR.h"
#include "OBJECTS.h"

int CheckCharType(const wchar_t Char)
{
    if (Char == WEOF) return CT_NULL;

    if ((Char >= L'a' && Char <= L'z') || (Char >= L'A' && Char <= L'Z')) return CT_NORMAL;

    if (Char >= L'0' && Char <= L'9') return CT_NUMBER;

    if (Char == L'"') return CT_DQ;

    if (Char == L'\'') return CT_SQ;

    if (Char == L'#') return CT_SHARP;

    static wchar_t CanConnectWithAnotherSymbol[] = {
        L'*',
        L'/',
        L'+',
        L'=',
        L'^'
    };

    static wchar_t CanNotConnectWithAnotherSymbol[] = {
        L'(',
        L')',
        L'[',
        L']',
        L'{',
        L'}',
        L'-',
        L'.',
        L',',
        L':',
        L'@',
        L'<',
        L'>',
    };

    for (int i = 0; i < sizeof(CanConnectWithAnotherSymbol)/sizeof(CanConnectWithAnotherSymbol[0]); i++)
        if (CanConnectWithAnotherSymbol[i] == Char)  return CT_CONNECTABLE;

    for (int i = 0; i < sizeof(CanNotConnectWithAnotherSymbol)/sizeof(CanNotConnectWithAnotherSymbol[0]); i++)
        if (CanNotConnectWithAnotherSymbol[i] == Char) return CT_UNCONNECTABLE;


    if (Char == L' ') return CT_SPACE;

    if (Char == L'\\') return CT_BS;

    if (Char == L'\n') return CT_NEWLINE;

    if (Char == L';') return CT_SEMICOLON;

    return 1;
}

CaseObjCarrier FCO(FILE* F,const uint8_t LineBreak)
{
    printf(">>FCO Function is called\n");

    CaseObjCarrier CaseCarriers = {0};

    wchar_t * CaseName = NULL;
    unsigned int CaseNameLen = 0;

    unsigned int CharIndex = 0; //字元的偏移位

    wchar_t ThisChar = 0; //目前字元
    wchar_t LastChar = 0; //上個字元

    uint8_t ThisCharType = 0; //目前字元類型 包括被覆蓋的類型
    uint8_t LastCharType = 0; //上個字元類型 包括被覆蓋的類型

    uint8_t HandleType = 0; //處理類型

    uint8_t StringHandleChar = 0; //字串類型 `'`與 `"`的編碼

    unsigned int CaseStartLine = 0; //Case開始行號
    unsigned int CaseStartColumn = 0; //Case開始列號

    unsigned int ProcessingLine = 1;
    unsigned int ProcessingColumn = 1;
    
    // Super Character 處理
    unsigned int SuperCharHandlerIndex = 0; //Super Character 控制字元(即 `\` )的位置

    wchar_t * SuperCharParentName = NULL;
    unsigned int SuperCharParentNameLen = 0;

    uint8_t SuperCharParentType = 0; //Super Character Parent 的類型

    wchar_t * SuperCharChildName = NULL;
    unsigned int SuperCharChildNameLen = 0;

    uint8_t SuperCharCollect = 0; //Super Character 的收集狀態。1 : 已收集完Parent ; 2 : 已收集完Child

    wchar_t * toCMPChar = 0; //Super Character Parent名稱判斷

    // !

    // Number 特殊處理
    uint8_t RecordingNumber = 0; //正在紀錄數字 0表無或者一般數字; 1表二進制; 2表十六進制
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
                            CaseNameLen++;
                            CaseName = realloc(
                                CaseName,
                                CaseNameLen*sizeof(wchar_t)
                                );
                            CaseName[CaseNameLen-1] = 0;

                            CaseCarriers.CarrierLen++;
                            CaseCarriers.Carrier = realloc(
                                CaseCarriers.Carrier,
                                CaseCarriers.CarrierLen*sizeof(CaseObj)
                            );
                            CaseCarriers.Carrier[CaseCarriers.CarrierLen-1] = (CaseObj){
                                .ObjType = LastCharType,
                                .ObjName = CaseName,

                                .CaseStartLine = CaseStartLine,
                                .CaseEndLine = ProcessingLine,

                                .CaseStartColumn = CaseStartColumn,
                                .CaseEndColumn = ProcessingColumn,
                            };

                            wprintf(L"added Name : `%ls` LC:(%d/%d)(%d/%d) Type %d\n", CaseName,CaseStartLine,CaseStartColumn,ProcessingLine,ProcessingColumn,LastCharType);

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

                        if (LastCharType == CT_NUMBER && LastChar == '0' && CaseNameLen == 1)
                            if (ThisChar == 'x' || ThisChar == 'b')
                            {
                                RecordingNumber = ThisChar == 'x' ? 2 : 1;
                                ThisCharType = CT_NUMBER;
                            }


                        if (RecordingNumber == 2)
                            ThisCharType = CT_NUMBER;

                        CaseNameLen++;
                        CaseName = realloc(
                            CaseName,
                            CaseNameLen*sizeof(wchar_t)
                            );
                        CaseName[CaseNameLen-1] = ThisChar;

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

                        CaseNameLen++;
                        CaseName = realloc(
                            CaseName,
                            CaseNameLen*sizeof(wchar_t)
                            );
                        CaseName[CaseNameLen-1] = ThisChar;

                        break;
                    }

                case CT_SHARP:
                    {
                        if (!CaseNameLen)
                        {
                            CaseStartColumn = ProcessingColumn;
                            CaseStartLine = ProcessingLine;
                        }

                        CaseNameLen++;
                        CaseName = realloc(
                            CaseName,
                            CaseNameLen*sizeof(wchar_t)
                            );
                        CaseName[CaseNameLen-1] = ThisChar;

                        break;
                    }
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
                            CaseNameLen*sizeof(wchar_t)
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
                            CaseNameLen*sizeof(wchar_t)
                            );

                        CaseName[0] = ThisChar;
                        CaseName[1] = 0;

                        CaseCarriers.CarrierLen++;
                        CaseCarriers.Carrier = realloc(
                            CaseCarriers.Carrier,
                            CaseCarriers.CarrierLen*sizeof(CaseObj)
                        );
                        CaseCarriers.Carrier[CaseCarriers.CarrierLen-1] = (CaseObj){
                            .ObjType = ThisCharType,
                            .ObjName = CaseName,

                            .CaseStartLine = CaseStartLine,
                            .CaseEndLine = CaseStartLine+1,

                            .CaseStartColumn = CaseStartColumn,
                            .CaseEndColumn = CaseStartColumn+1,
                        };

                        wprintf(L"(BY CCS) added Name : `%ls` LC:(%d/%d)(%d/%d) Type %d\n", CaseName,CaseStartLine,CaseStartColumn,ProcessingLine+1,ProcessingColumn+1,LastCharType);

                        CaseNameLen = 0;
                        CaseName = NULL;

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
                            StringHandleChar = 0;

                            CaseNameLen++;
                            CaseName = realloc(
                                CaseName,
                                CaseNameLen*sizeof(wchar_t)
                                );
                            CaseName[CaseNameLen-1] = 0;

                            CaseCarriers.CarrierLen++;
                            CaseCarriers.Carrier = realloc(
                                CaseCarriers.Carrier,
                                CaseCarriers.CarrierLen*sizeof(CaseObj)
                            );
                            CaseCarriers.Carrier[CaseCarriers.CarrierLen-1] = (CaseObj){
                                .ObjType = LastCharType,
                                .ObjName = CaseName,

                                .CaseStartLine = CaseStartLine,
                                .CaseEndLine = ProcessingLine,

                                .CaseStartColumn = CaseStartColumn,
                                .CaseEndColumn = ProcessingColumn,
                            };

                            wprintf(L"(BY STR) added Name : `%ls` LC:(%d/%d)(%d/%d) Type %d\n", CaseName,CaseStartLine,CaseStartColumn,ProcessingLine,ProcessingColumn,LastCharType);

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
                            CaseNameLen*sizeof(wchar_t)
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
                                SuperCharParentNameLen*sizeof(wchar_t)
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
                        (SuperCharParentNameLen+1)*sizeof(wchar_t)
                        );
                    toCMPChar = memcpy(
                        toCMPChar,
                        SuperCharParentName,
                        SuperCharParentNameLen*sizeof(wchar_t)
                        );
                    toCMPChar[SuperCharParentNameLen] = 0;

                    if (wcscmp(toCMPChar,L"n") == 0)
                    {
                        SuperCharParentType = 1;
                        SuperCharCollect = 2;
                    }else if (wcscmp(toCMPChar,L"a") == 0)
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
                                SuperCharChildNameLen*sizeof(wchar_t)
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

                    default:exit(123);
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
                                    CaseNameLen*sizeof(wchar_t)
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
                                    CaseNameLen*sizeof(wchar_t)
                                    );
                                CaseName[CaseNameLen-1] = SuperCharChildName[0];

                                break;
                            }
                        default: exit(2);
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
        default: exit(2);
        }


        if (ThisCharType == CT_NEWLINE)
        {
            ProcessingLine++;
            ProcessingColumn=1;
            continue;
        }

        if (ThisChar == WEOF)
            break;

        ProcessingColumn++;

        CharIndex++;
        LastChar = ThisChar;
        LastCharType = ThisCharType;
    }while (1);

    printf("<<FCO Function is ending\n");

    return CaseCarriers;
}
//
// Created by calle on 24-12-28.
//

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "ERR.h"
#include "OBJECTS.h"

extern inline  int CheckCharType(const wchar_t Char)
{
    if (Char == WEOF) return 0;

    if ((Char >= L'a' && Char <= L'z') || (Char >= L'A' && Char <= L'Z')) return 1;

    if (Char >= L'0' && Char <= L'9') return 2;

    if (Char == L'"') return 3;

    if (Char == L'\'') return 4;

    if (Char == L'#') return 5;

    static wchar_t CanConnectWithAnotherSymbol[] = { //why am i using static here? i dont know
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
        L';',
        L':',
        L'@',
        L'<',
        L'>',
    };

    for (int i = 0; i < sizeof(CanConnectWithAnotherSymbol)/sizeof(CanConnectWithAnotherSymbol[0]); i++)
        if (CanConnectWithAnotherSymbol[i] == Char)  return 9;

    for (int i = 0; i < sizeof(CanNotConnectWithAnotherSymbol)/sizeof(CanNotConnectWithAnotherSymbol[0]); i++)
        if (CanNotConnectWithAnotherSymbol[i] == Char) return 10;


    if (Char == L' ') return 11;

    if (Char == L'\\') return 12;

    if (Char == L'\n') return 13;

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

    unsigned int CaseStartLine = 1; //Case開始行號
    unsigned int CaseStartColumn = 0; //Case開始列號

    unsigned int ProcessingLine = 1;
    unsigned int ProcessingColumn = 0;
    
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

    do
    {
        ThisChar = fgetwc(F);
        ThisCharType = CheckCharType(ThisChar);

        printf("aaa %d\n",ThisChar);

        switch (HandleType)
        {
        case 0: //一般
            {
                //自元分節
                if (CharIndex)
                    if (!(
                        (LastCharType == 1 && ThisCharType == 2)
                        ||
                        (LastCharType == 5 && ThisCharType == 1)
                        ||
                        (LastCharType == 5 && ThisCharType == 2)
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

                            CaseName = NULL;
                            CaseNameLen = 0;
                        }

                //單字元處裡
                switch (ThisCharType)
                {
                case 0: break;
                case 1:
                    {
                        if (!CaseNameLen)
                        {
                            CaseStartColumn = ProcessingColumn;
                            CaseStartLine = ProcessingLine;
                        }

                        if (LastCharType == 5)
                            ThisCharType = 5;

                        CaseNameLen++;
                        CaseName = realloc(
                            CaseName,
                            CaseNameLen*sizeof(wchar_t)
                            );
                        CaseName[CaseNameLen-1] = ThisChar;

                        break;
                    }
                case 2:
                    {
                        if (!CaseNameLen)
                        {
                            CaseStartColumn = ProcessingColumn;
                            CaseStartLine = ProcessingLine;
                        }

                        if (LastCharType == 1)
                            ThisCharType = 1;

                        if (LastCharType == 5)
                            ThisCharType = 5;

                        CaseNameLen++;
                        CaseName = realloc(
                            CaseName,
                            CaseNameLen*sizeof(wchar_t)
                            );
                        CaseName[CaseNameLen-1] = ThisChar;

                        break;
                    }

                case 5:
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
                case 9:
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

                case 10:
                    {
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
                            .ObjType = LastCharType,
                            .ObjName = CaseName,

                            .CaseStartLine = CaseStartLine,
                            .CaseEndLine = ProcessingLine,

                            .CaseStartColumn = CaseStartColumn,
                            .CaseEndColumn = ProcessingColumn,
                        };

                        wprintf(L"(BY CCS) added Name : `%ls` LC:(%d/%d)(%d/%d) Type %d\n", CaseName,CaseStartLine,CaseStartColumn,ProcessingLine,ProcessingColumn,LastCharType);

                        CaseNameLen = 0;
                        CaseName = NULL;

                        break;
                    }

                //字串Handler
                case 3:
                case 4:
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
                case 3:
                case 4:
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
                case 1:
                case 2:
                case 5:
                case 9:
                case 10:
                case 11:
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


                case 12:
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
                    case 1:
                        {
                            SuperCharParentNameLen++;
                            SuperCharParentName = realloc(
                                SuperCharParentName,
                                SuperCharParentNameLen*sizeof(wchar_t)
                                );
                            SuperCharParentName[SuperCharParentNameLen-1] = ThisChar;

                            break;
                        }
                    case 10:
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
                    case 1:
                    case 2:
                        {
                            SuperCharChildNameLen++;
                            SuperCharChildName = realloc(
                                SuperCharChildName,
                                SuperCharChildNameLen*sizeof(wchar_t)
                                );
                            SuperCharChildName[SuperCharChildNameLen-1] = ThisChar;

                            break;
                        }

                    case 10:
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


        if (ThisCharType == 13)
        {
            ProcessingLine++;
            ProcessingColumn=0;
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


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
    };

    for (int i = 0; i < sizeof(CanConnectWithAnotherSymbol)/sizeof(CanConnectWithAnotherSymbol[0]); i++)
        if (CanConnectWithAnotherSymbol[i] == Char)  return 9;

    for (int i = 0; i < sizeof(CanNotConnectWithAnotherSymbol)/sizeof(CanNotConnectWithAnotherSymbol[0]); i++)
        if (CanNotConnectWithAnotherSymbol[i] == Char) return 10;


    if (Char == L' ') return 11;

    if (Char == L'\\') return 12;

    if (Char == L'\n' || Char == 10) return 13;

    return 1;
}



CaseObjCarrier FCO(FILE* F,const uint8_t LineBreak)
{
    CaseObjCarrier CaseCarriers = {0};

    wchar_t * CaseName = NULL;
    unsigned int CaseNameLen = 0;

    unsigned int CharIndex = 0; //字元的偏移位

    wchar_t ThisChar = 0; //目前字元
    wchar_t LastChar = 0; //上個字元

    uint8_t ThisCharType = 0; //目前字元類型
    uint8_t LastCharType = 0; //上個字元類型

    uint8_t HandleType = 0; //處理類型

    uint8_t StringHandleChar = 0; //字串類型 `'`與 `"`的編碼

    do
    {
        ThisChar = fgetwc(F);
        ThisCharType = CheckCharType(ThisChar);

        switch (HandleType)
        {
        case 0: //一般
            {
                //自元分節
                if (CharIndex) if (LastCharType != ThisCharType)
                {
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
                        .ObjType = ThisCharType,
                        .ObjName = CaseName
                    };

                    CaseName = NULL;
                    CaseNameLen = 0;
                }

                //單字元處裡
                switch (ThisCharType)
                {
                case 0: break;
                case 1:
                    {
                        break;
                    }
                case 2:
                    {
                        break;
                    }

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
                        }
                        break;
                    }
                default:break;
                }

                break;
            }
        default: exit(2);
        }


        CaseNameLen++;
        CaseName = realloc(
            CaseName,
            CaseNameLen*sizeof(wchar_t)
            );
        CaseName[CaseNameLen-1] = ThisChar;


        if (ThisChar == WEOF)
            break;

        CharIndex++;
        LastChar = ThisChar;
        LastCharType = ThisCharType;
    }while (1);

    return CaseCarriers;
}
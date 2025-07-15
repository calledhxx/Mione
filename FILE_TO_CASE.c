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

    if ((Char >= L'0' && Char <= L'9')) return 2;

    if ((Char == L'"')) return 3;

    if ((Char == L'\'')) return 4;

    //if (NULL) return 5;

    //if (NULL) return 6;

    //if (NULL) return 7;

    //if (NULL) return 8;

    //Symbols

    wchar_t CanConnectWithAnotherSymbol[] = {
        L'*',
        L'/',
        L'+',
        L'=',
        L'^'
    };

    wchar_t CanNotConnectWithAnotherSymbol[] = {
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
        L'@'
    };

    for (int i = 0; i < sizeof(CanConnectWithAnotherSymbol)/sizeof(CanConnectWithAnotherSymbol[0]); i++)
    {
        if (CanConnectWithAnotherSymbol[i] == Char)  return 9;
    }
    for (int i = 0; i < sizeof(CanNotConnectWithAnotherSymbol)/sizeof(CanNotConnectWithAnotherSymbol[0]); i++)
    {
        if (CanNotConnectWithAnotherSymbol[i] == Char) return 10;
    }

    //space
    if (Char == L' ') return 11;

    if (Char == L'\\') return 12;

    if (Char == L'\n' || Char == 10) return 13;


    return 1;
}



CaseObjCarrier FCO(FILE* F)
{
    CaseObj *CaseObjects = malloc(0);
    int CaseObjectsSize = 0;

    int inLockinType = 0; // 是否在限制別類裡，如果是則此項表示限制別類的類型。例如：字串=1

    wchar_t StringHostingBy = 0;//字串由誰發起 " 與 ' 的編碼

    int ThisInLockinLast = 0;//是否為inLockinType的最後一項，辨別使用

    int superCharSize = 0; //是否為特殊字符 '\' 的後項，表示 '\'後(包含)有多少字元進行運算

    int hasBracket = 0; //為 '\'符號的特殊項，表示這個 backslashOption 有 '(' (1) 以及 ')' (2)符號

    int superCharMode = 0; //為 '\'符號的特殊項, 表示這個特殊符號的功能是什麼 例 : 1 = 回傳換行值 (10) , 2 = 回傳自訂字元...

    int LastCharType = 0; //上個字元的別類
    wchar_t LastChar = 0;//上個字元

    wchar_t* backslashOption = malloc(0);

    wchar_t* superCharOpt = malloc(0); //每個模式自己個用
    int superCharOptSize = 0;

    wchar_t* superCharOut = malloc(0); //返回值
    int superCharOutSize = 0;

    wchar_t* CASE = malloc(0);
    int CASESize = 0;

    wchar_t c = 0;
    int cIndex = -1;

    int Line=1;
    int Colum = 0;

    do
    {
        c = fgetwc(F);
        cIndex++;

        int CharType = CheckCharType(c);



        if (c == L'\r')
            continue;


        Colum++;


        switch (inLockinType)
        {
        case 1:
            if (superCharSize)
            {
                superCharSize++;
                backslashOption = realloc(backslashOption, superCharSize * sizeof(wchar_t));
                backslashOption[superCharSize - 1] = c;
                //printf("*[SUPER CHAR ADD]* ");


                if (superCharSize > 2)
                {
                    if (c == L'(')
                    {
                        hasBracket = 1;
                    }
                    else if (c == L')')
                    {
                        if (hasBracket == 1)
                        {
                            hasBracket = 2;
                        }
                    }
                }


                if (superCharSize == 2)
                {
                    switch (c)
                    {
                    case L'n':
                        superCharMode = 1;
                        break;
                    case L'u':
                        superCharMode = 2;
                        break;
                    case L'\'':
                        superCharMode = 3;
                        break;
                    case L'"':
                        superCharMode = 4;
                        break;
                    }
                }


                switch (superCharMode)
                {
                case 1:
                    {
                        CASESize++;
                        CASE = realloc(CASE, CASESize * sizeof(wchar_t));
                        CASE[CASESize - 1] = 10;

                        superCharMode = 0;

                        backslashOption = 0;
                        superCharSize = 0;

                        continue;

                    }
                case 2:
                    {
                        if (hasBracket == 2) //end
                        {
                            //int unStart = cIndex -superCharSize + 1 /* '/' */ +  1 /* Opt符號 */  + 1 /* '(' */ + (1);
                            //int unEnd = cIndex-(1);

                            superCharOpt = realloc(superCharOpt, superCharOptSize * sizeof(wchar_t));
                            superCharOpt[superCharOptSize] = 0;

                            int isHex = 0;
                            for (int i = 1/* 不要 '(' */ ; i < superCharOptSize; i = i + 2)
                            {
                                if (isHex) if ((superCharOptSize - 1/* '(' */) % 2 == 1) break;
                                if (i == 1)
                                {
                                    if (superCharOpt[i] == L'1' && superCharOpt[i + 1] == L'6') isHex = 1;
                                }
                                else
                                {
                                    const wchar_t A[] = {
                                        L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F'
                                    };

                                    int indexA = 0; //個位
                                    int indexB = 0; //十位


                                    for (int index = 0; index < 16; index++)
                                    {
                                        if (A[index] == superCharOpt[i + 1])
                                        {
                                            indexA = index;
                                        };
                                        if (A[index] == superCharOpt[i])
                                        {
                                            indexB = index;
                                        };
                                    }

                                    superCharOutSize++;
                                    superCharOut = realloc(superCharOut, superCharOutSize * sizeof(wchar_t));
                                    superCharOut[superCharOutSize - 1] = indexB * 16 + indexA;
                                };
                            }


                            //for (int i =0;i<superCharOutSize;i++) {printf("A:%d %d\n",superCharOut[i],superCharOutSize);}


                            //for (int i =0;i<CASESize;i++) {printf("B:%d\n",CASE[i]);}

                            CASESize = CASESize + superCharOutSize;
                            CASE = realloc(CASE, CASESize * sizeof(wchar_t));


                            for (int i = 0; i < superCharOutSize; i++)
                            {
                                CASE[i + CASESize - superCharOutSize] = superCharOut[i];
                            }

                            superCharMode = 0;

                            backslashOption = 0;
                            superCharSize = 0;

                            continue;
                        }
                        else if (hasBracket == 1) //還在紀錄
                        {
                            superCharOptSize++;
                            superCharOpt = realloc(superCharOpt, superCharOptSize* sizeof(wchar_t));
                            superCharOpt[superCharOptSize - 1] = c;
                        }

                        break;
                    }
                case 3:
                    {
                        CASESize++;
                        CASE = realloc(CASE, CASESize* sizeof(wchar_t));
                        CASE[CASESize - 1] = L'\'';


                        superCharMode = 0;

                        backslashOption = 0;
                        superCharSize = 0;

                        continue;

                    }
                case 4:
                    {
                        CASESize++;
                        CASE = realloc(CASE, CASESize * sizeof(wchar_t));
                        CASE[CASESize - 1] = L'"';

                        superCharMode = 0;

                        backslashOption = 0;
                        superCharSize = 0;

                        continue;

                    }
                }
            }
            break;
        case 2:
            if (c == L'\n')
            {
                inLockinType = 0;
                ThisInLockinLast = 1;
            }
            break;
        case 3:
            if (LastCharType == CharType)  if (LastChar == L'*' || c == L'/')
            {
                inLockinType = 0;
                ThisInLockinLast = 1;
            }
            break;
        }


        if (!inLockinType)
        {
            if (LastCharType == CharType)
            {
                if (LastCharType == 9)
                {
                    if (LastChar == L'/')
                    {
                        if (c == L'/') inLockinType = 2;
                        if (c == L'*') inLockinType = 3;


                        CASESize--;
                        CASE[CASESize] = (char)0;
                        CASE = realloc(CASE, CASESize * sizeof(wchar_t));
                    }
                }
            }
            else //一般結束 沒有被限制的情況下
            {


                if (CASESize) //第一個會有0的差異
                {
                    if ((LastCharType == 1 && CharType == 2) )
                    {

                    }else
                    {

                        CASESize++;
                        CASE = realloc(CASE, CASESize * sizeof(wchar_t));
                        CASE[CASESize - 1] = 0;


                        CaseObjectsSize++;
                        CaseObjects = realloc(CaseObjects, CaseObjectsSize*sizeof(CaseObj));
                        CaseObjects[CaseObjectsSize - 1] = (CaseObj){
                            .ObjType = LastCharType,
                            .ObjName = CASE,
                        };

                        CASE = NULL;
                        CASE = malloc(0);
                        CASESize = 0;
                    }

                }
            }
        }


        if (!superCharSize) //一般
        {
            switch (CharType)
            {
            case 1:
                if (inLockinType)
                {
                }
                else
                {
                    //printf("yess  %d\n",inLockinType);
                    CASESize++;
                    CASE = realloc(CASE, CASESize * sizeof(wchar_t));
                    CASE[CASESize - 1] = c;
                }

                break;
            case 2:
                if (inLockinType){}else
                {
                    //printf("hahaha %d\n",inLockinType);
                    CASESize++;
                    CASE = realloc(CASE, CASESize * sizeof(wchar_t));
                    CASE[CASESize - 1] = c;
                    if (LastCharType == 1) CharType = 1;
                }
                break;
            case 3:
                if (inLockinType == 1 && StringHostingBy == (char)c)
                {
                    CASESize++;
                    CASE = realloc(CASE, CASESize * sizeof(wchar_t));
                    CASE[CASESize - 1] = c;

                    //printf("huh %d\n",c);

                    CASESize++;
                    CASE = realloc(CASE, CASESize * sizeof(wchar_t));
                    CASE[CASESize - 1] = 0;
                    //printf("my track~ '%s'\n", CASE);



                    CaseObjectsSize++;
                    CaseObjects = realloc(CaseObjects, CaseObjectsSize*sizeof(CaseObj));
                    CaseObjects[CaseObjectsSize - 1] = (CaseObj){
                        .ObjType = 3,
                        .ObjName = CASE,
                    };
                   // printf("*[CASE END]* \n");

                    inLockinType = 0;

                    CASE = NULL;
                    CASE = malloc(0);
                    CASESize = 0;
                }
                else if (inLockinType == 0)
                {
                    StringHostingBy = (int)c;
                    inLockinType = 1;
                }

                break;
            case 4:

        if (inLockinType == 1 && StringHostingBy == (char)c)
        {
            CASESize++;
            CASE = realloc(CASE, CASESize * sizeof(wchar_t));
            CASE[CASESize - 1] = c;

            //printf("huh %d\n",c);

            CASESize++;
            CASE = realloc(CASE, CASESize * sizeof(wchar_t));
            CASE[CASESize - 1] = 0;
            //printf("my track~ '%s'\n", CASE);



            CaseObjectsSize++;
            CaseObjects = realloc(CaseObjects, CaseObjectsSize*sizeof(CaseObj));
            CaseObjects[CaseObjectsSize - 1] = (CaseObj){
                .ObjType = 3,
                .ObjName = CASE,
            };
            // printf("*[CASE END]* \n");

            inLockinType = 0;

            CASE = NULL;
            CASE = malloc(0);
            CASESize = 0;
        }
        else if (inLockinType == 0)
        {
            StringHostingBy = (int)c;
            inLockinType = 1;
        }

                break;
            case 9:
                if (!inLockinType && !ThisInLockinLast)
                {
                    CASESize++;
                    CASE = realloc(CASE, CASESize * sizeof(wchar_t));
                    CASE[CASESize - 1] = c;


                }

                break;
            case 10:
                if (!inLockinType)
                {
                    if (LastCharType == 10)
                    {
                        CASESize++;
                        CASE = realloc(CASE, CASESize* sizeof(wchar_t));
                        CASE[CASESize - 1] = 0;

                        //printf("bdfr '%s'\n", CASE);


                        CaseObjectsSize++;
                        CaseObjects = realloc(CaseObjects, CaseObjectsSize*sizeof(CaseObj));
                        CaseObjects[CaseObjectsSize - 1] = (CaseObj){
                            .ObjType = 10,
                            .ObjName = CASE,
                        };
                        CASE = NULL;
                        CASE = malloc(0);
                        CASESize = 0;

                        CASESize++;
                        CASE = realloc(CASE, CASESize* sizeof(wchar_t));
                        CASE[CASESize - 1] = c;
                    }
                    else
                    {
                        CASESize++;
                        CASE = realloc(CASE, CASESize* sizeof(wchar_t));
                        CASE[CASESize - 1] = c;
                    }
                }
                break;
            case 12:
                if (inLockinType == 1)
                {
                    if (superCharSize) //防止上方的結構有加了
                    {
                    }
                    else
                    {
                        // printf("*[SUPER CHAR START]* ");
                        superCharSize++;
                        backslashOption = realloc(backslashOption, superCharSize * sizeof(wchar_t));
                        backslashOption[superCharSize - 1] = c;
                    }
                }else
                {
                    ErrCall("borken","M000",NULL,Line,Colum);
                }


                break;
            case 13:
                Line++;
                Colum = 0;

                if (inLockinType)
                {

                }else
                {
                    CaseObjectsSize++;
                    CaseObjects = realloc(CaseObjects, CaseObjectsSize*sizeof(CaseObj));
                    CaseObjects[CaseObjectsSize - 1] = (CaseObj){
                        .ObjType = 13,
                        .ObjName = L"\n",
                    };
                }
                break;
            }
        }

        if (!superCharSize) //Lockin 【內】專屬
        {
            switch (inLockinType)
            {
            case 1: //字串

                //printf("%d *[CASE ADDED]* \"%d\"",cIndex,c);
                CASESize++;
                CASE = realloc(CASE, CASESize * sizeof(wchar_t));
                CASE[CASESize - 1] = c;

                break;
            }
        }


        //printf("'%c' '%d' '%d'\n",c,CharType,ThislastSuperChar);
        LastCharType = CharType;
        LastChar = c;
        ThisInLockinLast = 0;
        ThisInLockinLast = 0;
        // ThislastSuperChar = 0;1

        if (c == WEOF) break;
    }
    while (1);



    if (inLockinType)
    {
        ErrCall("no end FILE TO CASE","M245",NULL,Line,Colum);
    }


    return (CaseObjCarrier){
        .Carrier = CaseObjects,
        .CarrierLen = CaseObjectsSize,
    };
}
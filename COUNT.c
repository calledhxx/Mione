//
// Created by calle on 24-11-3.
//

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ERR.h"
#include "Mione.h"
#include "PROMPT_DEF.h"
#include "SYMBOL_DEF.h"



CountObj COUNT(MioneObj*Pack,int PackSize)
{

    int FirstBracketIndex = 0;
    int BracketsChild = 0; //括號多寡

    int BracketCur = 0;//1 : [, 2 : (

    MioneObj* inBracket = malloc(0);
    int inBracketSize = 0;

    int CalculateType = 0;
    int CalculateLevel = 0;


    for(int i = 0; i < PackSize; i++)
    {
        int PastCost = 0;//符號所前扣之值

        switch (Pack[i].ObjType)
        {
        case SYMBOL:
            {
                switch (Pack[i].Symbol.CurNumber)
            {
            case 10:
                {
                    if (!BracketsChild)
                    {
                        BracketCur = 1;
                        FirstBracketIndex = i;

                        inBracketSize = 0;
                        free(inBracket);
                        inBracket = malloc(0);
                    }else
                    {
                        inBracketSize++;
                        inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                        inBracket[inBracketSize - 1] = Pack[i];
                    }
                    BracketsChild++;
                }
                break;
            case 11:
                {
                    BracketsChild--;
                    if (!BracketsChild && BracketCur == 1)
                    {
                        BracketCur = 0;
                    }else
                    {
                        inBracketSize++;
                        inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                        inBracket[inBracketSize - 1] = Pack[i];
                    }
                }
                break;
            case 12:
                {

                    if (!BracketsChild)
                    {
                        BracketCur = 2;
                        FirstBracketIndex = i;


                        inBracketSize = 0;
                        free(inBracket);
                        inBracket = malloc(0);
                    }else
                    {
                        inBracketSize++;
                        inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                        inBracket[inBracketSize - 1] = Pack[i];
                    }

                    BracketsChild++;
                }
                break;
                case 13:
                {
                    BracketsChild--;
                    if (!BracketsChild && BracketCur == 2)
                    {
                        int FunctionCalled = 0;
                        BracketCur = 0;

                        CountObj ChildCount = COUNT(inBracket, inBracketSize);

                        if (FirstBracketIndex > 0)
                        {
                             if (Pack[FirstBracketIndex - 1].ObjType == VARIABLE || Pack[FirstBracketIndex - 1].ObjType == VALUE)
                            {

                                if (Pack[FirstBracketIndex - 1].ObjType == VARIABLE)
                                {

                                    if (Pack[FirstBracketIndex - 1].VarUP->Val.ValueType == VALUE_FUNCTION_TYPE)
                                    {
                                        ValueReturnObj V = Function(
                                            Pack[FirstBracketIndex - 1].VarUP->Val.Area.Area,
                                            Pack[FirstBracketIndex - 1].VarUP->Val.Area.Size
                                            ).Vs;

                                        MioneObj* NewPack = malloc(0);
                                        int NewPackSize = 0;

                                        for (int index = 0; index < FirstBracketIndex-1; index++)
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }

                                        printf("sizoe of fuc returned : %d\n",V.ValueSize);

                                        if (V.ValueSize)
                                        {
                                            for (int index = 0; index < V.ValueSize; index++)
                                            {
                                                NewPackSize++;
                                                NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                                NewPack[NewPackSize - 1] = (MioneObj){
                                                    .ObjType = VALUE,
                                                    .Val = V.Value[index],
                                                    .Line = Pack[i-1].Line,
                                                    .Column = Pack[i-1].Column
                                                };
                                            }

                                        }else
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = (MioneObj){
                                                .ObjType = VALUE,
                                                .Val = (ValueObj){
                                                    .ValueType = VALUE_NOPOINTNUMBER_TYPE,.NPNumber = 0}
                                            };
                                        }



                                        for (int index = i+1 ; index < PackSize; index++)
                                        {

                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }



                                        PackSize = NewPackSize;
                                        Pack = NewPack;


                                        i = FirstBracketIndex - 1;

                                        FunctionCalled = 1;
                                    }
                                    else
                                    {

                                        char *str1 = "The Type of `";
                                        char *str2 = "` isn't a FUNCTION";
                                        char *VName = Pack[FirstBracketIndex - 1].VarUP->Name;

                                        char *REASON = malloc(sizeof(char) * (int)(strlen(str1) + strlen(str2) + strlen(VName) + 1));
                                        strcpy(REASON, str1);
                                        strcat(REASON, VName);
                                        strcat(REASON, str2);

                                        ErrCall(
                                            REASON,
                                                "MG001",
                                            "Maybe you can change the Variable Type to FUNCTION.",
                                            Pack[FirstBracketIndex - 1].Line,
                                            Pack[FirstBracketIndex - 1].Column

                                        );
                                    }
                                }
                                else
                                {
                                    if (Pack[FirstBracketIndex - 1].Val.ValueType == VALUE_FUNCTION_TYPE)
                                    {
                                        ValueReturnObj V = Function(
                                            Pack[FirstBracketIndex - 1].Val.Area.Area,
                                            Pack[FirstBracketIndex - 1].Val.Area.Size
                                            ).Vs;

                                        MioneObj* NewPack = malloc(0);
                                        int NewPackSize = 0;

                                        for (int index = 0; index < FirstBracketIndex-1; index++)
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }

                                        printf("sizoe of fuc returned : %d\n",V.ValueSize);

                                        if (V.ValueSize)
                                        {
                                            for (int index = 0; index < V.ValueSize; index++)
                                            {
                                                NewPackSize++;
                                                NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                                NewPack[NewPackSize - 1] = (MioneObj){
                                                    .ObjType = VALUE,
                                                    .Val = V.Value[index],
                                                    .Line = Pack[i-1].Line,
                                                    .Column = Pack[i-1].Column
                                                };

                                            }

                                        }else
                                        {
                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = (MioneObj){
                                                .ObjType = VALUE,
                                                .Val = (ValueObj){
                                                    .ValueType = VALUE_NOPOINTNUMBER_TYPE,.NPNumber = 0}
                                            };
                                        }


                                        for (int index = i+1 ; index < PackSize; index++)
                                        {

                                            NewPackSize++;
                                            NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                                            NewPack[NewPackSize - 1] = Pack[index];
                                        }

                                        PackSize = NewPackSize;
                                        Pack = NewPack;


                                        i = FirstBracketIndex - 1;





                                        FunctionCalled = 1;
                                    }
                                    else
                                    {
                                        ErrCall(
                                            "The Value before `()` Isn't a Function",
                                            "MG002",
                                            "Maybe you can try `function() <...:Mione> end()`.",
                                            Pack[FirstBracketIndex - 1].Line,
                                            Pack[FirstBracketIndex - 1].Column
                                        );
                                    }
                                }
                            }
                        }


                        if (!FunctionCalled)
                        {
                            MioneObj* NewPack = malloc(0);
                            int NewPackSize = 0;

                            for (int index = 0; index < FirstBracketIndex; index++)
                            {
                                NewPackSize++;
                                NewPack = realloc(NewPack, NewPackSize * sizeof(MioneObj));
                                NewPack[NewPackSize - 1] = Pack[index];
                            }


                            for (int index = 0; index < ChildCount.ValueSize; index++)
                            {
                                NewPackSize++;
                                NewPack = realloc(NewPack, NewPackSize * sizeof(MioneObj));
                                NewPack[NewPackSize - 1] = (MioneObj){
                                    .ObjType = VALUE,
                                    .Val = ChildCount.Value[index]
                                };
                            }

                            for (int index = i + 1; index < PackSize; index++)
                            {
                                NewPackSize++;
                                NewPack = realloc(NewPack, NewPackSize * sizeof(MioneObj));
                                NewPack[NewPackSize - 1] = Pack[index];
                            }

                            Pack = NewPack;
                            PackSize = NewPackSize;

                            i = FirstBracketIndex -1;
                        }
                    }else
                    {
                        inBracketSize++;
                        inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                        inBracket[inBracketSize - 1] = Pack[i];
                    }

                }
                break;
            default:
                    {
                        if(!BracketsChild){

                            if (CalculateLevel) switch (Pack[i].Symbol.SymbolType)
                            {
                                case 1:
                                    {

                                        if (i == PackSize - 1 || i - 1 <0)
                                        {
                                            ErrCall(
                                                "You can't connect any Two-side-count-SymbolGet.a to VOID (Meaning Nothing, even no Mione Object).",
                                                "MG003",
                                                "Maybe you can try `1+1` or anything else.",
                                                Pack[i].Line,
                                                Pack[i].Column
                                            );
                                        }



                                        if (Pack[i-1].ObjType == SYMBOL || Pack[i+1].ObjType == SYMBOL)
                                        {
                                            ErrCall(
                                              "dkaopkdapskdpsa",
                                              "MG3123i13",
                                              "Maybe you can try `1+1` or anything else.",
                                              Pack[i].Line,
                                              Pack[i].Column
                                          );
                                        }
                                        if (Pack[i].Symbol.xIndex == CalculateLevel) CalculateType = Pack[i].Symbol.CurNumber;

                                        break;
                                    }
                                 case 2:
                                     {
                                        if (i == PackSize - 1 )
                                        {
                                            ErrCall(
                                                "bbbbb",
                                                "MG007",
                                                "Maybe you can try `typeof 1` or anything else.",
                                                Pack[i].Line,
                                                Pack[i].Column
                                            );
                                        }

                                        if (Pack[i+1].ObjType == SYMBOL)
                                        {
                                            ErrCall(
                                                "dmakldmadma",
                                                "MG03127",
                                                "Maybe you can try `typeof 1` or anything else.",
                                                Pack[i].Line,
                                                Pack[i].Column
                                            );
                                        }

                                        if (i)
                                        {
                                            if (Pack[i-1].ObjType != SYMBOL)
                                            {
                                                ErrCall(
                                                    "[dlsapld[pasd[l",
                                                    "dasdsadasa",
                                                    NULL,
                                                    Pack[i].Line,
                                                    Pack[i].Column
                                                );
                                            }
                                        }

                                        if (Pack[i].Symbol.xIndex == CalculateLevel) CalculateType = Pack[i].Symbol.CurNumber;

                                        break;
                                     }
                                case 3:
                                    {
                                        if (i == PackSize - 1)
                                        {
                                            ErrCall(
                                               "ccccc",
                                               "MG008",
                                               "Maybe you can try `- 1` or anything else.",
                                                Pack[i].Line,
                                                Pack[i].Column
                                           );
                                        }

                                        if ( Pack[i+1].ObjType == SYMBOL)
                                        {
                                            ErrCall(
                                               "jdioasjdioas",
                                               "MG0428",
                                               "Maybe you can try `- 1` or anything else.",
                                                Pack[i].Line,
                                                Pack[i].Column
                                           );
                                        }


                                        for (int index = 0; 1 ; index++)
                                        {
                                            if (Symbols[index].CurNumber == -1) break;

                                            if (strcmp(Symbols[index].Name, Pack[i].Symbol.Name) == 0)
                                            {

                                                if (i-1>=0 && (Pack[i-1].ObjType == VARIABLE || Pack[i-1].ObjType == VALUE))
                                                {
                                                    if (Pack[i].Symbol.yIndex == CalculateLevel) CalculateType = Pack[i].Symbol.CurNumber;
                                                }else
                                                {
                                                    if (Pack[i].Symbol.xIndex == CalculateLevel) CalculateType = Pack[i].Symbol.CurNumber;
                                                }

                                            }

                                        }
                                        break;
                                    }
                            }
                        }else
                        {
                            inBracketSize++;
                            inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                            inBracket[inBracketSize - 1] = Pack[i];
                        }
                    }
            }
            }
            break;
            default:
            {
            if (!BracketsChild)
            {
                if (Pack[i].ObjType == VALUE && Pack[i].Val.ValueType == VALUE_TABLE_TYPE && !Pack[i].Val.Table.Counted)
                {
                    ValueObj V = Pack[i].Val;

                    VariableObj * CountedTable = malloc(0);
                    int CountedTableSize = 0;

                    // MioneReturnObj R =
                    Table(V.Table.MioneTable,V.Table.MioneTableSize,&CountedTable,&CountedTableSize);


                    Pack[i].Val.Table.Counted=1;
                    Pack[i].Val.Table.MioneTable = NULL;
                    Pack[i].Val.Table.MioneTableSize = 0;

                    VariableObj * NewTable = malloc(0);
                    int NewTableSize = 0;

                    for (int TableChildIndex = 0; TableChildIndex<CountedTableSize ; TableChildIndex++)
                    {
                        NewTableSize++;
                        NewTable = realloc (NewTable, NewTableSize*sizeof(VariableObj));
                        NewTable[NewTableSize-1] = CountedTable[TableChildIndex];

                        if (CountedTable[TableChildIndex].Place)
                        {
                            for (int CTCIndex = 0; CTCIndex<NewTableSize ; CTCIndex++)
                            {
                                if (NewTable[CTCIndex].Place > NewTable[NewTableSize-1].Place &&NewTable[CTCIndex].Place )
                                {
                                    VariableObj Butter = NewTable[CTCIndex];
                                    NewTable[CTCIndex] = NewTable[NewTableSize-1];
                                    NewTable[NewTableSize-1] = Butter;
                                }
                            }
                        }
                    }

                    printf("size %d\n",CountedTableSize);

                    CountedTable = NewTable;
                    CountedTableSize = NewTableSize;

                    for (int TableChildIndex = 0; TableChildIndex<CountedTableSize ; TableChildIndex++)
                    {
                        if (CountedTable[TableChildIndex].Place) printf("bbb %d\n", CountedTable[TableChildIndex].Place);
                        if (CountedTable[TableChildIndex].Name) printf("aaa %s\n", CountedTable[TableChildIndex].Name);
                    }


                    Pack[i].Val.Table.CountedTable = CountedTable;
                    Pack[i].Val.Table.CountedTableSize = CountedTableSize;
                }

                if (CalculateType)
            {
                MioneObj Out;
                int UsePointNumber = 0;

                switch (CalculateType)
                {
                    case 1: // +
                        {
                            if (Pack[i - 2].ObjType == VARIABLE || Pack[i - 2].ObjType == VALUE)
                        {

                            ValueObj Target1 ;
                            ValueObj Target2 ;


                            if (Pack[i - 2].ObjType == VARIABLE) Target1 = Pack[i - 2].VarUP->Val; else Target1 = Pack[i - 2].Val;
                            if (Pack[i].ObjType == VARIABLE) Target2 = Pack[i].VarUP->Val; else Target2 = Pack[i].Val;

                            if (!(Target1.ValueType == VALUE_NOPOINTNUMBER_TYPE || Target1.ValueType == VALUE_POINTNUMBER_TYPE)){ ErrCall("Type Error1","MG0011111","",
                                Pack[i-2].Line,
                                Pack[i-2].Column);}
                            if (!(Target2.ValueType == VALUE_NOPOINTNUMBER_TYPE || Target2.ValueType == VALUE_POINTNUMBER_TYPE)) {ErrCall("Type Error2","MG0011111","",
                                Pack[i].Line,
                                Pack[i].Column);}

                            if (Target1.ValueType == 3) UsePointNumber = 1;
                            if (Target2.ValueType == 3) UsePointNumber = 1;

                            if (UsePointNumber)
                            {
                                long double Value1 = 0;
                                long double Value2 = 0;



                                if (Target2.ValueType == 3)
                                {
                                    if (Target1.ValueType == 3) Value1 = Target1.PNumber; else Value1 = (long double)Target1.NPNumber;
                                    Value2 = Target2.PNumber;
                                }
                                else
                                {
                                    Value1 = Target1.PNumber;
                                    Value2 = (long double)Target2.NPNumber;

                                }

                                Out = (MioneObj){
                                    .ObjType = VALUE,
                                    .Val = (ValueObj){
                                        .ValueType = 3,
                                        .PNumber = Value1 + Value2,
                                    },
                                    .Line = Pack[i].Line,
                                    .Column = Pack[i].Column
                                };

                                PastCost = 2;
                            }
                            else
                            {
                                long int Value1 = 0;
                                long int Value2 = 0;

                                Value1 = Target1.NPNumber;
                                Value2 = Target2.NPNumber;




                                Out = (MioneObj){
                                    .ObjType = VALUE,
                                    .Val = (ValueObj){
                                        .ValueType = 2,
                                        .NPNumber = Value1 + Value2,
                                    },
                                    .Line = Pack[i].Line,
                                    .Column = Pack[i].Column
                                };

                                PastCost = 2;
                            }
                        }
                        else
                        {
                            ErrCall(
                                "You must only connect Two-side-count-SymbolGet.a to VV(Variable or Value).",
                                "MG005",
                                "Maybe you can try `1+1` or anything else.",
                                Pack[i].Line,
                                Pack[i].Column
                            );
                        }
                        CalculateType = 0;
                        break;
                        }
                case 2: // ,
                        {

                            ValueObj Target = Pack[i].ObjType == VARIABLE ? Pack[i].VarUP->Val : Pack[i].Val;

                            Out = (MioneObj){
                                .ObjType = VALUE,
                                .Val = Target,
                                .Line = Pack[i].Line,
                                .Column = Pack[i].Column
                            };

                            PastCost = 1;


                            CalculateType = 0;

                            break;
                        }
                    case 3: // *
                        {


                        if (Pack[i - 2].ObjType == VARIABLE || Pack[i - 2].ObjType == VALUE)
                        {
                            ValueObj Target1 ;
                            ValueObj Target2 ;

                            if (Pack[i - 2].ObjType == VARIABLE) Target1 = Pack[i - 2].VarUP->Val; else Target1 = Pack[i - 2].Val;
                            if (Pack[i].ObjType == VARIABLE) Target2 = Pack[i].VarUP->Val; else Target2 = Pack[i].Val;

                            if (!(Target1.ValueType == 2 || Target1.ValueType == 3)) ErrCall("Type Error 1","MG0011113","",
                                Pack[i-2].Line,
                                Pack[i-2].Column);
                            if (!(Target2.ValueType == 2 || Target2.ValueType == 3)) ErrCall("Type Error 2","MG0011113","",
                                Pack[i].Line,
                                Pack[i].Column);

                            if (Target1.ValueType == 3) UsePointNumber = 1;
                            if (Target2.ValueType == 3) UsePointNumber = 1;


                            if (UsePointNumber)
                            {
                                long double Value1 = 0;
                                long double Value2 = 0;



                                if (Target2.ValueType == 3)
                                {
                                    if (Target1.ValueType == 3) Value1 = Target1.PNumber; else Value1 = (long double)Target1.NPNumber;
                                    Value2 = Target2.PNumber;
                                }
                                else
                                {
                                    Value1 = Target1.PNumber;
                                    Value2 = (long double)Target2.NPNumber;

                                }

                                Out = (MioneObj){
                                    .ObjType = VALUE,
                                    .Val = (ValueObj){
                                        .ValueType = 3,
                                        .PNumber = Value1 * Value2,
                                    },
                                    .Line = Pack[i].Line,
                                    .Column = Pack[i].Column
                                };

                                PastCost = 2;
                            }
                            else
                            {
                                long int Value1 = 0;
                                long int Value2 = 0;

                                Value1 = Target1.NPNumber;
                                Value2 = Target2.NPNumber;




                                Out = (MioneObj){
                                    .ObjType = VALUE,
                                    .Val = (ValueObj){
                                        .ValueType = 2,
                                        .NPNumber = Value1 * Value2,
                                    },
                                    .Line = Pack[i].Line,
                                    .Column = Pack[i].Column
                                };

                                PastCost = 2;
                            }
                        }
                        else
                        {
                            ErrCall(
                                "You must only connect Two-side-count-SymbolGet.a to VV(Variable or Value).",
                                "MG005",
                                "Maybe you can try `1*1` or anything else.",
                                Pack[i].Line,
                                Pack[i].Column
                            );
                        }
                        CalculateType = 0;
                        break;

                        }
                case 6: //-
                        {
                             if (i-2>=0 && (Pack[i - 2].ObjType == VARIABLE || Pack[i - 2].ObjType == VALUE))
                    {
                        // 1- 1
                         ValueObj Target1 ;
                            ValueObj Target2 ;





                            if (Pack[i - 2].ObjType == VARIABLE) Target1 = Pack[i - 2].VarUP->Val; else Target1 = Pack[i - 2].Val;
                            if (Pack[i].ObjType == VARIABLE) Target2 = Pack[i].VarUP->Val; else Target2 = Pack[i].Val;

                        if (!(Target1.ValueType == 2 || Target1.ValueType == 3)) ErrCall("Type Error 3","MG0011116","",
                            Pack[i-2].Line,
                            Pack[i-2].Column);
                        if (!(Target2.ValueType == 2 || Target2.ValueType == 3)) ErrCall("Type Error 4","MG0011116","",
                            Pack[i].Line,
                            Pack[i].Column);

                            if (Target1.ValueType == 3) UsePointNumber = 1;
                            if (Target2.ValueType == 3) UsePointNumber = 1;

                            if (UsePointNumber)
                            {
                                long double Value1 = 0;
                                long double Value2 = 0;



                                if (Target2.ValueType == 3)
                                {
                                    if (Target1.ValueType == 3) Value1 = Target1.PNumber; else Value1 = (long double)Target1.NPNumber;
                                    Value2 = Target2.PNumber;
                                }
                                else
                                {
                                    Value1 = Target1.PNumber;
                                    Value2 = (long double)Target2.NPNumber;

                                }

                                Out = (MioneObj){
                                    .ObjType = VALUE,
                                    .Val = (ValueObj){
                                        .ValueType = 3,
                                        .PNumber = Value1 -  Value2,
                                    },
                                    .Line = Pack[i].Line,
                                    .Column = Pack[i].Column
                                };

                                PastCost = 2;
                            }
                            else
                            {
                                long int Value1 = 0;
                                long int Value2 = 0;

                                Value1 = Target1.NPNumber;
                                Value2 = Target2.NPNumber;




                                Out = (MioneObj){
                                    .ObjType = VALUE,
                                    .Val = (ValueObj){
                                        .ValueType = 2,
                                        .NPNumber = Value1 - Value2,
                                    },
                                    .Line = Pack[i].Line,
                                    .Column = Pack[i].Column
                                };

                                PastCost = 2;
                            }
                    }else
                    {
                        // -1
                        ValueObj Target;



                        if (Pack[i].ObjType == VARIABLE) Target = Pack[i].VarUP->Val;
                        if (Pack[i].ObjType == VALUE) Target = Pack[i].Val;

                        if (Target.ValueType == 3) UsePointNumber = 1;


                        if (Target.ValueType == 2 || Target.ValueType == 3){}else
                        {
                            ErrCall("Type error aaaa","MG00111","aa",
                                Pack[i].Line,
                                Pack[i].Column);
                        }


                        if (UsePointNumber)
                        {
                            long double Value1 = 0;

                            Value1 = Target.PNumber*-1;


                            Out = (MioneObj){
                                .ObjType = VALUE,
                                .Val = (ValueObj){
                                    .ValueType = 3,
                                    .PNumber = Value1
                                },
                                    .Line = Pack[i].Line,
                                    .Column = Pack[i].Column
                            };

                            PastCost = 1;
                        }else
                        {
                            long int Value1 = 0;

                            Value1 = Target.NPNumber*-1;


                            Out = (MioneObj){
                                .ObjType = VALUE,
                                .Val = (ValueObj){
                                    .ValueType = 2,
                                    .NPNumber = Value1
                                },
                                    .Line = Pack[i].Line,
                                    .Column = Pack[i].Column
                            };

                            PastCost = 1;
                        }

                    }
                    CalculateType = 0;
                    break;
                        }
                case 7: // .
                        {
                             if (i-2>=0 && (Pack[i - 2].ObjType == VARIABLE || Pack[i - 2].ObjType == VALUE))
                    {
                        // 1.1
                            ValueObj Target1 ;
                            ValueObj Target2 ;


                        if (Pack[i - 2].ObjType == VARIABLE) ErrCall("to do ; Variable can`t be Point Number","MG00111","IT IS TODO NOT ERROR",
                            Pack[i].Line,
                            Pack[i].Column); else Target1 = Pack[i - 2].Val;
                        if (Pack[i].ObjType == VARIABLE) ErrCall("to do ; Variable can`t be Point Number","MG00111","IT IS TODO NOT ERROR",
                            Pack[i].Line,
                            Pack[i].Column); else Target2 = Pack[i].Val;

                        if (!(Target1.ValueType == 2 || Target1.ValueType == 3)) ErrCall("TODO!!!","MG001131211","",
                            Pack[i-2].Line,
                            Pack[i-2].Column);
                        if (!(Target2.ValueType == 2 || Target2.ValueType == 3)) ErrCall("TODO!!!","MG001131211","",
                            Pack[i].Line,
                            Pack[i].Column);

                            if (Target1.ValueType == 3) UsePointNumber = 1;
                            if (Target2.ValueType == 3) UsePointNumber = 1;

                            if (UsePointNumber)
                            {
                                ErrCall("Point Number can`t change in to another Point Number","123456","",
                                Pack[i].Line,
                                Pack[i].Column);
                            }
                            else
                            {
                                long double Value1 = 0;
                                long double Value2 = 0;

                                Value1 = Target1.NPNumber;
                                Value2 = Target2.NPNumber;

                                long double Points = 0;
                                //LOCKED MAX NUMBER = 10^10
                                for (int x = 1; x < 10; x++)
                                {
                                    int cmp=1;
                                    for (int y = 1; y < x; y++)cmp*=10;

                                    if (cmp>Value2)
                                    {
                                        Points = Value2/cmp;
                                        break;
                                    }
                                }


                                Out = (MioneObj){
                                    .ObjType = VALUE,
                                    .Val = (ValueObj){
                                        .ValueType = 3,
                                        .PNumber = Value1*1 + Points,
                                    },
                                    .Line = Pack[i].Line,
                                    .Column = Pack[i].Column
                                };

                                PastCost = 2;
                            }
                    }else
                    {
                        // .1
                        ValueObj Target;

                        if (Pack[i].ObjType == VARIABLE) ErrCall("to do ; Variable can`t be Point Number","MG00111","IT IS TODO NOT ERROR",
                                Pack[i].Line,
                                Pack[i].Column);
                        if (Pack[i].ObjType == VALUE) Target = Pack[i].Val;

                        if (Target.ValueType == 3) UsePointNumber = 1;

                        if (Target.ValueType == 2 || Target.ValueType == 3){}else
                        {
                            ErrCall("Type error aaaabbbbbb","MG00111","aa",
                                Pack[i].Line,
                                Pack[i].Column);
                        }


                        if (UsePointNumber)
                        {
                            ErrCall("Point Number can`t change in to another Point Number","123456","",
                                Pack[i].Line,
                                Pack[i].Column);
                        }else
                        {
                            long int Value1 = 0;

                            Value1 = Target.NPNumber;

                            long double Points = 0;
                            //LOCKED MAX NUMBER = 10^10
                            for (int x = 1; x < 10; x++)
                            {
                                int cmp=1;
                                for (int y = 1; y < x; y++)cmp*=10;

                                if (cmp>Value1)
                                {

                                    Points = (long double)Value1/cmp;

                                    break;
                                }
                            }

                            Out = (MioneObj){
                                .ObjType = VALUE,
                                .Val = (ValueObj){
                                    .ValueType = 3,
                                    .PNumber = Points
                                },
                                    .Line = Pack[i].Line,
                                    .Column = Pack[i].Column
                            };

                            PastCost = 1;
                        }

                    }
                    CalculateType = 0;
                    break;
                        }
                case 8:
                        {
                             if (Pack[i - 2].ObjType == VARIABLE || Pack[i - 2].ObjType == VALUE)
                    {
                        ValueObj Target1 ;
                        ValueObj Target2 ;

                        if (Pack[i - 2].ObjType == VARIABLE) Target1 = Pack[i - 2].VarUP->Val; else Target1 = Pack[i - 2].Val;
                        if (Pack[i].ObjType == VARIABLE) Target2 = Pack[i].VarUP->Val; else Target2 = Pack[i].Val;

                        int db = 0;


                        if (Target1.ValueType == Target2.ValueType)
                        {
                            switch (Target1.ValueType)
                            {
                            case 1: //string
                                if (strcmp(Target2.String,Target1.String)==0) db =1;
                                break;
                            case 2:
                                if ((Target2.ValueType == 2?Target2.NPNumber:Target2.PNumber) - Target1.NPNumber == 0) db =1;
                                break;
                            case 3:
                                if ((Target2.ValueType == 2?Target2.NPNumber:Target2.PNumber)  -  Target1.PNumber == 0) db =1;
                                break;
                            case 4:
                                if ((Target2.ValueType == 4 ? Target2.Area.Area : 0) == Target1.Area.Area) db =1;
                                break;

                            case 5:
                                if ((Target2.ValueType == 5 ? Target2.Area.Area : 0) == Target1.Area.Area) db =1;
                                break;

                            case 6:
                                if ((Target2.ValueType == 6 ? Target2.Table.CountedTable : 0) == Target1.Table.CountedTable) db =1;
                                break;
                            case 7:
                                if ((Target2.ValueType == 7 ? Target2.Area.Area : 0) == Target1.Area.Area) db =1;
                                break;
                            case 8:
                                if ((Target2.ValueType == 8 ? Target2.db : 0)  == Target1.db) db =1;
                                break;
                            default:
                                printf("%d %d how????\nAny question please contact me by email: calledhxx@gmaill.com .errOn COUNT.c\n",Target1.ValueType,Target2.ValueType);
                                exit(0xff);
                                break;
                            }
                        }




                        Out = (MioneObj){
                            .ObjType = VALUE,
                            .Val = (ValueObj){
                                .ValueType = VALUE_DB_TYPE,
                                .db = db,
                            },
                            .Line = Pack[i].Line,
                            .Column = Pack[i].Column
                        };

                        PastCost = 2;
                    }else
                        {
                            ErrCall(
                                "You must only connect Two-side-count-SymbolGet.a to VV(Variable or Value).",
                                "MG005",
                                "Maybe you can try `1*1` or anything else.",
                                Pack[i].Line,
                                Pack[i].Column
                            );
                        }
                        CalculateType = 0;
                        break;
                        }


                case 14:
                        {
                            ValueObj Target = Pack[i].ObjType == VARIABLE ? Pack[i].VarUP->Val : Pack[i].Val;

                            extern DefineVariableObj * Dvo;
                        extern int DvoSize;

                        VariableObj * FoundVarUP = NULL;

                       for (int DvoIndex = 0; DvoIndex < DvoSize; DvoIndex++)
                       {
                           for (int VariableIndex = 0; VariableIndex < *(Dvo[DvoIndex].VariablesSizeUP); VariableIndex++)
                           {
                               switch (Target.ValueType)
                               {
                               case VALUE_STRING_TYPE:
                                   {
                                       if (strcmp((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Name, Target.String) == 0)
                                       {
                                           FoundVarUP = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex];
                                       }
                                   }
                                   break;

                               case VALUE_NOPOINTNUMBER_TYPE:
                                   {
                                       if ((*Dvo[DvoIndex].VariableUPsUP)[VariableIndex]->Place, Target.NPNumber)
                                       {
                                           FoundVarUP = (*Dvo[DvoIndex].VariableUPsUP)[VariableIndex];

                                       }
                                   }
                                   break;
                                   default:
                                       {
                                           ErrCall("dasdaskdoppas","dasjiodjaosjdoias",NULL,Pack[i-1].Line,Pack[i-1].Column);
                                       }
                               }

                           }
                       }

                        if (!FoundVarUP)
                        {
                            FoundVarUP = malloc(sizeof(VariableObj));

                            if (Target.ValueType == VALUE_STRING_TYPE)
                            {
                                *FoundVarUP = (VariableObj){
                                    .Name = Target.String
                                };
                            }else
                            {
                                *FoundVarUP = (VariableObj){
                                    .Place = Target.NPNumber
                                };
                            }

                            (*Dvo[DvoSize-1].VariablesSizeUP)++;
                            (*Dvo[DvoSize-1].VariableUPsUP) = realloc((*Dvo[DvoSize-1].VariableUPsUP), (*Dvo[DvoSize-1].VariablesSizeUP) * sizeof(VariableObj**));
                            (*Dvo[DvoSize-1].VariableUPsUP)[(*Dvo[DvoSize-1].VariablesSizeUP)-1] = FoundVarUP;
                        }

                        Out = (MioneObj){
                            .ObjType = VALUE,
                            .Val = FoundVarUP->Val,
                        };

                        PastCost = 1;
                        CalculateType = 0;
                        break;
                        }

                    default:
                        ErrCall("dsa","不支援",NULL,Pack[i-1].Line,Pack[i-1].Column);
                        break;
                }

                MioneObj* NewPack = malloc(0);
                int NewPackSize = 0;


                for (int index = 0; index < i - PastCost; index++)
                {
                    NewPackSize++;
                    NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                    NewPack[NewPackSize - 1] = Pack[index];
                }

                NewPackSize++;
                NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                NewPack[NewPackSize - 1] = Out;
                int ToIndex = NewPackSize - 1;

                for (int index = i + 1; index < PackSize; index++)
                {
                    NewPackSize++;
                    NewPack = realloc(NewPack, sizeof(MioneObj) * (NewPackSize));
                    NewPack[NewPackSize - 1] = Pack[index];
                }

                PackSize = NewPackSize;
                Pack = NewPack;

                i = ToIndex-1;
            }
            }else
            {
                inBracketSize++;
                inBracket = realloc(inBracket, sizeof(MioneObj) * (inBracketSize));
                inBracket[inBracketSize - 1] = Pack[i];
            }
        }
        }

        if (i == PackSize - 1)
        {
            CalculateLevel++;
            i = -1;// ...困擾了我好久
            if (CalculateLevel > 3) { break; }

        }
    }

    ValueObj * VPack = malloc(0);
    int VPackSize = 0;

    //printf("sizeof %d\n",PackSize);


    for (int i = 0; i < PackSize; i++)
    {

        if ( Pack[i].ObjType == VALUE)
        {
            VPackSize ++;
            VPack = realloc(VPack, sizeof(ValueObj) * (VPackSize));
            VPack[VPackSize-1] = Pack[i].Val;
        }else if (Pack[i].ObjType == VARIABLE)
        {
            VPackSize ++;
            VPack = realloc(VPack, sizeof(ValueObj) * (VPackSize));
            VPack[VPackSize-1] = Pack[i].VarUP->Val;
        }
    }



    for (int index = 0; index < VPackSize; index++)
    {
        printf("%d/%d ",Pack[index].Line, Pack[index].Column);
        if (VPack[index].ValueType == 2) printf("`NPN`:`%d`\n",VPack[index].NPNumber);
        else if (VPack[index].ValueType == 1) printf("`STRING`:`%s`\n",VPack[index].String);
        else if (VPack[index].ValueType == 3) printf("`PN`:`%Lf`\n",VPack[index].PNumber);
        else if (VPack[index].ValueType == 8) printf("`db`:`%d`\n",VPack[index].db);
        else if (VPack[index].ValueType == 0) printf("`NULL`:`NON`\n");
        else printf("`UNDEFINED`\n");
    }

    if (BracketCur) ErrCall("cadasdas","dasdada",NULL,0,0);

    CountObj Returns = (CountObj){
        .Value = VPack,
        .ValueSize= VPackSize
    };

    return Returns;
}
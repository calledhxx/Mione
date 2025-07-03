//
// Created by calle on 24-12-28.
//

#include <inttypes.h>
#include <tgmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>


#include "OBJECTS.h"
#include "SYMBOL_DEF.h"
#include "PROMPT_DEF.h"
#include "HeadFile/AllHeads.h"
#include "MIONE.h"
#include "ERR.h"
#include "NUMBER.h"


VariableObj * retVarUP(ScopeObj * SVUup,const wchar_t* Name,const int Place)
{
    VariableObj * ret = 0;

    for (int VariableUPIndex = 0;VariableUPIndex < SVUup->VariableUPsSize;VariableUPIndex++)
    {
        if (
          (SVUup->VariableUPs[VariableUPIndex]->Name && wcscmp(SVUup->VariableUPs[VariableUPIndex]->Name,Name) == 0) ||
          (SVUup->VariableUPs[VariableUPIndex]->Place != 0 && SVUup->VariableUPs[VariableUPIndex]->Place == Place)
          )
            ret =SVUup->VariableUPs[VariableUPIndex];
    }

    if (!ret && SVUup->ParentUP) return retVarUP(SVUup->ParentUP,Name,Place);

    return ret;
}



MioneObj *CMO(CaseObj*CASES,int CASESIZE,
    int * SIZE,int LineADD,int ColumnADD,ScopeObj * SVUup)
{
    MioneObj *MIONE = 0;
    int MIONESIZE = 0;

    int ChildCount = 0; //子項數量

    int TableCount = 0; //表單子項數量

    CaseObj* Area = malloc(0);
    int AreaSize = 0;

    int Line = 0+LineADD;
    int Column = 0+ColumnADD;

    int goEndType = 0; //range or function or lights
    int CapLine = 0; //行

    SVUup->ChildUPsSize = 0;
    SVUup->ChildUPs = malloc(0);

    SVUup->VariableUPs = malloc(0);
    SVUup->VariableUPsSize = 0;

    ToReplaceValueForCMOObj * TRVFC = malloc(0);
    int TRVFCSize = 0;

    for (int i = 0; i <CASESIZE; i++)
    {

           int Paired =0; //Head Symbol Prompt Variable Value


           if (CASES[i].ObjType == 13)
           {
               Line++;
               Column = 0;

               continue;
           };

        //HEAD


         if (ChildCount == 0 && TableCount == 0) for (int Ci = 0;1; Ci++)
         {

             if (Heads[Ci].CurNumber == -1) break;

             if (wcscmp(CASES[i].ObjName,Heads[Ci].Name) == 0)  {
                 Column++;


                 (MIONESIZE)++;
                 (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
                 (MIONE)[(MIONESIZE)-1] = (MioneObj){
                     .ObjType= HEAD,
                     .Head = Heads[Ci],
                      .Line = Line,
                      .Column = Column,
                     .ScopeUP = SVUup,
                 };

                 Paired = HEAD;

             }
         }
        //PROMPT
         if (ChildCount == 0 && TableCount == 0) for (int Ci = 0;1; Ci++)
        {
             if (Prompts[Ci].CurNumber == -1) break;

             if (wcscmp(CASES[i].ObjName,Prompts[Ci].Name) == 0)
             {
                 Column++;
                 (MIONESIZE)++;
                 (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
                 (MIONE)[(MIONESIZE)-1] = (MioneObj){
                     .ObjType= PROMPT,
                     .Prompt = Prompts[Ci],
                      .Line = Line,
                     .Column = Column,
                     .ScopeUP = SVUup,
                 };
                 Paired = PROMPT;
             }
        }

        //SYMBOL
         if (ChildCount == 0 && TableCount == 0) for (int Ci = 0; 1; Ci++)
        {
             if (Symbols[Ci].Identification == -1)  break;

             if (wcscmp(CASES[i].ObjName,Symbols[Ci].Name) == 0)
             {
                 Column++;

                 (MIONESIZE)++;
                 (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
                 (MIONE)[(MIONESIZE)-1] = (MioneObj){
                     .ObjType= SYMBOL,
                     .Symbol = Symbols[Ci]
                     ,.Line = Line,
                     .Column = Column,
                     .ScopeUP = SVUup,

                 };
                 Paired = SYMBOL;
             }
        }

        //Value : String
         if (ChildCount == 0 && TableCount == 0) if (CASES[i].ObjType == SYMBOL|| CASES[i].ObjType == VARIABLE)
        {

             wchar_t*str=malloc(0);

             for (int s = 1; s<wcslen(CASES[i].ObjName); s++)
             {
                 str=realloc(str,(s+1)*sizeof(wchar_t));
                 str[s-1] = s == wcslen(CASES[i].ObjName)-1 ? L'\0' :  CASES[i].ObjName[s];
             }

             ValueObj Value = (ValueObj){.ValueType = VALUE_STRING_TYPE, .String = str};
             Column++;

            (MIONESIZE)++;
            (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
            (MIONE)[(MIONESIZE)-1] = (MioneObj){
                .ObjType= VALUE,
                .Val = Value,
                .Line = Line,
                .Column = Column,
                .ScopeUP = SVUup,
            };

             Paired = VALUE;
        }

           //Value : Table

           if (ChildCount == 0) if (wcscmp(CASES[i].ObjName,L"}") == 0)
           {
               TableCount--;
               if (!TableCount)
               {
                   Paired = VALUE;

                   Column++;

                   (MIONESIZE)++ ;
                   (MIONE) = (MioneObj*)realloc(MIONE, (MIONESIZE)*sizeof(MioneObj));
                   (MIONE)[(MIONESIZE)-1] = (MioneObj){
                       .ObjType = VALUE,

                       .Line = Line,
                       .Column = Column,
                     .ScopeUP = SVUup,
                   };


                   TRVFCSize++;
                   TRVFC = realloc(TRVFC,TRVFCSize*sizeof(ToReplaceValueForCMOObj));
                   TRVFC[TRVFCSize-1] = (ToReplaceValueForCMOObj){
                       .a = (ToCMObj){Area,AreaSize,CapLine,Column},
                       .ObjIndex = MIONESIZE-1,
                       .ValueType = VALUE_TABLE_TYPE
                   };


                   Area = NULL;
                   Area = malloc(0);
                   AreaSize = 0;

               }
           }
           if (ChildCount == 0 && TableCount)
           {
               AreaSize++;
               Area = realloc(Area,AreaSize*sizeof(CaseObj));
               Area[AreaSize-1] = CASES[i];
           }
           if (ChildCount == 0) if (wcscmp(CASES[i].ObjName,L"{") == 0)
           {
               if (!TableCount)
               {
                   CapLine = Line;
                   Paired = VALUE;
               }
               TableCount++;
           }

           if (ChildCount == 0 && TableCount == 0) if (wcscmp(CASES[i].ObjName,L";") == 0)
           {

               Paired = -1;
               Column++;

               (MIONESIZE)++;
               (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
               (MIONE)[(MIONESIZE)-1] = (MioneObj){
                   .ObjType= -1,
                   .Line = Line,
                   .Column = Column,
                     .ScopeUP = SVUup,
               };
           }

           //Value : lights,function or range END
           if (TableCount == 0) if (wcscmp(CASES[i].ObjName,L"end") == 0)
           {
               ChildCount--;
               if (ChildCount == 0 && TableCount == 0) //僅包覆最高層的子向
               {
                   Paired = VALUE;

                   Column++;

                   (MIONESIZE)++ ;
                   (MIONE) = (MioneObj*)realloc(MIONE, (MIONESIZE)*sizeof(MioneObj));
                   (MIONE)[(MIONESIZE)-1] = (MioneObj){
                       .ObjType = VALUE,

                       .Line = Line,
                       .Column = Column,
                       .ScopeUP = SVUup,
                   };


                   TRVFCSize++;
                   TRVFC = realloc(TRVFC,TRVFCSize*sizeof(ToReplaceValueForCMOObj));
                   TRVFC[TRVFCSize-1] = (ToReplaceValueForCMOObj){
                       .a = (ToCMObj){Area,AreaSize,CapLine,Column},
                       .ObjIndex = MIONESIZE-1,
                       .ValueType = goEndType
                   };
                   Area = NULL;
                   Area = malloc(0);
                   AreaSize = 0;
               }else
               {

                   AreaSize++;
                   Area = realloc(Area,AreaSize*sizeof(CaseObj));
                   Area[AreaSize-1] = CASES[i];
               }
           }else
           {

               if (ChildCount)
               {

                   AreaSize++;
                   Area = realloc(Area,AreaSize*sizeof(CaseObj));
                   Area[AreaSize-1] = CASES[i];
               }
           }
           //Value : Function
           if (TableCount == 0) if (wcscmp(CASES[i].ObjName,L"function") == 0)
           {
               ChildCount++;
               if (ChildCount == 1)
               {
                   CapLine = Line;
                   goEndType = VALUE_FUNCTION_TYPE;
                   Paired = VALUE;
               }
           }


           //Value : Range


           if (TableCount == 0) if (wcscmp(CASES[i].ObjName,L"range") == 0)
           {
               ChildCount++;
               if (ChildCount == 1)
               {
                   CapLine = Line;
                   goEndType = VALUE_RANGE_TYPE;
                   Paired = VALUE;
               }
           }

           //Value : coroutine
           //Value : Lights


           if (TableCount == 0) if (wcscmp(CASES[i].ObjName,L"lights") == 0)
           {
               ChildCount++;
               if (ChildCount == 1)
               {
                   CapLine = Line;
                   goEndType = VALUE_LIGHTS_TYPE;
                   Paired = VALUE;
               }
           }

           if (ChildCount == 0 && TableCount == 0)
           {

               if (wcscmp(CASES[i].ObjName,L"true") == 0)
               {

                   Paired = VALUE;
                   ValueObj Value = (ValueObj){.ValueType = VALUE_DB_TYPE, .db = 1};
                   Column++;

                   (MIONESIZE)++;
                   (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
                   (MIONE)[(MIONESIZE)-1] = (MioneObj){
                       .ObjType = VALUE,
                       .Val = Value,
                       .Line = Line,
                       .Column = Column,
                     .ScopeUP = SVUup,
                   };
               }else  if (wcscmp(CASES[i].ObjName,L"false") == 0)
               {

                   Paired = VALUE;
                   ValueObj Value = (ValueObj){.ValueType = VALUE_DB_TYPE, .db = 0};
                   Column++;

                   (MIONESIZE)++;
                   (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
                   (MIONE)[(MIONESIZE)-1] = (MioneObj){
                       .ObjType = VALUE,
                       .Val = Value,
                       .Line = Line,
                       .Column = Column,
                     .ScopeUP = SVUup,
                   };
               }else  if (wcscmp(CASES[i].ObjName,L"null") == 0)
               {
                   Paired = VALUE;
                   ValueObj Value = (ValueObj){.ValueType = 0};
                   Column++;

                   (MIONESIZE)++;
                   (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
                   (MIONE)[(MIONESIZE)-1] = (MioneObj){
                       .ObjType = VALUE,
                       .Val = Value,
                       .Line = Line,
                       .Column = Column,
                     .ScopeUP = SVUup,
                   };
               }
           }



        //Value : Number
            if (ChildCount == 0 && TableCount == 0) if(CASES[i].ObjType == 2)
           {
               Paired = VALUE;

                NumberObj Number = wcharToNumber(CASES[i].ObjName);

               ValueObj Value = (ValueObj){
                   .ValueType = VALUE_NUMBER_TYPE,
                   .Number = Number
               };


                Column++;

               (MIONESIZE)++;
               (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
               (MIONE)[(MIONESIZE)-1] = (MioneObj){
                   .ObjType = VALUE,
                   .Val = Value,
                   .Line = Line,
                   .Column = Column,
                     .ScopeUP = SVUup,
               };
           }
        //Variable
         if (ChildCount == 0 && TableCount == 0) if (Paired == 0 && (CASES[i].ObjType != 13))
        {
            Paired = VARIABLE;

             VariableObj* ret = retVarUP(SVUup,CASES[i].ObjName,0);

             if (!ret)
             {
                 ret = malloc(sizeof(VariableObj));
                 ret->Name = CASES[i].ObjName;

                 SVUup->VariableUPsSize++;
                 SVUup->VariableUPs = realloc(SVUup->VariableUPs,SVUup->VariableUPsSize*sizeof(VariableObj*));
                 SVUup->VariableUPs[SVUup->VariableUPsSize-1] = ret;
             }


             Column++;

            (MIONESIZE)++;
            (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
            (MIONE)[(MIONESIZE)-1] = (MioneObj){
                .ObjType = VARIABLE,
                .VarUP = ret,
                .Line = Line,
                .Column = Column,
                .ScopeUP = SVUup,
                .Val = (ValueObj){0},
            };
        };
    }


    if (ChildCount) ErrCall("END???","M111",NULL,Line,Column);
    if (TableCount) ErrCall("}???","M111",NULL,Line,Column);

    for (int i = 0;i<TRVFCSize;i++)
    {
        int MioObjSize = 0;

        ScopeObj * ChildSVUup = malloc(sizeof(ScopeObj));
        *ChildSVUup = (ScopeObj){0};
        ChildSVUup->ParentUP = SVUup;

        SVUup->ChildUPsSize++;
        SVUup->ChildUPs = realloc(SVUup->ChildUPs,SVUup->ChildUPsSize*sizeof(ScopeObj*));
        SVUup->ChildUPs[SVUup->ChildUPsSize-1] = ChildSVUup;

        MioneObj * MioObj = CMO(TRVFC[i].a.CASE,TRVFC[i].a.CASESIZE,&MioObjSize,TRVFC[i].a.LineADD,TRVFC[i].a.ColumnADD,ChildSVUup);
        MioneBuiltObj Built = ToMione((MioneToBuildObj){
            .Objs = MioObj,
            .ObjsSize = MioObjSize,
        });



        ValueObj Value;
        
        switch (TRVFC[i].ValueType)
        {
        case VALUE_TABLE_TYPE:
            {
                TableObj eTable = (TableObj){
                    .VariableUPsUP = 0,
                };
        
                eTable.TableAreaUP = malloc(sizeof(struct _MioneBuiltObject));
                *eTable.TableAreaUP = Built;

                Value = (ValueObj){
                    .ValueType = VALUE_TABLE_TYPE,
                    .Table = eTable,
                };

                break;
            }
        default:
            {
                AreaObj Area;

                Area.AreaUP = malloc(sizeof(struct _MioneBuiltObject));
                *Area.AreaUP = Built;

                Value = (ValueObj){
                    .ValueType = TRVFC[i].ValueType,
                    .Area = Area,
                };

                break;
            }
        }

        MIONE[TRVFC[i].ObjIndex].Val = Value;

    }

    (*SIZE) = (MIONESIZE);
    return MIONE;
}
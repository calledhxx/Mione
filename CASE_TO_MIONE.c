//
// Created by calle on 24-12-28.
//

#include <inttypes.h>
#include <tgmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OBJECTS.h"
#include "SYMBOL_DEF.h"
#include "PROMPT_DEF.h"
#include "HeadFile/AllHeads.h"
#include "MIONE.h"
#include "ERR.h"


MioneObj *CMO(CaseObj*CASES,int CASESIZE,
    int * SIZE,int LineADD,int ColumnADD,DefineVariableObj * *DvoUP,int * DvoSizeUP)
{
    MioneObj *MIONE = 0;
    int MIONESIZE = 0;

    int ChildCount = 0; //子項數量

    int TableCount = 0; //表單子項數量

    int Lock = -1; //被封鎖到...

    CaseObj* Area = malloc(0);
    int AreaSize = 0;

    int Line = 0+LineADD;
    int Column = 0+ColumnADD;

    int goEndType = 0; //range or function or lights
    int CapLine = 0; //行

    (*DvoSizeUP)++;
    *DvoUP = realloc(*DvoUP,(*DvoSizeUP)*sizeof(DefineVariableObj));
    (*DvoUP)[(*DvoSizeUP)-1] = (DefineVariableObj){
        .VariablesSizeUP = malloc(sizeof(int)),
        .VariableUPsUP = malloc(sizeof(VariableObj**)),
    };

    (*(*DvoUP)[(*DvoSizeUP)-1].VariablesSizeUP) = 0;
    (*(*DvoUP)[(*DvoSizeUP)-1].VariableUPsUP) = (VariableObj**)malloc(0);



    for (int i = 0; i <CASESIZE; i++)
    {

       if (Lock != i)
       {
           int Paired =0; //Head Symbol Prompt Variable Value

           if (CASES[i].ObjType == 13)
           {
               Line++;
               Column = 0;
           };

            // printf("`%s`\n",CASES[i].ObjName);

        //HEAD
         if (ChildCount == 0 && TableCount == 0) for (int Ci = 0;1; Ci++)
         {
             if (Heads[Ci].CurNumber == -1) break;

             if (strcmp(CASES[i].ObjName,Heads[Ci].Name) == 0)  {
                 Column++;

                 (MIONESIZE)++;
                 (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
                 (MIONE)[(MIONESIZE)-1] = (MioneObj){
                     .ObjType= HEAD,
                     .Head = Heads[Ci],
                      .Line = Line,
                      .Column = Column
                 };

                 Paired = HEAD;

             }
         }
        //PROMPT
         if (ChildCount == 0 && TableCount == 0) for (int Ci = 0;1; Ci++)
        {
             if (Prompts[Ci].CurNumber == -1) break;

             if (strcmp(CASES[i].ObjName,Prompts[Ci].Name) == 0)
             {
                 Column++;
                 (MIONESIZE)++;
                 (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
                 (MIONE)[(MIONESIZE)-1] = (MioneObj){
                     .ObjType= PROMPT,
                     .Prompt = Prompts[Ci],
                      .Line = Line,
                     .Column = Column
                 };
                 Paired = PROMPT;
             }


        }


        //SYMBOL
         if (ChildCount == 0 && TableCount == 0) for (int Ci = 0; 1; Ci++)
        {
             if (Symbols[Ci].CurNumber == -1)  break;

             if (strcmp(CASES[i].ObjName,Symbols[Ci].Name) == 0)
             {
                 Column++;

                 (MIONESIZE)++;
                 (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
                 (MIONE)[(MIONESIZE)-1] = (MioneObj){
                     .ObjType= SYMBOL,
                     .Symbol = Symbols[Ci]
                     ,.Line = Line,
                     .Column = Column
                 };
                 Paired = SYMBOL;
             }
        }

        //Value : String
         if (ChildCount == 0 && TableCount == 0) if (CASES[i].ObjType == SYMBOL|| CASES[i].ObjType == VARIABLE)
        {

             char*str=malloc(0);

             for (int s = 1; s<strlen(CASES[i].ObjName); s++)
             {
                 str=realloc(str,s+1);
                 str[s-1] = s == strlen(CASES[i].ObjName)-1 ? '\0' :  CASES[i].ObjName[s];
             }

            ValueObj Value = (ValueObj){.ValueType = VALUE_STRING_TYPE, .String = str};
             Column++;

            (MIONESIZE)++;
            (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
            (MIONE)[(MIONESIZE)-1] = (MioneObj){
                .ObjType= VALUE,
                .Val = Value,
                .Line = Line,
                .Column = Column
            };

             Paired = VALUE;

        }

           //Value : Table

           if (ChildCount == 0) if (strcmp(CASES[i].ObjName,"}") == 0)
           {
               TableCount--;
               if (!TableCount)
               {
                   Paired = 5;

                   int MioObjSize = 0;

                   MioneObj * MioObj = CMO(Area,AreaSize,&MioObjSize,CapLine,Column,DvoUP,DvoSizeUP);
                   // VariableObj * TbMemory = malloc(0);
                   // int  TbMemorySize = 0;
                   //
                   // MioneReturnObj Tb = Table(MioObj,MioObjSize,&TbMemory,&TbMemorySize);


                   TableObj eTable = (TableObj){
                       .MioneTable = MioObj,
                       .MioneTableSize = MioObjSize,
                       .CountedTable = NULL,
                       .CountedTableSize = 0,

                       .Counted = 0
                   };

                   ValueObj Value = (ValueObj){
                       .ValueType = VALUE_TABLE_TYPE,
                       .Table = eTable,
                   };

                   Column++;

                   (MIONESIZE)++ ;
                   (MIONE) = (MioneObj*)realloc(MIONE, (MIONESIZE)*sizeof(MioneObj));
                   (MIONE)[(MIONESIZE)-1] = (MioneObj){
                       .ObjType = VALUE,
                       .Val = Value,
                       .Line = Line,
                       .Column = Column
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
           if (ChildCount == 0) if (strcmp(CASES[i].ObjName,"{") == 0)
           {
               Paired = VALUE;
               TableCount++;
           }




           //Value : lights,function or range END 
           if (TableCount == 0) if (strcmp(CASES[i].ObjName,"end") == 0)
           {
               ChildCount--;
               if (ChildCount == 0 && TableCount == 0) //僅包覆最高層的子向
               {
                   int MioObjSize = 0;

                   MioneObj * MioObj = CMO(Area,AreaSize,&MioObjSize,CapLine,Column,DvoUP,DvoSizeUP);


                   AreaObj eArea = (AreaObj){
                       .Area =MioObj,
                       .Size = MioObjSize,
                       .Index = MIONESIZE,
                   };

                   ValueObj Value = (ValueObj){
                       .ValueType = goEndType,
                       .Area = eArea,
                   };


                   Column++;

                   (MIONESIZE)++ ;
                   (MIONE) = (MioneObj*)realloc(MIONE, (MIONESIZE)*sizeof(MioneObj));
                   (MIONE)[(MIONESIZE)-1] = (MioneObj){
                       .ObjType = VALUE,
                       .Val = Value,
                       .Line = Line,
                       .Column = Column
                   };

                   Area = NULL;
                   Area = malloc(0);
                   AreaSize = 0;

                   goEndType = 0;

                   Paired = VALUE;
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
           if (TableCount == 0) if (strcmp(CASES[i].ObjName,"function") == 0)
           {
               ChildCount++;
               if (ChildCount == 1)
               {
                   CapLine = Line;
                   goEndType = VALUE_FUNCTION_TYPE;
                   Paired = 5;
               }
           }


           //Value : Range


           if (TableCount == 0) if (strcmp(CASES[i].ObjName,"range") == 0)
           {
               ChildCount++;
               if (ChildCount == 1)
               {
                   CapLine = Line;
                   goEndType = VALUE_RANGE_TYPE;
                   Paired = 5;
               }
           }

           //Value : Lights


           if (TableCount == 0) if (strcmp(CASES[i].ObjName,"lights") == 0)
           {
               ChildCount++;
               if (ChildCount == 1)
               {
                   CapLine = Line;
                   goEndType = VALUE_LIGHTS_TYPE;
                   Paired = 5;
               }
           }

           if (ChildCount == 0 && TableCount == 0) if (strcmp(CASES[i].ObjName,"true") == 0)
           {
               Paired = VALUE;
               ValueObj Value = (ValueObj){.ValueType = VALUE_DB_TYPE, .db = 1};
               Column++;

               (MIONESIZE)++;
               (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
               (MIONE)[(MIONESIZE)-1] = (MioneObj){
                   .ObjType= VALUE,
                   .Val = Value,
                   .Line = Line,
                   .Column = Column
               };
           }
           if (ChildCount == 0 && TableCount == 0) if (strcmp(CASES[i].ObjName,"false") == 0)
           {
               Paired = VALUE;
               ValueObj Value = (ValueObj){.ValueType = VALUE_DB_TYPE, .db = 0};
               Column++;

               (MIONESIZE)++;
               (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
               (MIONE)[(MIONESIZE)-1] = (MioneObj){
                   .ObjType= VALUE,
                   .Val = Value,
                   .Line = Line,
                   .Column = Column
               };
           }

        //Value : NPNumber
            if (ChildCount == 0 && TableCount == 0) if(CASES[i].ObjType == 2)
           {
               Paired = VALUE;

               long int V = 0;
               V=V+atoi(CASES[i].ObjName);

               ValueObj Value = (ValueObj){
                   .ValueType = VALUE_NOPOINTNUMBER_TYPE,
                   .NPNumber = V
               };
                Column++;

               (MIONESIZE)++;
               (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
               (MIONE)[(MIONESIZE)-1] = (MioneObj){
                   .ObjType = VALUE,
                   .Val = Value,
                   .Line = Line,
                   .Column = Column
               };
           }
        //Variable
         if (ChildCount == 0 && TableCount == 0) if (Paired == 0 && (CASES[i].ObjType != 13))
        {
            Paired = VARIABLE;

            int NewVar = 1;

            VariableObj* VariableUP ;


             for (int DvoIndex = 0; DvoIndex < *DvoSizeUP; DvoIndex++)
             {
                 for (int j = 0; j < *((*DvoUP)[DvoIndex].VariablesSizeUP); j++)
                 {
                     if (strcmp((*(*DvoUP)[DvoIndex].VariableUPsUP)[j]->Name ,CASES[i].ObjName)==0)
                     {
                         NewVar = 0;
                         VariableUP = (*(*DvoUP)[DvoIndex].VariableUPsUP)[j];
                         break;

                     }
                 }
             }

             if (NewVar)
             {
                 VariableUP = malloc(sizeof (VariableObj));
                 *VariableUP = (VariableObj){
                     .Name = CASES[i].ObjName,
                 };


                 (*((*DvoUP)[(*DvoSizeUP)-1].VariablesSizeUP))++;
                 (*((*DvoUP)[(*DvoSizeUP)-1].VariableUPsUP)) = realloc((*((*DvoUP)[(*DvoSizeUP)-1].VariableUPsUP)),(*((*DvoUP)[(*DvoSizeUP)-1].VariablesSizeUP))*sizeof(VariableObj**));
                 (*((*DvoUP)[(*DvoSizeUP)-1].VariableUPsUP))[(*((*DvoUP)[(*DvoSizeUP)-1].VariablesSizeUP))-1] = VariableUP;

             }


             Column++;

            (MIONESIZE)++;
            (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
            (MIONE)[(MIONESIZE)-1] = (MioneObj){
                .ObjType = VARIABLE,
                .VarUP = VariableUP,
                .Line = Line,
                 .Column = Column
            };
        };

           //printf("%s\n",CASES[i].ObjName);


       }
    }
    if (ChildCount) ErrCall("END???","M111",NULL,Line,Column);
    if (TableCount) ErrCall("}???","M111",NULL,Line,Column);


    (*SIZE) = (MIONESIZE);
    return MIONE;
}
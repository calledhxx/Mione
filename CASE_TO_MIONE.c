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

    int goEndType = 0; //range or function or lights


    CaseObj* Area = malloc(0);
    int AreaSize = 0;

    int Line = 0+LineADD;
    int Column = 0+ColumnADD;

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
                     .ObjType= 1,
                     .Head = Heads[Ci],
                      .Line = Line,
                      .Column = Column
                 };

                 Paired = 1;

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
                     .ObjType= 2,
                     .Prompt = Prompts[Ci],
                      .Line = Line,
                     .Column = Column
                 };
                 Paired = 2;
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
                     .ObjType= 3,
                     .Symbol = Symbols[Ci]
                     ,.Line = Line,
                     .Column = Column
                 };
                 Paired = 3;
             }
        }

        //Value : String
         if (ChildCount == 0 && TableCount == 0) if (CASES[i].ObjType == 3|| CASES[i].ObjType == 4)
        {

            Paired = 5;
             char*str=malloc(0);

             for (int s = 1; s<strlen(CASES[i].ObjName); s++)
             {
                 str=realloc(str,s+1);
                 str[s-1] = s == strlen(CASES[i].ObjName)-1 ? '\0' :  CASES[i].ObjName[s];
             }

            ValueObj Value = (ValueObj){.ValueType = 1, .String = str};
             Column++;

            (MIONESIZE)++;
            (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
            (MIONE)[(MIONESIZE)-1] = (MioneObj){
                .ObjType= 5,
                .Val = Value,
                .Line = Line,
                .Column = Column
            };


        }

           //Value : Table

           if (ChildCount == 0) if (strcmp(CASES[i].ObjName,"}") == 0)
           {
               TableCount--;
               if (!TableCount)
               {
                   Paired = 5;

                   int MioObjSize = 0;

                   MioneObj * MioObj = CMO(Area,AreaSize,&MioObjSize,MIONE[MIONESIZE-1].Line,MIONE[MIONESIZE-1].Column,DvoUP,DvoSizeUP);

                   VariableObj * TbMemory = malloc(0);
                   int  TbMemorySize = 0;

                   MioneReturnObj Tb = Table(MioObj,MioObjSize,&TbMemory,&TbMemorySize);

                   printf("N %d\n",TbMemory[1].Val.NPNumber);


                   TableObj eTable = (TableObj){
                       .Table = TbMemory,
                       .Size = TbMemorySize,
                   };

                   ValueObj Value = (ValueObj){
                       .ValueType = 6,
                       .Table = eTable,
                   };

                   Column++;

                   (MIONESIZE)++ ;
                   (MIONE) = (MioneObj*)realloc(MIONE, (MIONESIZE)*sizeof(MioneObj));
                   (MIONE)[(MIONESIZE)-1] = (MioneObj){
                       .ObjType = 5,
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
               Paired = 5;
               TableCount++;
           }




           //Value : lights,function or range END 
           if (TableCount == 0)if (strcmp(CASES[i].ObjName,"end") == 0)
           {

               ChildCount--;
               if (ChildCount == 0 && TableCount == 0) //僅包覆最高層的子向
               {
                   Paired = 5;

                   int MioObjSize = 0;

                   MioneObj * MioObj = CMO(Area,AreaSize,&MioObjSize,MIONE[MIONESIZE-1].Line,MIONE[MIONESIZE-1].Column,DvoUP,DvoSizeUP);



                   AreaObj eArea = (AreaObj){
                       .Area =MioObj,
                       .Size = MioObjSize,
                       .Index = MIONESIZE,
                   };

                   ValueObj Value = (ValueObj){
                       .ValueType = goEndType == 1 ? 5 : (goEndType == 2 ? 4 : 7),
                       .Area = eArea,
                   };


                   Column++;

                   (MIONESIZE)++ ;
                   (MIONE) = (MioneObj*)realloc(MIONE, (MIONESIZE)*sizeof(MioneObj));
                   (MIONE)[(MIONESIZE)-1] = (MioneObj){
                       .ObjType = 5,
                       .Val = Value,
                       .Line = Line,
                       .Column = Column
                   };

                   Area = NULL;
                   Area = malloc(0);
                   AreaSize = 0;

                   goEndType = 0;
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
                   goEndType = 2;
                   Paired = 5;

               }
           }


           //Value : Range


           if (TableCount == 0) if (strcmp(CASES[i].ObjName,"range") == 0)
           {
               ChildCount++;
               if (ChildCount == 1)
               {
                   goEndType = 1;
                   Paired = 5;

               }
           }

           //Value : Lights


           if (TableCount == 0) if (strcmp(CASES[i].ObjName,"lights") == 0)
           {
               ChildCount++;
               if (ChildCount == 1)
               {
                   goEndType = 3;
                   Paired = 5;
               }
           }

           if (ChildCount == 0 && TableCount == 0) if (strcmp(CASES[i].ObjName,"true") == 0)
           {
               Paired = 5;
               ValueObj Value = (ValueObj){.ValueType = 8, .db = 1};
               Column++;

               (MIONESIZE)++;
               (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
               (MIONE)[(MIONESIZE)-1] = (MioneObj){
                   .ObjType= 5,
                   .Val = Value,
                   .Line = Line,
                   .Column = Column
               };
           }
           if (ChildCount == 0 && TableCount == 0) if (strcmp(CASES[i].ObjName,"false") == 0)
           {
               Paired = 5;
               ValueObj Value = (ValueObj){.ValueType = 8, .db = 0};
               Column++;

               (MIONESIZE)++;
               (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
               (MIONE)[(MIONESIZE)-1] = (MioneObj){
                   .ObjType= 5,
                   .Val = Value,
                   .Line = Line,
                   .Column = Column
               };
           }

        //Value : NPNumber
            if (ChildCount == 0 && TableCount == 0) if(CASES[i].ObjType == 2)
           {
               Paired = 5;

               long int V = 0;
               V=V+atoi(CASES[i].ObjName);

               ValueObj Value = (ValueObj){
                   .ValueType = 2,
                   .NPNumber = V
               };
                Column++;

               (MIONESIZE)++;
               (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
               (MIONE)[(MIONESIZE)-1] = (MioneObj){
                   .ObjType = 5,
                   .Val = Value,
                   .Line = Line,
                   .Column = Column
               };
           }
        //Variable
         if (ChildCount == 0 && TableCount == 0) if (Paired == 0 && (CASES[i].ObjType != 13))
        {
            Paired = 4;

            int NewVar = 1;

            VariableObj* VariableUP = malloc(sizeof (VariableObj));


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
                 *VariableUP = (VariableObj){
                     .Name = CASES[i].ObjName,
                 };

                 VariableObj * VUP = malloc(sizeof(VariableObj));
                 VUP = VariableUP;


                 (*((*DvoUP)[(*DvoSizeUP)-1].VariablesSizeUP))++;
                 (*((*DvoUP)[(*DvoSizeUP)-1].VariableUPsUP)) = realloc((*((*DvoUP)[(*DvoSizeUP)-1].VariableUPsUP)),(*((*DvoUP)[(*DvoSizeUP)-1].VariablesSizeUP))*sizeof(VariableObj**));
                 (*((*DvoUP)[(*DvoSizeUP)-1].VariableUPsUP))[(*((*DvoUP)[(*DvoSizeUP)-1].VariablesSizeUP))-1] = VUP;

             }


             Column++;

            (MIONESIZE)++;
            (MIONE) = (MioneObj*)realloc( (MIONE) ,(MIONESIZE)*sizeof(MioneObj));
            (MIONE)[(MIONESIZE)-1] = (MioneObj){
                .ObjType = 4,
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
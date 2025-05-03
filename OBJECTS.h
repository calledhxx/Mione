//
// Created by chenn on 24-8-3.
//

#define HEAD 1
#define PROMPT 2
#define SYMBOL 3
#define VARIABLE  4
#define VALUE 5

#define VALUE_STRING_TYPE 1
#define VALUE_NOPOINTNUMBER_TYPE 2
#define VALUE_POINTNUMBER_TYPE 3
#define VALUE_FUNCTION_TYPE 4
#define VALUE_RANGE_TYPE 5
#define VALUE_TABLE_TYPE 6
#define VALUE_LIGHTS_TYPE 7
#define VALUE_DB_TYPE 8
#define VALUE_COROUTINE_TYPE 9

#ifndef OBJECTS_H
#define OBJECTS_H

typedef struct _AreaObject
{
    struct _MioneBuiltObject * AreaUP;
}AreaObj;

typedef struct _TableObject
{
    struct _MioneBuiltObject * TableAreaUP;

    struct _VariablesObject * VariablesUP;
}TableObj;

typedef struct _ValueObject
{
    int ValueType; //值類型
    /*
    1:字串
    2:整數
    3:浮點數
    4:函數
    5:執行式
    6:表單
    7:開關
    8:布林值
    9:mione塊
    */
    struct _AreaObject Area; //給於函數(function),開關(lights)或者執行式(range)。
    char * String; //給予文字(string)。
    long int NPNumber; //給予無小數點數字(no point number)。
    long double PNumber; //給予小數點數字(point number)。
    struct _TableObject Table; //給予表格(table)。
    int db;//布林
} ValueObj;

typedef struct _VariableObject
{
    char* Name; //變數名稱
    int Place; //位置
    ValueObj Val; //值

} VariableObj;

typedef struct _SymbolObject
{
    char * Name;
    int SymbolType;
    int CurNumber;
    int AfterConnectVV; //後面是否可與VV相連 不會報錯 MIONE

    int xIndex;
    int yIndex;
}SymbolObj;


typedef struct _PromptObject
{
    char * Name;
    int CurNumber;

}PromptObj;

typedef struct _HeadObject
{
    char * Name;
    int CurNumber;
    struct _HeadReturnObject (*Fuc)(struct _HeadRequestObject *);
}HeadObj;

typedef struct _MioneObject
{
    int ObjType; //HPSVV宏 1H 2P 3S 4VAR 5VAL 0換行

    VariableObj * VarUP; //當ObjType為VAR時，會用到此變數。
    ValueObj Val;  //當ObjType為VALUE宏時，會用到此值。
    //char* _Text;

    SymbolObj Symbol; //當ObjType為SYMBOL時，會用到此符號。
    PromptObj Prompt; //當ObjType為PROMPT時，會用到此提示。
    HeadObj Head; //當ObjType為HEAD時，會用到此標題。

    int Line; //行號
    int Column; //列號

    struct _ScopeObject* ScopeUP; //作用域

} MioneObj;

typedef struct _CaseObject
{
    int ObjType; /*
    CharType...
    */
    char * ObjName;
} CaseObj;

typedef struct _PairObject
{
    MioneObj Prompt;

    MioneObj* Source;

    int SourceSize;

}PairObj;

typedef struct _CountObject
{
    ValueObj* Value;
    int ValueSize;
} CountObj;

typedef struct _ValuesObject
{
    ValueObj* Value;
    int ValueSize;
} ValuesObj;

typedef struct _DefinedVariableObject
{
    ValueObj Value;
    VariableObj* TheDefinedVarUP;
}DefinedVariableObj;

typedef struct _DefinedVariablesCaseObject
{
    DefinedVariableObj * DefinedVariables;
    int DefinedVariablesSize;
}DefinedVariablesCaseObj;

typedef struct _VariablesObject
{
    VariableObj * Vars;
    int VarsSize;
} VariablesObj;


typedef struct _HeadReturnObject
{
    int ToState;/*
        0:正常
        1:回傳
        2:重設 Variable
        4:特色變數
    */

    ValuesObj Values;
    DefinedVariablesCaseObj VAVs;
    VariablesObj Vars;

} HeadReturnObj;


typedef struct _VariableRequestUPObject
{
    VariableObj * * VariableUPs;
    int VariablesSize;
} VariableRequestUPObj;


typedef struct _HeadRequestObject
{
    PairObj * Pairs;
    int PairsSize;

    VariableObj * * VariablesUP;
    int * VariablesUPSizeUP;
} HeadRequestObj;

typedef struct _MioneSectionObject
{
    MioneObj HeadAction;

    PairObj * Pairs;
    int PairsSize;
} MioneSectionObj;

typedef struct _MioneToBuildObject
{
    const MioneObj* Objs;
    const int ObjsSize;
} MioneToBuildObj;

typedef struct _MioneBuiltObject
{
    MioneSectionObj * Sections;
    int SectionsSize;
} MioneBuiltObj;

typedef struct _ToImplementObject
{
    MioneBuiltObj Built;
} ToImplementObj;

typedef struct _ImplementedObject
{
    int ToState; /*
    0:正常
    1:回傳 (HEAD回傳的結果)
    2:區域性特色變數
    */

    ValuesObj Values;
    VariablesObj Vars;
}ImplementedObj;

typedef struct _ScopeObject
{
    VariableObj * * VariableUPs;
    int VariableUPsSize;

    struct _ScopeObject * ParentUP;

    struct _ScopeObject ** ChildUPs;
    int ChildUPsSize;
} ScopeObj;


#endif //OBJECTS_H
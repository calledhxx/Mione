//
// Created by chenn on 24-8-3.
//

#include <stdint.h>
#include <wchar.h>

#define HEAD 1
#define PROMPT 2
#define SYMBOL 3
#define VARIABLE  4
#define VALUE 5

#define VALUE_STRING_TYPE 1
#define VALUE_FUNCTION_TYPE 4
#define VALUE_RANGE_TYPE 5
#define VALUE_TABLE_TYPE 6
#define VALUE_LIGHTS_TYPE 7
#define VALUE_DB_TYPE 8

#define VALUE_NUMBER_TYPE 9

#ifndef OBJECTS_H
#define OBJECTS_H

//
//
//數組
//
//
typedef struct _IntegerObject
{
    uint32_t * Units;
    unsigned int UnitsLen;

    unsigned int Digits;
} IntegerObj;

typedef struct _NumberObject
{
    IntegerObj Integer; //採整數個位最前
    IntegerObj Decimal; //採小數個位最前
} NumberObj;
//
//
//
//
//

//HEAD

typedef struct _HeadObject
{
    wchar_t * Name;
    int Identification;
    struct _HeadReturnObject (*Fuc)(struct _HeadCallObject *);
}HeadObj;

//


//PROMPT

typedef struct _PromptObject
{
    wchar_t * Name;
    int Identification; //識別符號
}PromptObj;
//


//SYMBOL

typedef struct _SymbolObject
{
    wchar_t * Name;
    int Identification; //識別符號
    int AfterConnectVV; //後面是否可與VV相連 不會報錯 MIONE
}SymbolObj;
//

//
//
// Area and Table物件 GROUP
//
//
typedef struct _AreaObject
{
    struct _MioneSectionObjectsCarrier * SectionCarrierPointer;
}AreaObj;

typedef struct _TableObject
{
    struct _MioneSectionObjectsCarrier * SectionCarrierPointer;

    struct _VariableObjectPointersCarrier * VariableObjPointerCarrierPointer;
}TableObj;
//
//
//
//
//
//

//
//
//VALUE GROUP
//
//

typedef struct _ValueObject
{
    int ValueType; //值類型
    /*
    1:字串
    4:函數
    5:執行式
    6:表單
    7:開關
    8:布林值
    9:數
    */
    struct _AreaObject Area; //給於函數(function),開關(lights)或者執行式(range)。
    wchar_t * String; //給予文字(string)。



    NumberObj Number; //給予數字。
    struct _TableObject Table; //給予表格(table)。
    int db;//布林
} ValueObj;

typedef struct _ValueObjectCarrier
{
    ValueObj* Carrier;
    int CarrierLen;
} ValueObjCarrier;

//
//
//
//
//




//
//
//VARIABLE GROUP
//
//


typedef struct _VariableObject
{
    wchar_t* VariableName; //變數名稱
    int VariablePlace; //變數位置

    ValueObj Value; //值
} VariableObj;

typedef struct _VariableObjectsCarrier
{
    VariableObj * Carrier;
    unsigned int CarrierLen;
} VariableObjCarrier;

//
//
//
//
//

//
//
//Mione物件 GROUP
//
//
typedef struct _MioneObject
{
    int ObjType; //HPSVV宏 1H 2P 3S 4VAR 5VAL 0換行

    VariableObj * VarUP; //當ObjType為VAR時，會用到此變數。
    ValueObj Val;  //當ObjType為VALUE宏時，會用到此值。

    SymbolObj Symbol; //當ObjType為SYMBOL時，會用到此符號。
    PromptObj Prompt; //當ObjType為PROMPT時，會用到此提示。
    HeadObj Head; //當ObjType為HEAD時，會用到此標題。

    int Line; //行號
    int Column; //列號

    struct _ScopeObject* ScopeUP; //作用域
} MioneObj;

typedef struct _MioneObjectsCarrier
{
    MioneObj * Carrier;
    unsigned int CarrierLen;
} MioneObjCarrier;

//
//
//
//
//

//
//
//CASE物件 GROUP
//
//
typedef struct _CaseObject
{
    int ObjType; /*
    CharType...
    */
    wchar_t * ObjName;
} CaseObj;

typedef struct _CaseObjectsCarrier
{
    CaseObj * Carrier;
    int CarrierLen;
} CaseObjCarrier;
//
//
//
//
//


typedef struct _PairObject
{
    MioneObj Host; //若是開頭組合，則此物件為HEAD本身，反之PROMPT本身。

    MioneObjCarrier SourceCarrier;
}PairObj;

typedef struct _PairObjectsCarrier
{
    unsigned int CarrierLen;
    PairObj * Carrier;
} PairObjCarrier;

//
//
// Mione Section Group
//
//

typedef struct _MioneSectionObject
{
    MioneObj Head;

    PairObjCarrier PairCarrier;
} MioneSectionObj;

typedef struct _MioneSectionObjectsCarrier
{
    MioneSectionObj * Carrier;
    int CarrierLen;
} MioneSectionObjCarrier;

//
//
//
//
//




//
//
// Value and Variable物件 GROUP
//
//

typedef struct _ValueAndVariableObject
{
    ValueObj Value;
    VariableObj* VariablePointer;
}ValueAndVariableObj;

typedef struct _ValueAndVariableObjectsCarrier
{
    ValueAndVariableObj * Carrier;
    int CarrierLen;
}ValueAndVariableObjCarrier;

//
//
//
//
//


typedef struct _VariableObjectPointersCarrier
{
    VariableObj ** Carrier;
    unsigned int CarrierLen;
} VariableObjPtrCarrier;


//
// HEAD 函數交換物件
//

typedef struct _HeadReturnObject
{
    int ToState;/*
        0b00000000:無行動

        0b00000001:回傳值
        0b00000010:重設Variable
        0b00000100:特色變數
    */

    ValueObjCarrier ValueCarrier;
    ValueAndVariableObjCarrier ValueAndVariableCarrier;
    VariableObjCarrier VariableCarrier;

} HeadReturnObj;

typedef struct _HeadCallObject
{
    PairObjCarrier PairCarrier; //程式句二大組合

    ValueObjCarrier CallByValueCarrier; //傳入的值

    VariableObjPtrCarrier VariablePtrCarrier; //以上本作用域特色變數
} HeadCallObj;

//
//
//


typedef struct _ErrorObject
{
    wchar_t * Message;
    wchar_t * Code;

    MioneObjCarrier ErrorObjectCarrier;
} ErrorObj;

typedef struct _EventObject
{
    int ToState; /*
        0b00000000:執行回報

        0b00000001:錯誤
    */

    ErrorObj Error;

} EventObj;

//
//Mione 運行交換物件
//
typedef struct _ToImplementObject
{
    MioneSectionObjCarrier Built;
    ValueObjCarrier CallByValueCarrier;
} ToImplementObj;

typedef struct _ImplementedObject
{
    int ToState; /*
        0b00000000:無行動

        0b00000001:回傳值
        0b00000010:區域性特色變數
        0b00000100:事件回傳
    */

    ValueObjCarrier ValueCarrier;
    VariableObjCarrier VariableCarrier;
    EventObj Event;
}ImplementedObj;
//
//
//

typedef struct _ScopeObject
{
    VariableObjPtrCarrier VariablePtrCarrier;

    struct _ScopeObject * ParentScopePointer;
    struct _ScopeObjectCarrier * ChildrenScopeCarrierPointer;
} ScopeObj;

typedef struct _ScopeObjectCarrier
{
    ScopeObj * Carrier;
    unsigned int CarrierLen;
} ScopeObjCarrier;


//
// CMO函數特殊物件
//
typedef struct _ToCMObject
{
    CaseObjCarrier CaseCarrier;

    int LineADD;
    int ColumnADD;
} ToCMObj;

typedef struct _ToReplaceValueForCMOObject
{
    ToCMObj a;
    int ObjIndex;
    int ValueType ;
} ToReplaceValueForCMOObj;
//
//
//




#endif //OBJECTS_H
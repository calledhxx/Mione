#ifndef OBJECTS_H
#define OBJECTS_H

enum
{
    HEAD = 1,
    PROMPT = 2,
    SYMBOL = 3,
    VARIABLE = 4,
    VALUE = 5,
};

enum
{
    CT_NULL = 0,
    CT_NORMAL = 1,
    CT_NUMBER = 2,
    CT_DQ = 3,
    CT_SQ = 4,
    CT_SHARP = 5,
    CT_CONNECTABLE = 9,
    CT_UNCONNECTABLE = 10,
    CT_SPACE = 11,
    CT_BS = 12,
    CT_NEWLINE = 13,
    CT_SEMICOLON = 14
};

enum
{
    CASE_NORMAL,
    CASE_SINGLE_STRING,
    CASE_DOUBLE_STRING,
    CASE_DECNUMBER,
    CASE_HEXNUMBER,
    CASE_BINNUMBER,
    CASE_CONNECTABLE,
    CASE_UNCONNECTABLE,
    CASE_PHRASE,
    CASE_BREAKER,
};

enum
{
    VALUE_STRING_TYPE = 1,
    VALUE_FUNCTION_TYPE = 4,
    VALUE_RANGE_TYPE = 5,
    VALUE_TABLE_TYPE = 6,
    VALUE_LIGHTS_TYPE = 7,
    VALUE_DB_TYPE = 8,
    VALUE_NUMBER_TYPE = 9,
};


typedef struct _CasePositionObject
{
    unsigned int CaseStartLine; //Case開始行號
    unsigned int CaseStartColumn; //Case開始列號

    unsigned int CaseEndLine; //Case結束行號
    unsigned int CaseEndColumn; //Case結束列號
} CasePositionObj;

typedef struct _CaseObject
{
    CasePositionObj CasePosition;

    char ObjType;
    char * ObjName;
} CaseObj;

typedef struct _CaseObjectsCarrier
{
    CaseObj * Carrier;
    unsigned int CarrierLen;
} CaseObjCarrier;


typedef struct _HeadObject
{
    char * Name;
    unsigned Identification;
    struct _EventObject (*Fuc)(struct _HeadCallObject *);
}HeadObj;


typedef struct _PromptObject
{
    char * Name;
    unsigned Identification; //識別符號
}PromptObj;

enum{
    SC_BeforeSymbol = 1,
    SC_AfterSymbol = 2,
    //符合其中之一就連接

    SC_BeforeVariableOrValue = 4,
    SC_AfterVariableOrValue = 8,
};

typedef struct _SymbolObject
{
    char * Name;
    unsigned Identification; //識別符號
    unsigned SymbolCarry;
}SymbolObj;

typedef struct _AreaObject
{
    struct _TrainObjectsCarrier * TrainObjCarrierPointer;
}AreaObj;

typedef struct _TableObject
{
    struct _TrainObjectsCarrier * TrainObjCarrierPointer;

    struct _VariableObjectPointersCarrier * VariableObjPointerCarrierPointer;
}TableObj;

typedef struct _ValueObject
{
    unsigned ValueType; //值類型
    struct _AreaObject Area; //給於函數(function),開關(lights)或者執行式(range)。
    char * String; //給予文字(string)。
    double Number; //給予數字。
    struct _TableObject Table; //給予表格(table)。
    char db;//布林
} ValueObj;

typedef struct _ValueObjectCarrier
{
    ValueObj* Carrier;
    int CarrierLen;
} ValueObjCarrier;


typedef struct _VariableObject
{
    char* VariableName; //變數名稱
    int VariablePlace; //變數位置

    ValueObj Value; //值
} VariableObj;

typedef struct _VariableObjectsCarrier
{
    VariableObj * Carrier;
    unsigned int CarrierLen;
} VariableObjCarrier;

typedef struct _MioneObject
{
    unsigned ObjType; //HPSVV 1H 2P 3S 4VAR 5VAL

    VariableObj ** PointerOfScopeVariablePtr; //當ObjType為VAR時，會用到此變數。
    ValueObj Value;  //當ObjType為VALUE宏時，會用到此值。

    SymbolObj Symbol; //當ObjType為SYMBOL時，會用到此符號。
    PromptObj Prompt; //當ObjType為PROMPT時，會用到此提示。
    HeadObj Head; //當ObjType為HEAD時，會用到此標題。

    CasePositionObj MioneObjectPosition;
} MioneObj;

typedef struct _MioneObjectsCarrier
{
    MioneObj * Carrier;
    unsigned int CarrierLen;
} MioneObjCarrier;

typedef struct _CarriageObject
{
    MioneObj CarriageManager;
    MioneObjCarrier CarriagePassengers;
}CarriageObj;

typedef struct _TrainObject
{
    unsigned int CarriageLen;
    CarriageObj * Carriages;
} TrainObj;

typedef struct _TrainObjectsCarrier
{
    unsigned int CarrierLen;
    TrainObj * Carrier;
} TrainObjCarrier;

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

typedef struct _VariableObjectPointersCarrier
{
    VariableObj ** Carrier;
    unsigned int CarrierLen;
} VariableObjPtrCarrier;

typedef struct _PointersOfVariableObjectPointersCarrier
{
    VariableObj *** Carrier;
    unsigned int CarrierLen;
} PtrVariableObjPtrCarrier;

typedef struct _ErrorObject
{
    char * Message;
    char * Code;
    CasePositionObj ErrorPosition;

} ErrorObj;

enum
{
    EVENT_NONE = 0,
    EVENT_ERROR = 1,
    EVENT_RETURN_VALUES = 2,
    EVENT_RESET_VARIABLE_TO_VALUE = 4,
    EVENT_MAJOR_VARIABLE = 8,
    EVENT_FCO_RETURN = 16,
    EVENT_CMO_RETURN = 32,
    EVENT_TO_MIONE_RETURN = 64,
};

typedef struct _EventObject
{
    unsigned ToState; /*
        無
        錯誤
        回傳值
        重設Variable
        特色變數

        FCO函數回傳
        CMO函數回傳
        ToMione函數回傳
    */

    ErrorObj Error;

    ValueObjCarrier ReturnValues;
    ValueAndVariableObjCarrier ResetVariablesToValues;
    VariableObjCarrier MajorVariables;

    CaseObjCarrier CaseCarrier;
    MioneObjCarrier MioneCarrier;
    TrainObjCarrier TrainCarrier;

} EventObj;

typedef struct _HeadCallObject
{
    TrainObj Train;

    ValueObjCarrier CallByValueCarrier; //傳入的值

    VariableObjPtrCarrier VariablePtrCarrier; //以上本作用域特色變數
} HeadCallObj;

typedef struct _ToImplementObject
{
    TrainObjCarrier Built;
    ValueObjCarrier CallByValueCarrier;
} ToImplementObj;

typedef struct _ScopeObject
{
    PtrVariableObjPtrCarrier PtrOfVariablePtrCarrier;

    struct _ScopeObject * ParentScopePointer;
    struct _ScopeObjectCarrier * ChildrenScopeCarrierPointer;
} ScopeObj;

typedef struct _ScopeObjectCarrier
{
    ScopeObj * Carrier;
    unsigned int CarrierLen;
} ScopeObjCarrier;

#endif //OBJECTS_H
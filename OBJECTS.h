#ifndef OBJECTS_H
#define OBJECTS_H
#include <stdio.h>
#include <windows.h>

enum
{
    HEAD = 1,
    PROMPT = 2,
    SYMBOL = 3,
    VARIABLE = 4,
    VALUE = 5,
    WELD = 6,
};

enum
{
    CT_NULL = 0,
    CT_NORMAL = 1,
    CT_NUMBER = 2,
    CT_DQ = 3,
    CT_SQ = 4,
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
    CASE_BREAKER,
};

enum{ //含疊加態
    SC_BeforeSymbol = 1,
    SC_AfterSymbol = 2,
    //符合其中之一就連接

    SC_BeforeVariableOrValue = 4,
    SC_AfterVariableOrValue = 8,
};

enum
{
    EVENT_NONE,
    EVENT_NO_FILE_FOUND,
    EVENT_SUPER_CHAR_ERROR,
    EVENT_MO_ERROR, //Mione Object Error
    EVENT_LAYOUT_ERROR,
    EVENT_COMPUTATION_ERROR,
    EVENT_HEAD_SUFFIX_ERROR,
    EVENT_PROMPT_SUFFIX_ERROR,
    EVENT_TRAIN_ERROR,
};

enum //HEAD
{
    HEAD_SET = 1,
    HEAD_RETURN = 2,
    HEAD_GET = 3,
    HEAD_LIBRARY = 4,
    HEAD_ENTER = 5,
    HEAD_INCLUDE = 6,
    HEAD_IF = 7,
    HEAD_FOR = 8,
    HEAD_WHILE = 9,
};

enum //PROMPT
{
    PROMPT_SET = 1,
    PROMPT_MODEL = 2,
    PROMPT_REQUEST = 3,
    PROMPT_POSITION = 4,
    PROMPT_RESPOND = 5,
    PROMPT_FROM = 6,
    PROMPT_OF = 7,
    PROMPT_AS = 8,
    PROMPT_THEN = 9,
    PROMPT_ELSE = 10,
    PROMPT_DO = 11,
};

enum //SYMBOL
{
    SYMBOL_ADD = 1,
    SYMBOL_COMMA = 2,
    SYMBOL_STAR = 3,
    SYMBOL_SLASH = 4,
    SYMBOL_UP = 5,
    SYMBOL_SUB = 6,
    SYMBOL_DOT = 7,
    SYMBOL_IS = 8,
    SYMBOL_FRONT_BRACKET = 9,
    SYMBOL_BACK_BRACKET = 10,
    SYMBOL_FRONT_PARENTHESES = 11,
    SYMBOL_BACK_PARENTHESES = 12,
    SYMBOL_GREATER_THAN = 13,
    SYMBOL_LESS_THAN = 14,
};

enum //VALUE TYPE
{
    VALUE_STRING_TYPE = 1,
    VALUE_FUNCTION_TYPE = 4,
    VALUE_RANGE_TYPE = 5,
    VALUE_TABLE_TYPE = 6,
    VALUE_DB_TYPE = 8,
    VALUE_NUMBER_TYPE = 9,

    VALUE_WINDOWS_LIBRARY_TYPE = 10,
};

enum //WELD
{
    WELD_SUBJECT = 1
};

typedef struct block_s
{
    unsigned size;
    struct block_s * next;
    struct block_s * current;
} block_t;

typedef char unit_t;

typedef struct line_s
{
    unit_t * units;
    unit_t * originalPointerOfUnits;
    block_t * leaderPointer;
} line_t;

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
    struct _HeadFunctionRespondObject (*Fuc)(const struct _HeadFunctionRequestObject *);
}HeadObj;

typedef struct _HeadObjectCarrier
{
    HeadObj * Carrier;
    unsigned CarrierLen;
} HeadObjCarrier;

typedef struct _PromptObject
{
    char * Name;
    unsigned Identification; //識別符號
}PromptObj;

typedef struct _PromptObjectCarrier
{
    PromptObj * Carrier;
    unsigned CarrierLen;
} PromptObjCarrier;

typedef struct _SymbolObject
{
    char * Name;
    unsigned Identification; //識別符號
    unsigned SymbolCarry;
}SymbolObj;

typedef struct _SymbolObjectCarrier
{
    SymbolObj * Carrier;
    unsigned CarrierLen;
} SymbolObjCarrier;

typedef struct _AreaObject
{
    struct _TrainObjectsCarrier * TrainObjCarrierPointer;
}AreaObj;

typedef struct _TableObject
{
    struct _TrainObjectsCarrier * TrainObjCarrierPointer;

    struct _VariableObjectsCarrier * VariableObjCarrierPointer;
}TableObj;

typedef struct _ValueObject
{
    unsigned ValueType; //值類型
    struct _AreaObject Area; //給於函數(function),開關(lights)或者執行式(range)。
    char * String; //給予文字(string)。
    double Number; //給予數字。
    struct _TableObject Table; //給予表格(table)。
    char db;//布林

    HMODULE WindowsLibrary; //給於外部函式庫
} ValueObj;

typedef struct _ValueObjectCarrier
{
    ValueObj* Carrier;
    unsigned CarrierLen;
} ValueObjCarrier;


typedef struct _VariableObject
{
    char* VariableName; //變數名稱
    unsigned VariablePlace; //變數位置

    ValueObj Value; //值
} VariableObj;

typedef struct _VariableLinkObject
{
    struct _VariableLinkObject * PointToAnotherLink;
    VariableObj * VariablePtr;
} VariableLinkObj;

typedef struct _VariableLinkObjectCarrier
{
    VariableLinkObj * Carrier;
    unsigned int CarrierLen;
} VariableLinkObjCarrier;

typedef struct _VariableLinkPointersObjectCarrier
{
    VariableLinkObj * * Carrier;
    unsigned int CarrierLen;
} VariableLinkPtrObjCarrier;

typedef struct _VariableObjectsCarrier
{
    VariableObj * Carrier;
    unsigned int CarrierLen;
} VariableObjCarrier;

typedef struct _WeldObject
{
    char * Name;
    unsigned Identification;
    struct _WeldFunctionRespondObject (*Fuc)(struct _WeldFunctionRequestObject *);
} WeldObj;

typedef struct _WeldObjectsCarrier
{
    WeldObj* Carrier;
    unsigned CarrierLen;
} WeldObjCarrier;

typedef struct _MioneObject
{
    unsigned char ObjType; //HPSVV 1H 2P 3S 4VAR 5VAL 6W

    VariableLinkObj * VariableLinkPtr; //當ObjType為VAR時，會用到此變數。
    ValueObj Value;  //當ObjType為VALUE宏時，會用到此值。

    SymbolObj Symbol; //當ObjType為SYMBOL時，會用到該值。
    PromptObj Prompt; //當ObjType為PROMPT時，會用到該值。
    HeadObj Head; //當ObjType為HEAD時，會用到該值。
    WeldObj Weld; //當ObjType為WELD時，會用到該值。

    CasePositionObj MioneObjectPosition;
    struct _ScopeObject * PointerOfScope; //此物件所屬的作用域
} MioneObj;

typedef struct _MioneObjectsCarrier
{
    MioneObj * Carrier;
    unsigned int CarrierLen;
} MioneObjCarrier;

typedef struct _MioneLayoutObject
{
    MioneObjCarrier MioneObjectsCarrier;
    unsigned LayoutHandler;
} MioneLayoutObj;

typedef struct _MioneLayoutObjectsCarrier
{
    MioneLayoutObj * Carrier;
    unsigned CarrierLen;
} MioneLayoutObjCarrier;

typedef struct _PassengerObject
{
    MioneObj Direct;
    MioneObjCarrier Indirect;
    char IsIndirect;
} PassengerObj;

typedef struct _PassengerObjectsCarrier
{
    PassengerObj * Carrier;
    unsigned CarrierLen;
} PassengerObjCarrier;

typedef struct _CarriageObject
{
    MioneObj CarriageManager;
    PassengerObjCarrier CarriagePassengersCarrier;
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

typedef struct _EventObject
{
    char * Address;
    char * Message;
    unsigned Code;
    CasePositionObj EventPosition;
} EventObj;

typedef struct _FILE_TO_CASE_ObjectFunctionRespondObject
{
    CaseObjCarrier CaseCarrier;
    EventObj Event;
} FCOFunctionRespondObj;

typedef struct _FILE_TO_CASE_ObjectFunctionRequestObject
{
    FILE * f;
    const EventObj EventTemplate;
} FCOFunctionRequestObj;

typedef struct _CASE_TO_MIONE_ObjectFunctionRespondObject
{
    MioneObjCarrier MioneCarrier;
    EventObj Event;
} CMOFunctionRespondObj;

typedef struct _CASE_TO_MIONE_ObjectFunctionRequestObject
{
    CaseObjCarrier CassCarrier;
    struct _ScopeObject * ScopePointer;
    const EventObj EventTemplate;
} CMOFunctionRequestObj;

typedef struct _MIONEFunctionRespondObject
{
    TrainObjCarrier TrainCarrier;
    EventObj Event;
} MIONEFunctionRespondObj;

typedef struct _MIONEFunctionRequestObject
{
    MioneObjCarrier MioneCarrier;
    const EventObj EventTemplate;
} MIONEFunctionRequestObj;

typedef struct _IMPLEMENTFunctionRespondObject
{
    ValueObjCarrier ReturnValues;
    ValueAndVariableObjCarrier ResetVariablesToValues;
    VariableObjCarrier MajorVariables;
    VariableObjPtrCarrier Subjects;

    EventObj Event;
} IMPLEMENTFunctionRespondObj;

typedef struct _IMPLEMENTFunctionRequestObject
{
    TrainObjCarrier Built;
    ValueObjCarrier CallByValueCarrier;
    const EventObj EventTemplate;
} IMPLEMENTFunctionRequestObj;

typedef struct _HeadFunctionRespondObject
{
    ValueObjCarrier ReturnValues;
    VariableObjCarrier MajorVariables;
    ValueAndVariableObjCarrier ResetVariablesToValues;
    VariableObjPtrCarrier Subjects;
    EventObj Event;
} HeadFunctionRespondObj;

typedef struct _HeadFunctionRequestObject
{
    TrainObj Train;

    ValueObjCarrier CallByValueCarrier; //傳入的值

    VariableObjPtrCarrier VariablePtrCarrier; //以上本作用域特色變數

    const EventObj EventTemplate;
} HeadFunctionRequestObj;

typedef struct _ScopeObject
{
    VariableLinkPtrObjCarrier VariableLinkPtrCarrier;

    struct _ScopeObject * ParentScopePointer;
    struct _ScopeObjectPointerCarrier * ChildrenScopePtrCarrierPointer;
} ScopeObj;

typedef struct _ScopeObjectPointerCarrier
{
    ScopeObj * * Carrier;
    unsigned int CarrierLen;
} ScopeObjPtrCarrier;

typedef struct _ExternalLibraryRespondObject
{
    ValueObjCarrier ValueCarrier;
    EventObj Event;
} ExternalLibraryRespondObj;

typedef struct _ExternalLibraryRequestObject
{
    ValueObjCarrier ValueCarrier;
    EventObj EventTemplate;
} ExternalLibraryRequestObj;

typedef struct _CONTAINERRequestObject
{
    MioneObjCarrier MioneCarrier;
    EventObj EventTemplate;
} CONTAINERRequestObj;

typedef struct _CONTAINERRespondObject
{
    VariableObjPtrCarrier VariablePtrCarrier;
    EventObj Event;

} CONTAINERRespondObj;

typedef struct _RESOURCERequestObject
{
    MioneObjCarrier MioneCarrier;
    EventObj EventTemplate;
} RESOURCERequestObj;

typedef struct _RESOURCERespondObject
{
    ValueObjCarrier ValueCarrier;
    EventObj Event;
} RESOURCERespondObj;

typedef struct _COMPUTATIONRequestObject
{
    MioneObjCarrier MioneCarrier;
    EventObj EventTemplate;
} COMPUTATIONRequestObj;

typedef struct _COMPUTATIONRespondObject
{
    MioneObjCarrier MioneCarrier;
    EventObj Event;
} COMPUTATIONRespondObj;

typedef struct  _WeldFunctionRespondObject
{
    MioneObjCarrier MioneCarrier;
    EventObj Event;
} WeldFunctionRespondObj;

typedef struct  _WeldFunctionRequestObject
{
    MioneObjCarrier MioneCarrier;
    EventObj EventTemplate;
} WeldFunctionRequestObj;

typedef struct _PASSENGERRequestObject
{
    EventObj EventTemplate;
    PassengerObjCarrier PassenegrsCarrier;
} PASSENGERRequestObj;

typedef struct _PASSENGERRespondObject
{
    EventObj Event;
    MioneObjCarrier MioneCarrier;
} PASSENGERRespondObj;

#endif //OBJECTS_H
#ifndef OBJECT_H
#define OBJECT_H

enum object_type_e
{
    OBJECT_NONE ,
    OBJECT_HEAD ,
    OBJECT_PROMPT,
    OBJECT_SYMBOL,
    OBJECT_VARIABLE,
    OBJECT_VALUE,
    OBJECT_WELD,
};

enum token_e
{
    TOKEN_NONE,

    TOKEN_HEAD_SET,
    TOKEN_HEAD_IF,
    TOKEN_HEAD_WHILE,
    TOKEN_HEAD_LIBRARY,
    TOKEN_HEAD_INCLUDE,

    TOKEN_PROMPT_EQUAL,

    TOKEN_SYMBOL_PLUS,
    TOKEN_SYMBOL_MINUS,
    TOKEN_SYMBOL_OPENING_PARENTHESIS,
    TOKEN_SYMBOL_CLOSING_PARENTHESIS,

    TOKEN_WELD_SUBJECT
};

typedef struct object_s
{
    enum object_type_e object_type;

    enum token_e token; //Only use in HEAD ,PROMPT ,SYMBOL, WELD

    union
    {
        variable_t variable;
        value_t value;
    } vv;
} object_t;

typedef struct object_carrier_s
{
    object_t * objects;
    unsigned objects_length;
} object_carrier_t;

enum layout_handler_e
{
    LAYOUT_HANDLER_NONE,
    LAYOUT_HANDLER_FUNCTION,
    LAYOUT_HANDLER_RANGE,
    LAYOUT_HANDLER_TABLE,
};

typedef struct scope_s
{
    struct scope_carrier_s * child_scope_carrier_ptr;
    struct scope_s * parent_scope_ptr;
    variable_link_ptr_carrier_t variable_link_ptr_carrier;
} scope_t;

typedef struct scope_carrier_s
{
    scope_t* scopes;
    unsigned scopes_length;
} scope_carrier_t;

typedef struct layout_s
{
    object_carrier_t object_carrier;
    enum layout_handler_e layout_handler;
} layout_t;

typedef struct layout_carrier_s
{
    layout_t * layouts;
    unsigned layouts_length;
} layout_carrier_t;

extern object_carrier_t word_to_object(word_carrier_t word_carrier);

#endif

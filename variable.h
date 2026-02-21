#ifndef VARIABLE_H
#define VARIABLE_H

#include "object.h"
#include "value.h"

enum variable_link_type_e
{
    VARIABLE_LINK_NONE,
    VARIABLE_LINK_LEADER,
    VARIABLE_LINK_COLEADER,
    VARIABLE_LINK_FOLLOWER,
};

typedef struct variable_link_s
{
    enum variable_link_type_e variable_link_type;
    struct variable_link_s * toward_variable_link_ptr;
    struct variable_s * toward_variable_ptr;
} variable_link_t;

typedef struct variable_link_carrier_s
{
    variable_link_t * variable_links;
    unsigned variable_links_length;
} variable_link_carrier_t;


typedef struct genuine_variable_s
{
    char* name;
    value_t value;
} genuine_variable_t;

typedef struct dummy_variable_s
{
    variable_link_t variable_link;
} dummy_variable_t;

typedef struct variable_s
{
    union
    {
        genuine_variable_t genuine_variable;
        dummy_variable_t dummy_variable;
    } variable;
    char is_dummy;
} variable_t;

typedef struct variable_carrier_s
{
    variable_t variables;
    unsigned variables_length;
} variable_carrier_t;

extern variable_link_t find_variable_in_scope(scope_t scope,const char* name,unsigned * scope_depth_butter_ptr);

#endif

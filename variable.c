#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "object.h"
#include "variable.h"

variable_t * variable_chaser(variable_link_t variable_link)
{
    while (1)
    {
        switch (variable_link.variable_link_type)
        {
        case VARIABLE_LINK_LEADER:
                return variable_link.toward_variable_ptr;
        case VARIABLE_LINK_FOLLOWER:
        case VARIABLE_LINK_COLEADER:
            {
                if (variable_link.toward_variable_link_ptr)
                    variable_link = *variable_link.toward_variable_link_ptr;
                else
                    return 0;
                break;
            }
        default:
            return 0;
        }
    }
}

variable_link_t find_variable_in_scope(scope_t scope,const char* name,unsigned * const scope_depth_butter_ptr)
{
    variable_link_t result = {0};

    *scope_depth_butter_ptr = 0;

    while (scope.parent_scope_ptr){
        for (unsigned i = 0; i < scope.variable_link_carrier.variable_links_length; i++)
            if (scope.variable_link_carrier.variable_links[i].variable_link_type != VARIABLE_LINK_NONE)
            {
                variable_t const * const variable_ptr  = variable_chaser(scope.variable_link_carrier.variable_links[i]);

                if (variable_ptr->is_dummy) //這東西要是能被觸發 那天就塌了
                    exit(124);

                if (strcmp(name,variable_ptr->variable.genuine_variable.name) == 0)
                {
                    result = scope.variable_link_carrier.variable_links[i];
                    goto end;
                }
            }

        (*scope_depth_butter_ptr)++;

        scope = *scope.parent_scope_ptr;
    };

    end:

    return result;
}

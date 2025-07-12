#include "../OBJECTS.h"

#include "SET.h"
#include "RETURN.h"
#include "IF.h"
#include "FOR.h"
#include "SYS.h"
#include "GET.h"


HeadObj Heads[]  = {
    (HeadObj){
        .Fuc = SET,
        .Name = L"set",
        .Identification = 1,
    },
    (HeadObj){
        .Fuc = RETURN,
        .Name = L"return",
        .Identification = 2
    },
    (HeadObj){
        .Fuc = IF,
        .Name = L"if",
        .Identification = 3
    },
    (HeadObj){
        .Fuc = FOR,
        .Name = L"for",
        .Identification = 4
    },
    (HeadObj){
        .Fuc = SYS,
        .Name = L"sys",
        .Identification = 5
    },
    (HeadObj){
        .Fuc = GET,
        .Name = L"get",
        .Identification = 6
    },

    (HeadObj){
        .Identification = -1
    }
};
#include <stdlib.h>

#include "OBJECTS.h"

SymbolObj Symbols[] =
{
    (SymbolObj){
        .Name = "+",
        .SymbolType = 1,
        .xIndex = 2,
        .CurNumber = 1,
        
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = ",",
        .SymbolType = 1,
        .xIndex = 0, // 1,2,3,5,8,13
        .CurNumber = 2,
        
        .AfterConnectVV = 1,
       

    },

    (SymbolObj){
        .Name = "*",
        .SymbolType = 1,
        .xIndex = 1,
        .CurNumber = 3,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = "/",
        .SymbolType = 1,
        .xIndex = 1,
        .CurNumber = 4,
        .AfterConnectVV = 1,
       
    },



    (SymbolObj){
        .Name = "^",
        .SymbolType = 1,
        .xIndex = 0,
        .CurNumber = 5,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = "-",
        .SymbolType = 3,
        .xIndex = 1,// -1
        .yIndex = 2,// 1-1
        .CurNumber = 6,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = ".",
        .SymbolType = 3,
        .xIndex = 0,// .1
        .yIndex = 0,// 0.1 or <TABLE>.<CHILD>
        .CurNumber = 7,
        .AfterConnectVV = 1,
       
    },
    (SymbolObj){
        .Name = "==",
        .SymbolType = 1,
        .xIndex = 2,
        .CurNumber = 8,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = "[",
        .SymbolType = 0,
        .xIndex = 0, //這邊的設定有點無效 只是標住他的確與階級0同樣層級執行
        .CurNumber = 10,

        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = "]",
        .SymbolType = 0,
        .xIndex = 0, //這邊的設定有點無效 只是標住他的確與階級0同樣層級執行
        .CurNumber = 11,

        .AfterConnectVV = 0,
       
    },
    (SymbolObj){
        .Name = "(",
        .SymbolType = 0,
        .xIndex = 0, //這邊的設定有點無效 只是標住他的確與階級0同樣層級執行
        .CurNumber = 12,

        .AfterConnectVV = 1,
       
    },

    (SymbolObj){
        .Name = ")",
        .SymbolType = 0,
        .xIndex = 0, //這邊的設定有點無效 只是標住他的確與階級0同樣層級執行
        .CurNumber = 13,

        .AfterConnectVV = 0,
       
    },
    (SymbolObj){
        .Name = "abreast",
        .SymbolType = 2,
        .xIndex = 1,
        .CurNumber = 15,
        .AfterConnectVV = 1,
       
    },

    (SymbolObj){ //衛兵值
        .CurNumber = -1
    }
};
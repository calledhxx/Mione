#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

static const char * const NormalKeyword[] = {
    "function",
    "range",
    "end",
    "true",
    "false"
};

static const char * const UnconnectableKeyword[] = {
    "{",
    "}",
};

static layout_t popLayoutFromLayoutCarrier(layout_carrier_t * LayoutCarrierPtr)
{
    if (1>= LayoutCarrierPtr->layouts_length) //這是神經病吧？
        exit(-121);
    const layout_t Layout = LayoutCarrierPtr->layouts[LayoutCarrierPtr->layouts_length - 1];
    LayoutCarrierPtr->layouts_length--;
    LayoutCarrierPtr->layouts = realloc(
        LayoutCarrierPtr->layouts,
        sizeof(layout_t) * LayoutCarrierPtr->layouts_length
        );

    return Layout;
}

static void pushLayoutIntoLayoutCarrier(layout_carrier_t * LayoutCarrierPtr,const layout_t NewLayout)
{
    LayoutCarrierPtr->layouts_length++;
    LayoutCarrierPtr->layouts = realloc(
        LayoutCarrierPtr->layouts,
        LayoutCarrierPtr->layouts_length * sizeof(layout_t)
        );
    LayoutCarrierPtr->layouts[LayoutCarrierPtr->layouts_length - 1] = NewLayout;
}

static void pushMioneObjectIntoLayout(layout_t * LayoutPtr,const object_t Object)
{
    LayoutPtr->object_carrier.objects_length++;
    LayoutPtr->object_carrier.objects = realloc(
        LayoutPtr->object_carrier.objects,
        LayoutPtr->object_carrier.objects_length * sizeof(object_t)
        );
    LayoutPtr->object_carrier.objects[LayoutPtr->object_carrier.objects_length - 1] = Object;
}

object_carrier_t word_to_object(
    const word_carrier_t word_carrier
    )
{
    scope_t * current_scope_ptr = malloc(sizeof(scope_t));
    *current_scope_ptr = (scope_t){0};
    current_scope_ptr->child_scope_carrier_ptr = malloc(sizeof(scope_carrier_t));
    *current_scope_ptr->child_scope_carrier_ptr = (scope_carrier_t){0};

    layout_carrier_t layout_carrier = {0}; //沒有父級關西，所以不用俄羅斯娃娃法


    pushLayoutIntoLayoutCarrier(&layout_carrier,(layout_t){
        .layout_handler = LAYOUT_HANDLER_NONE,
    });

    for (
        unsigned int word_index = 0;
        word_index < word_carrier.words_length;
        word_index++
        )
    {
        char Paired = 0; //是否已經配對過了。

        const word_t ThisWord = word_carrier.words[word_index];

        switch (ThisWord.word_type) // 檢查是否匹配 BREAKER, HEAD ,PROMPT,WELD與SYMBOL，否則進行第二處理。
        {
        case WORD_BREAKER: //新增Breaker
            {
                pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1],(object_t){0});

                Paired = 1;

                break;
            }
        case WORD_NORMAL: //一般，該藍處裡 Head ,Weld 與其他VALUE關鍵字
            {
                for (
                  unsigned int keywordDetectIndex = 0;
                  keywordDetectIndex < (unsigned)(sizeof(NormalKeyword)/sizeof(char*));
                  keywordDetectIndex++
                  )
                    if (strcmp(ThisWord.word, NormalKeyword[keywordDetectIndex])==0)
                    {
                        Paired = 1;

                        switch (keywordDetectIndex)
                        {
                        case 0: //function
                            {
                                pushLayoutIntoLayoutCarrier(
                                    &layout_carrier,
                                    (layout_t){
                                        .object_carrier = (object_carrier_t){
                                            0
                                        },
                                        .layout_handler = LAYOUT_HANDLER_FUNCTION
                                    }
                                    );


                                current_scope_ptr->child_scope_carrier_ptr->scopes_length++;
                                current_scope_ptr->child_scope_carrier_ptr->scopes = realloc(
                                    current_scope_ptr->child_scope_carrier_ptr->scopes,
                                    sizeof(scope_t) * current_scope_ptr->child_scope_carrier_ptr->scopes_length
                                    );
                                current_scope_ptr->child_scope_carrier_ptr->scopes[
                                     current_scope_ptr->child_scope_carrier_ptr->scopes_length - 1
                                    ] = (scope_t){
                                        .parent_scope_ptr = current_scope_ptr,
                                        .child_scope_carrier_ptr = malloc(sizeof(scope_carrier_t)),
                                        .variable_link_ptr_carrier = (variable_link_ptr_carrier_t){0}
                                    };
                                *current_scope_ptr->child_scope_carrier_ptr->scopes[
                                    current_scope_ptr->child_scope_carrier_ptr->scopes_length - 1
                                ].child_scope_carrier_ptr = (scope_carrier_t){0};

                                current_scope_ptr = &current_scope_ptr->child_scope_carrier_ptr->scopes[
                                     current_scope_ptr->child_scope_carrier_ptr->scopes_length - 1
                                    ];

                                Paired = 1;

                                break;
                            }

                        case 1: //range
                            {
                                pushLayoutIntoLayoutCarrier(
                                    &layout_carrier,
                                    (layout_t){
                                        .object_carrier = (object_carrier_t){
                                            0
                                        },
                                        .layout_handler = LAYOUT_HANDLER_RANGE
                                    }
                                    );

                                current_scope_ptr->child_scope_carrier_ptr->scopes_length++;
                                current_scope_ptr->child_scope_carrier_ptr->scopes = realloc(
                                    current_scope_ptr->child_scope_carrier_ptr->scopes,
                                    sizeof(scope_t) * current_scope_ptr->child_scope_carrier_ptr->scopes_length
                                    );
                                current_scope_ptr->child_scope_carrier_ptr->scopes[
                                     current_scope_ptr->child_scope_carrier_ptr->scopes_length - 1
                                    ] = (scope_t){
                                        .parent_scope_ptr = current_scope_ptr,
                                        .child_scope_carrier_ptr = malloc(sizeof(scope_carrier_t)),
                                        .variable_link_ptr_carrier = (variable_link_ptr_carrier_t){0}
                                    };
                                *current_scope_ptr->child_scope_carrier_ptr->scopes[
                                    current_scope_ptr->child_scope_carrier_ptr->scopes_length - 1
                                ].child_scope_carrier_ptr = (scope_carrier_t){0};

                                current_scope_ptr = &current_scope_ptr->child_scope_carrier_ptr->scopes[
                                     current_scope_ptr->child_scope_carrier_ptr->scopes_length - 1
                                    ];

                                Paired = 1;

                                break;
                            }
                        case 2: //end
                            {
                                const layout_t LastLayout = popLayoutFromLayoutCarrier(&layout_carrier);

                                if (LastLayout.layout_handler != LAYOUT_HANDLER_FUNCTION && LastLayout.layout_handler != LAYOUT_HANDLER_RANGE)
                                {
                                    exit(1);
                                    goto end;
                                }

                                current_scope_ptr = current_scope_ptr->parent_scope_ptr;

                                train_carrier_t train_carrier = object_to_train(LastLayout.object_carrier);

                                pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1],(object_t){
                                    .object_type = OBJECT_VALUE,
                                    .vv.value = (value_t){
                                        .value_type = LastLayout.layout_handler == LAYOUT_HANDLER_FUNCTION ? VALUE_FUNCTION : VALUE_RANGE,
                                        .value.train_carrier_ptr = memcpy(malloc(sizeof(train_carrier_t)),&train_carrier,sizeof(train_carrier_t))
                                    }
                                });

                                Paired = 1;

                                break;
                            }
                        case 3: //true
                            {
                                pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1],(object_t){
                                    .object_type = OBJECT_VALUE,
                                    .vv.value = (value_t){
                                        .value_type = VALUE_BOOL,
                                        .value.boolean = 1
                                    }
                                });

                                Paired = 1;

                                break;
                            }
                        case 4: //false
                            {
                                pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1],(object_t){
                                    .object_type = OBJECT_VALUE,
                                    .vv.value = (value_t){
                                        .value_type = VALUE_BOOL,
                                        .value.boolean = 0
                                    }
                                });

                                Paired = 1;

                                break;
                            }
                        default:
                            printf("the other keywords come up unexpectedly (NormalKeyword).\n");
                            exit(-5);
                        }

                        break;
                    }

                if (Paired) continue;

                unsigned HeadDetectIndex = 0;

                while (HeadKeyword[HeadDetectIndex].token)
                {
                    if (strcmp(ThisWord.word, HeadKeyword[HeadDetectIndex].name) == 0)
                    {
                        pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1],(object_t){
                            .object_type = OBJECT_HEAD,
                            .token = HeadKeyword[HeadDetectIndex].token
                        });

                        Paired = 1;

                        break;
                    }
                    HeadDetectIndex++;
                }


                if (Paired) continue;

                unsigned WeldDetectIndex = 0;

                while (WeldKeyword[WeldDetectIndex].token)
                {
                    if (strcmp(ThisWord.word, WeldKeyword[WeldDetectIndex].name) == 0)
                    {
                        pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1],(object_t){
                            .object_type = OBJECT_WELD,
                            .token = WeldKeyword[WeldDetectIndex].token
                        });

                        Paired = 1;

                        break;
                    }
                    WeldDetectIndex++;
                }

                if (Paired) continue;
            }
        case WORD_CONNECTABLE: //搭配著尚未分配的Normal Case。該藍處理Prompt 及 Symbol 與部分VALUE
        case WORD_UNCONNECTABLE:
            {
                  for (
                  unsigned int keywordDetectIndex = 0;
                  keywordDetectIndex < (unsigned)(sizeof(UnconnectableKeyword)/sizeof(char*));
                  keywordDetectIndex++
                  )
                    if (strcmp(ThisWord.word, UnconnectableKeyword[keywordDetectIndex])==0)
                    {
                        Paired = 1;

                        switch (keywordDetectIndex)
                        {
                        case 0: // {
                            {
                                pushLayoutIntoLayoutCarrier(
                                    &layout_carrier,
                                    (layout_t){
                                        .object_carrier = (object_carrier_t){
                                            0
                                        },
                                        .layout_handler = LAYOUT_HANDLER_TABLE
                                    });

                                current_scope_ptr->child_scope_carrier_ptr->scopes_length++;
                                current_scope_ptr->child_scope_carrier_ptr->scopes = realloc(
                                    current_scope_ptr->child_scope_carrier_ptr->scopes,
                                    sizeof(scope_t) * current_scope_ptr->child_scope_carrier_ptr->scopes_length
                                    );
                                current_scope_ptr->child_scope_carrier_ptr->scopes[
                                     current_scope_ptr->child_scope_carrier_ptr->scopes_length - 1
                                    ] = (scope_t){
                                        .parent_scope_ptr = current_scope_ptr,
                                        .child_scope_carrier_ptr = malloc(sizeof(scope_carrier_t)),
                                        .variable_link_ptr_carrier = (variable_link_ptr_carrier_t){0}
                                    };
                                *current_scope_ptr->child_scope_carrier_ptr->scopes[
                                    current_scope_ptr->child_scope_carrier_ptr->scopes_length - 1
                                ].child_scope_carrier_ptr = (scope_carrier_t){0};

                                current_scope_ptr = &current_scope_ptr->child_scope_carrier_ptr->scopes[
                                     current_scope_ptr->child_scope_carrier_ptr->scopes_length - 1
                                    ];

                                Paired = 1;

                                break;
                            }

                        case 1: // }
                            {
                                const layout_t LastLayout = popLayoutFromLayoutCarrier(&layout_carrier);

                                if (LastLayout.layout_handler != LAYOUT_HANDLER_TABLE)
                                {
                                    exit(1);
                                    goto end;
                                }

                                train_carrier_t train_carrier = object_to_train(LastLayout.object_carrier);

                                pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1],(object_t){
                                    .object_type = OBJECT_VALUE,
                                    .vv.value = (value_t){
                                        .value_type = LastLayout.layout_handler == LAYOUT_HANDLER_TABLE,
                                        .value.train_carrier_ptr = memcpy(malloc(sizeof(train_carrier_t)),&train_carrier,sizeof(train_carrier_t))
                                    }
                                });

                                Paired = 1;

                                break;
                            }

                        default:
                            printf("the other keywords come up unexpectedly (UnconnectableKeyword).\n");
                            exit(-3);
                        }

                        break;
                    }

                if (Paired) continue;

                unsigned PromptDetectIndex = 0;

                while (PromptKeyword[PromptDetectIndex].token)
                {
                    if (strcmp(ThisWord.word, PromptKeyword[PromptDetectIndex].name) == 0)
                    {
                        pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1],(object_t){
                           .object_type = OBJECT_PROMPT,
                           .token = PromptKeyword[PromptDetectIndex].token
                       });

                        Paired = 1;

                        break;
                    }

                    PromptDetectIndex++;
                }

                if (Paired) continue;

                unsigned SymbolDetectIndex = 0;

                while (SymbolKeyword[SymbolDetectIndex].token)
                {
                    if (strcmp(ThisWord.word, SymbolKeyword[SymbolDetectIndex].name) == 0)
                    {
                        pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1],(object_t){
                           .object_type = OBJECT_SYMBOL,
                           .token = SymbolKeyword[SymbolDetectIndex].token
                       });


                        Paired = 1;
                        break;
                    }

                    SymbolDetectIndex++;
                }

                if (Paired) continue;
            }

        default: //二次處理
            {
                switch (ThisWord.word_type)
                {
                case WORD_NORMAL: //配對成Variable
                    {
                        unsigned scope_depth = 0;

                        variable_link_t * v_link_ptr = find_variable_in_scope(
                            current_scope_ptr,
                            ThisWord.word,
                            &scope_depth
                            );

                        variable_t variable = {0};
                        variable_link_t * variable_link_ptr = 0;

                        if (!v_link_ptr)
                        {
                            //新變數

                            variable_link_ptr = malloc(sizeof(variable_link_t));
                            *variable_link_ptr =
                                (variable_link_t){
                                    .variable_link_type = VARIABLE_LINK_LEADER,
                                    .toward_variable_ptr = malloc(sizeof(variable_t)),
                                };

                            variable.is_dummy = 0;
                            variable.variable.genuine_variable = (genuine_variable_t){
                                .value = (value_t){0},
                                .name = ThisWord.word
                            };

                            *variable_link_ptr->toward_variable_ptr = variable;
                        }else
                        {
                            switch (v_link_ptr->variable_link_type)
                            {
                            case VARIABLE_LINK_COLEADER:
                            case VARIABLE_LINK_LEADER:
                                {
                                    if (scope_depth)
                                    {
                                        //同域沒有該變數

                                        variable_link_ptr = malloc(sizeof(variable_link_t));
                                        *variable_link_ptr = (variable_link_t){
                                            .variable_link_type = VARIABLE_LINK_COLEADER,
                                            .toward_variable_link_ptr = v_link_ptr,
                                        };

                                        variable.is_dummy = 1;
                                        variable.variable.dummy_variable = (dummy_variable_t){
                                            .variable_link = *variable_link_ptr
                                        };
                                    }else
                                    {
                                        //同域已有該變數，且一定為COLEADER

                                        variable_link_ptr = malloc(sizeof(variable_link_t));
                                        *variable_link_ptr = (variable_link_t){
                                            .variable_link_type = VARIABLE_LINK_FOLLOWER,
                                            .toward_variable_link_ptr = v_link_ptr,
                                        };

                                        variable.is_dummy = 1;
                                        variable.variable.dummy_variable = (dummy_variable_t){
                                            .variable_link = *variable_link_ptr
                                        };
                                    }
                                    break;
                                }
                            case VARIABLE_LINK_FOLLOWER:
                                {
                                    //同域已有該變數

                                    variable_link_ptr = 0;

                                    variable.is_dummy = 1;
                                    variable.variable.dummy_variable = (dummy_variable_t){
                                        .variable_link = *v_link_ptr
                                    };

                                    break;
                                }
                            default:
                                exit(125);
                            }
                        }

                        printf(
                            "VARIABLE CREATED!\n"
                            "VARIABLE NAME: %s\n"
                            "VARIABLE IS_DUMMY: %d\n"
                            ,ThisWord.word,variable.is_dummy
                            );


                        if (variable_link_ptr)
                        {
                            printf(
                            "   VARIABLE LINK CREATED!\n"
                            "   VARIABLE LINK: %p\n"
                            "   VARIABLE LINK TYPE: %d\n"
                            "   VARIABLE LINK TO VARIABLE: %p\n"
                            "   VARIABLE LINK TO VARIABLE LINK: %p\n"
                            ,variable_link_ptr,variable_link_ptr->variable_link_type,variable_link_ptr->toward_variable_ptr,variable_link_ptr->toward_variable_link_ptr
                            );

                            current_scope_ptr->variable_link_ptr_carrier.variable_link_ptrs_length++;
                            current_scope_ptr->variable_link_ptr_carrier.variable_link_ptrs = realloc(
                                current_scope_ptr->variable_link_ptr_carrier.variable_link_ptrs,
                            current_scope_ptr->variable_link_ptr_carrier.variable_link_ptrs_length * sizeof(variable_link_t*)
                                );
                            current_scope_ptr->variable_link_ptr_carrier.variable_link_ptrs[
                            current_scope_ptr->variable_link_ptr_carrier.variable_link_ptrs_length - 1
                                ] = variable_link_ptr;
                        }

                        pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1],(object_t){
                           .object_type = OBJECT_VARIABLE,
                            .vv.variable = variable,
                       });

                        Paired = 1;

                        break;
                    }
                case WORD_CONNECTABLE: //跳出 之後Error Handle 嘻嘻
                case WORD_UNCONNECTABLE:
                    {
                        exit(1);
                        goto end;
                    }

                case WORD_DOUBLE_STRING: //配對成 Value 的String
                case WORD_SINGLE_STRING:
                    {
                        pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1], (object_t){
                            .object_type = OBJECT_VALUE,
                            .vv.value = (value_t){
                                .value_type = VALUE_STRING,
                                .value.string = ThisWord.word
                            },
                        });


                        Paired = 1;

                        break;
                    }
                case WORD_DECNUMBER:
                    {
                        pushMioneObjectIntoLayout(&layout_carrier.layouts[layout_carrier.layouts_length - 1], (object_t){
                            .object_type = OBJECT_VALUE,
                            .vv.value = (value_t){
                                .value_type = VALUE_NUMBER,
                                .value.number = atof(ThisWord.word)
                            },
                        });

                        break;
                    }
                case WORD_BINNUMBER:
                case WORD_HEXNUMBER:
                    exit(123);


                default:break;
                }
            };
        }

        // free(ThisWord.ObjName);
    }
    // free(word_carrier.Carrier);

    if (layout_carrier.layouts_length!=1)
    {
        exit(100);
        goto end;
    }

    end:

    return layout_carrier.layouts[0].object_carrier;
}
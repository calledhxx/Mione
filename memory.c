#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#include "memory.h"

#define offsetByte(i) ((i ? (i%3 ? i%3 : 3) : 0) * 64 * (1<<(2*(int)((i ? i - 1 : 0)/3))))

line_t ** LinePointerList = 0;
unsigned LinePointerListLength = 0;

unsigned offsetLog2(unsigned x)
{
    unsigned result = 0;

    while (x>>1)
    {
        x>>=1;
        result++;
    }

    return result;
}

unit_t * AlignedMalloc(unsigned const alignTo,size_t const size,void ** originalPointerPTR)
{
    unit_t* ptr = *originalPointerPTR = malloc(size + alignTo);

    if (!ptr)
        exit(1);

    return ptr + alignTo - (intptr_t)ptr % alignTo;
}

void CreateLine(void)
{
    LinePointerListLength++;
    LinePointerList = realloc(LinePointerList,sizeof(line_t*) * LinePointerListLength);
    if (LinePointerList == NULL)
        exit(1);

    LinePointerList[LinePointerListLength - 1] = malloc(sizeof(line_t));
    LinePointerList[LinePointerListLength - 1]->units = AlignedMalloc(
        sizeof(block_t),
        1048576,
        &LinePointerList[LinePointerListLength - 1]->units_original_pointer
        );
    if (LinePointerList[LinePointerListLength - 1] == NULL)
        exit(1);

    LinePointerList[LinePointerListLength - 1]->leader_pointer =
        (block_t *)LinePointerList[LinePointerListLength - 1]->units;

    for (int i = 0; i < 22; i++)
    {
        block_t* const address = (block_t*)(LinePointerList[LinePointerListLength - 1]->units + offsetByte(i));
        address->size = 64 * (1<<(2*(int)floor((i ? i - 1 : 0)/3)));
        address->current = address;
        address->next =
            i == 21
        ? NULL
        : (block_t*)(LinePointerList[LinePointerListLength - 1]->units + offsetByte(i+1));
    }
}

void CountToGetBlockInfo(unsigned const distance,
                         unsigned * UsedBlockLenPTR,
                         unsigned * CurrentBlockSizePTR,
                         unsigned * IndexInBlockGroupPTR)
{
    if (!distance)
    {
        * UsedBlockLenPTR = 1;
        * CurrentBlockSizePTR = 64;
        * IndexInBlockGroupPTR = 0;

        return;
    }

    unsigned const log_2 = offsetLog2(distance);
    *CurrentBlockSizePTR = 1<<(log_2&1 ? log_2-1 : log_2);

    unsigned const log_4 = offsetLog2(*CurrentBlockSizePTR / 16) / 2;
    *IndexInBlockGroupPTR = distance/ *CurrentBlockSizePTR;
    *UsedBlockLenPTR = (log_4 - 1 ? (log_4 - 1 )* 3 : 1) + *IndexInBlockGroupPTR;
}

int stackIndex = 0;

unit_t* Allocate(unit_t * const originalPointer, size_t const size)
{
    printf("%d\n",++stackIndex);
    block_t * currentBlockPtr = LinePointerList[LinePointerListLength - 1]->leader_pointer;
    unit_t * result = 0;

    if (!currentBlockPtr)
        exit(3);

    unsigned
        UsedBlockLen = 0,
        CurrentBlockSize = 0,
        IndexInBlockGroup = 0;

    if (originalPointer)
    {
        CountToGetBlockInfo(
            (intptr_t)originalPointer - (intptr_t)LinePointerList[LinePointerListLength - 1]->units,
            &UsedBlockLen,
            &CurrentBlockSize,
            &IndexInBlockGroup
            );


        if (CurrentBlockSize >= size)
        {
            result = originalPointer;
            memcpy(result, originalPointer, CurrentBlockSize);
            goto ret;
        }
    }

    while (currentBlockPtr)
    {
        if (currentBlockPtr->size >= size)
        {
            LinePointerList[LinePointerListLength - 1]->leader_pointer = currentBlockPtr->next;
            result = (unit_t*)currentBlockPtr;
            break;
        }
        currentBlockPtr = currentBlockPtr->next;
    }


    ret:
    return result;
}

void Free(unit_t * const originalPointer)
{
    printf("%d\n",--stackIndex);

    block_t * currentBlockPtr = LinePointerList[LinePointerListLength - 1]->leader_pointer;

    if (!currentBlockPtr)
        exit(3);

    if (!originalPointer)
        return;

    unsigned
        UsedBlockLen = 0,
        CurrentBlockSize = 0,
        IndexInBlockGroup = 0;

    CountToGetBlockInfo(
            (intptr_t)originalPointer - (intptr_t)LinePointerList[LinePointerListLength - 1]->units,
            &UsedBlockLen,
            &CurrentBlockSize,
            &IndexInBlockGroup
            );


    ((block_t*)originalPointer)->next = currentBlockPtr;
    ((block_t*)originalPointer)->size = CurrentBlockSize;
    LinePointerList[LinePointerListLength - 1]->leader_pointer =
        ((block_t*)originalPointer)->current =
            originalPointer;
}

    // CreateLine();
    // Allocate(0,15);
    // Free(a);

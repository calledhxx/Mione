#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#include "memory.h"

#define offsetByte(i) (((i) ? ((i)%3 ? (i)%3 : 3) : 0) * 64 * (1<<(2*(int)(((i) ? (i) - 1 : 0)/3))))

line_t ** LinePointerList = 0;
unsigned LinePointerListLength = 0;

unit_t ** MegaAllocateList = 0;
unsigned MegaAllocateListLength = 0;

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

void print_line(line_t const line)
{
    block_t * currentBlockPtr = line.leader_pointer;

    for (int i = 0; i < 22; i++)
    {
        block_t* const address = (block_t*)(line.units + offsetByte(i));

        printf("    %d: addr:%llu, slef:%llu %d %llu\n",i,address,address->current,address->size,address->next);

    }
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

        // printf("%d: %llu %d %llu\n",i,address->current,address->size,address->next);
    }
}

line_t * GetLine(unit_t const * const unitPointer)
{
    if (!unitPointer)
        return LinePointerList[LinePointerListLength - 1];

    line_t * result = 0;

    for (int i = 0;i < LinePointerListLength; i++)
    {
        line_t * line = LinePointerList[i];

        if (!result || (unitPointer - result->units > unitPointer - line->units && unitPointer - line->units >= 0))
        {
            result = line;
        }
    }

    return result;
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

    // printf("dis %u\n",distance);
    // printf("used %u\n",*UsedBlockLenPTR);
    // printf("block %u\n",*CurrentBlockSizePTR);
    // printf("index %u\n",*IndexInBlockGroupPTR);
}

void fre(unit_t * const originalPointer)
{
    for (int i = 0; i < MegaAllocateListLength; i++)
    {
        if (MegaAllocateList[i] == originalPointer)
        {
            free(MegaAllocateList[i]);
            MegaAllocateList[i] = 0;
            return;
        }
    }

    line_t * const line = GetLine(originalPointer);
    block_t * currentBlockPtr = line->leader_pointer;

    if (!originalPointer)
        return;

    unsigned
        UsedBlockLen = 0,
        CurrentBlockSize = 0,
        IndexInBlockGroup = 0;

    CountToGetBlockInfo(
            (intptr_t)originalPointer - (intptr_t)line->units,
            &UsedBlockLen,
            &CurrentBlockSize,
            &IndexInBlockGroup
            );

    ((block_t*)originalPointer)->size = CurrentBlockSize;

    if ((intptr_t)currentBlockPtr > (intptr_t)originalPointer)
    {
        ((block_t*)originalPointer)->next = currentBlockPtr;
        ((block_t*)originalPointer)->current =
            line->leader_pointer =
                originalPointer;

        // printf("leader -> %llu fre\n",line->leader_pointer);
    }else
    {
        ((block_t*)originalPointer)->next = currentBlockPtr ? currentBlockPtr->next : 0;
        ((block_t*)originalPointer)->current = originalPointer;
    }
}

unit_t* Allocate(unit_t * const originalPointer, size_t const size)
{
    line_t * const line = GetLine(originalPointer);

    // print_line(*line);

    block_t * currentBlockPtr = line->leader_pointer;
    unit_t * result = 0;

    if (!currentBlockPtr)
    {
        result = (unit_t*)((intptr_t)result | POINTER_TAG_LINE_LEADER_NOT_FOUND);
        goto ret;
    }

    unsigned
            UsedBlockLen = 0,
            CurrentBlockSize = 0,
            IndexInBlockGroup = 0;

    if (originalPointer)
    {
        CountToGetBlockInfo(
            (intptr_t)originalPointer - (intptr_t)line->units,
            &UsedBlockLen,
            &CurrentBlockSize,
            &IndexInBlockGroup
            );

        if (CurrentBlockSize >= size)
        {
            result = originalPointer;
            goto ret;
        }

    }

    while (currentBlockPtr)
    {
        if (currentBlockPtr->size >= size)
        {
            result = (unit_t*)currentBlockPtr->current;
            line->leader_pointer = currentBlockPtr->next;
            // printf("leader -> %llu allocate\n",line->leader_pointer);


            if (currentBlockPtr->size < CurrentBlockSize)
                exit(323);

            if (originalPointer)
                memcpy(result, originalPointer, CurrentBlockSize);

            fre(originalPointer);
            goto ret;
        }
        currentBlockPtr = currentBlockPtr->next;
    }

    result = (unit_t*)((intptr_t)result | POINTER_TAG_LINE_HAS_RUN_OUT);

    ret:
    return result;
}

unit_t* alc(unit_t * const originalPointer, size_t const size)
{
    unit_t * result = 0;

    if (size > 262144)
    {
        MegaAllocateListLength++;
        MegaAllocateList = realloc(MegaAllocateList,sizeof(unit_t*) * MegaAllocateListLength);
        MegaAllocateList[MegaAllocateListLength - 1] = result = malloc(size);
    }else
    {
        for (int i = 0; i < MegaAllocateListLength; i++)
        {
            if (MegaAllocateList[i] == originalPointer)
            {
                return originalPointer;
            }
        }

        result = Allocate(originalPointer,size);

        if ((intptr_t)result & POINTER_TAG_LINE_HAS_RUN_OUT)
        {
            CreateLine();
            result = Allocate(originalPointer,size);

            // if it still fails, then we are really out of memory
        }else if ((intptr_t)result & POINTER_TAG_LINE_LEADER_NOT_FOUND)
        {
            CreateLine();
            result = Allocate(0,size);

            if (originalPointer)
            {
                line_t * const line = GetLine(originalPointer);

                unsigned
                    UsedBlockLen = 0,
                    CurrentBlockSize = 0,
                    IndexInBlockGroup = 0;

                CountToGetBlockInfo(
                        (intptr_t)originalPointer - (intptr_t)line->units,
                        &UsedBlockLen,
                        &CurrentBlockSize,
                        &IndexInBlockGroup
                        );

                memcpy(result,originalPointer,size > CurrentBlockSize ? CurrentBlockSize : size);
            }

            fre(originalPointer);

            // if it still fails, then we are really out of memory
        }
    }

    result = (unit_t*)((intptr_t)result & ~0x4); //clean pointer tag

    return result;
}

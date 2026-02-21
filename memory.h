#ifndef MEMORY_H
#define MEMORY_H

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
    block_t * leader_pointer;
    void * units_original_pointer;

} line_t;

extern void CreateLine(void);
extern unit_t* Allocate(unit_t * const originalPointer, size_t const size);
extern void Free(unit_t * const originalPointer);

#endif

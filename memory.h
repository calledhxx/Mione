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

enum pointer_tag_e
{
    POINTER_TAG_NONE = 0,
    POINTER_TAG_LINE_HAS_RUN_OUT = 1<<0,
};

extern void CreateLine(void);
extern unit_t* Allocate(unit_t * const originalPointer, size_t const size);

unit_t* alc(unit_t * const originalPointer, size_t const size);
extern void fre(unit_t * const originalPointer);

#endif

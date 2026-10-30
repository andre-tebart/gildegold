/* mem.c -- original file name unknown ("m_alloc_debug", "m:pool_alloc(tp_m_pool)").
   Tracked allocations: every block is recorded in a fixed table and counted per group, the tag prefix
   up to ':'. Pools hand out fixed-size zeroed slots from linked blocks.
   Extent in GildeGold.exe (inferred, not proven): 0x4223c0..0x422c90.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/mem.c); boundaries in decl/text.dcl, notes in notes/units/mem.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "err.h"
#include "mem.h"

/* 0x4223c0 */
void *mem_Alloc(int size, char *tag)
{
    char buf[256];
    char group[16];
    MEMGROUP *g, *last;
    void *p;
    char *c;
    int n, i;
    MEMENTRY *e;

    if (!mem_Table)
        return NULL;
    if (!size)
        return NULL;
    mem_Count++;
    g = &mem_Main;
    if (tag && (c = strchr(tag, ':'))) {
        n = c - tag + 1;
        if (n >= 16) {
            sprintf(buf, "m_alloc_debug: GroupInformation too long: %s", tag);
            err_Message(buf);
        }
        strncpy(group, tag, n);
        group[n] = 0;
        g = &mem_Main;
        do {
            if (!strcmp(group, g->name))
                break;
            last = g;
            g = g->next;
        } while (g);
        if (!g) {
            g = malloc(sizeof(MEMGROUP));
            strcpy(g->name, group);
            g->blocks = 0;
            g->bytes = 0;
            g->maxblocks = 0;
            g->maxbytes = 0;
            g->calls = 0;
            g->next = NULL;
            last->next = g;
        }
    }
    g->calls++;
    n = size;
    p = malloc(n);
    if (!p) {
        sprintf(group, "Out of Memory Error: Malloc - Failed %s for %u bytes", tag, size);
        return NULL;
    }
    MEM_ZERO_VAR(p, n);
    if (mem_Blocks < mem_Max) {
        for (i = 0, e = mem_Table; i < mem_Max; e++, i++)
            if (!e->ptr)
                break;
        e->ptr = p;
        e->tag = tag;
        e->size = size;
        e->group = g;
        mem_Bytes += size;
        if (mem_Bytes > mem_MaxBytes)
            mem_MaxBytes = mem_Bytes;
        g->bytes += size;
        if (g->bytes > g->maxbytes)
            g->maxbytes = g->bytes;
        mem_Blocks++;
        if (mem_Blocks > mem_MaxBlocks)
            mem_MaxBlocks = mem_Blocks;
        g->blocks++;
        if (g->blocks > g->maxblocks)
            g->maxblocks = g->blocks;
    }
    return p;
}
/* 0x4225e0, 1 when p is NULL or a table-listed allocation, 0 for an untracked pointer */

int mem_IsValid(void *p)
{
    int i;
    MEMENTRY *e;

    if (p) {
        for (i = 0, e = mem_Table; i < mem_Max; e++, i++)
            if (e->ptr == p)
                break;
        return i != mem_Max;
    }
    return 1;
}
/* 0x422620 */

void mem_Free(void *p)
{
    int i, size;
    MEMENTRY *e;

    if (p) {
        mem_Count--;
        for (i = 0, e = mem_Table; i < mem_Max; e++, i++)
            if (e->ptr == p)
                break;
        if (i != mem_Max) {
            size = e->size;
            e->ptr = NULL;
            free(p);
            mem_Blocks--;
            e->group->blocks--;
            mem_Bytes -= size;
            e->group->bytes -= size;
        }
    }
}
/* 0x4226a0, allocate and zero the max-entry MEMENTRY table, reset the counters and the main group */

void mem_Init(int max)
{
    int size;

    mem_Max = max;
    size = max * sizeof(MEMENTRY);
    mem_Bytes = 0;
    mem_MaxBytes = 0;
    mem_Blocks = 0;
    mem_MaxBlocks = 0;
    mem_Table = malloc(size);
    D3_ZERO_AT(mem_Table, size);
    mem_Main.blocks = 0;
    mem_Main.bytes = 0;
    mem_Main.maxblocks = 0;
    mem_Main.maxbytes = 0;
    mem_Main.calls = 0;
    mem_Main.next = NULL;
    strcpy(mem_Main.name, "_main_:");
}
/* 0x422750, format the memory statistics (per group: max / current blocks and bytes, the filtered blocks) into a local buffer; the original has no output call */

void mem_Dump(char *filter)
{
    char name[16];
    char buf[1024];
    MEMGROUP *g;
    MEMENTRY *e;
    char match;
    char *c;
    int i;

    if (!mem_Bytes && !mem_Blocks)
        sprintf(buf, "[*] Memory-Subsystem-Information:\n--> max_used %i blocks in %i bytes (%ik).",
                mem_MaxBlocks, mem_MaxBytes, mem_MaxBytes >> 10);
    else
        sprintf(buf, "[*] Memory-Subsystem-Information: [*]\n--> max_used %i blocks in %i bytes (%ik). %i block(s) left in %i (%ik) bytes.",
                mem_MaxBlocks, mem_MaxBytes, mem_MaxBytes >> 10, mem_Blocks, mem_Bytes, mem_Bytes >> 10);
    g = &mem_Main;
    do {
        if (filter && strstr(g->name, filter))
            match = 1;
        else
            match = 0;
        if (g->maxblocks > 0 && (g->name[0] != '!' || g->blocks || match) && (!filter || match)) {
            strcpy(name, g->name);
            c = strrchr(name, ':');
            if (c)
                *c = 0;
            if (!g->bytes && !g->blocks)
                sprintf(buf, "[%s] w/ %i (%ik) bytes in %i block(s) (called %i times).",
                        name, g->maxbytes, g->maxbytes >> 10, g->maxblocks, g->calls);
            else
                sprintf(buf, "[%s] -> w/ %i/%i (%ik/%ik) bytes in %i/%i block(s) (called %i times).",
                        name, g->bytes, g->maxbytes, g->bytes >> 10, g->maxbytes >> 10, g->blocks, g->maxblocks, g->calls);
            if (match) {
                for (i = 0, e = mem_Table; i < mem_Max; i++, e++)
                    if (e->ptr && e->group == g)
                        sprintf(buf, "{%s} w/ %i bytes (%ik).", e->tag, e->size, e->size >> 10);
            }
        }
        g = g->next;
    } while (g);
}
/* 0x422910, report and free the blocks still allocated, dump the groups when verbose, free the group list and the table, reset the counters */

void mem_Exit(void)
{
    char buf[1024];
    int i;
    MEMENTRY *e;
    MEMGROUP *g, *n;

    if (mem_Blocks || mem_Bytes || mem_Verbose) {
        if (mem_Blocks || mem_Bytes) {
            sprintf(buf, "[*] Memory-Shutdown-Anomaly: [*]\nUsed %i blocks in %i bytes.\n\n%i block(s) left in %i bytes.",
                    mem_MaxBlocks, mem_MaxBytes, mem_Blocks, mem_Bytes);
            err_Message(buf);
        }
        if (mem_Verbose)
            mem_Dump(NULL);
        for (i = 0, e = mem_Table; i < mem_Max; i++, e++) {
            if (e->ptr) {
                sprintf(buf, "-> Pointer not freed:%s", e->tag);
                free(e->ptr);
                e->ptr = NULL;
            }
        }
    }
    mem_MaxBlocks = 0;
    mem_Blocks = 0;
    mem_MaxBytes = 0;
    mem_Bytes = 0;
    mem_Max = 0;
    for (g = mem_Main.next; g; g = n) {
        n = g->next;
        free(g);
    }
    free(mem_Table);
    mem_Table = NULL;
}
/* 0x422a20 */

void *pool_Alloc(MPOOL **pool, unsigned int size, int count)
{
    MPOOL *p;
    int *slot;
    int i, sz, rem;

    if (pool && size > 0 && count > 0) {
        sz = size + 4;
        rem = size & 3;
        if (rem > 0)
            sz += 4 - rem;
        p = *pool;
        if (!p) {
            p = mem_Alloc(sz * count + sizeof(MPOOL), "m:pool_get_free(tp_m_pool)");
            *pool = p;
            p->used = 0;
            p->count = count;
            p->size = sz;
            p->next = NULL;
        } else {
            do {
                if (p->used < p->count && p->size == sz)
                    goto found;
                p = p->next;
            } while (p);
            p = mem_Alloc(sz * count + sizeof(MPOOL), "m:pool_alloc(tp_m_pool)");
            p->used = 0;
            p->count = count;
            p->size = sz;
            p->next = *pool;
            *pool = p;
        }
found:
        for (i = 0, slot = (int *)(p + 1); i < p->count; i++, slot = (int *)((char *)slot + p->size)) {
            if (*slot == 0) {
                p->used++;
                *slot++ = (int)p;
                MEM_ZERO(slot, sz - 4);
                return slot;
            }
        }
    }
    return NULL;
}
/* 0x422b30 */

void pool_Free(MPOOL **pool, void *ptr)
{
    int *slot = (int *)ptr - 1;
    MPOOL *p, *q;

    if (slot && (p = (MPOOL *)*slot) && (char *)ptr >= (char *)(p + 1)
        && (char *)ptr < (char *)(p + 1) + p->size * p->count) {
        *slot = 0;
        if (--p->used == 0) {
            if (p == *pool) {
                *pool = p->next;
                mem_Free(p);
                return;
            }
            for (q = *pool; q->next != p; q = q->next)
                ;
            q->next = p->next;
            mem_Free(p);
        }
    }
}

/* returns the next used slot (the record after its in-use flag) of the pool chain walked by it, 0 at the end.
   The scan is `while (1) { if (*slot) break; ... }`: a plain while would be rotated by VC6. */
/* 0x422ba0 @deep byte work only (behaviour + readable met): 2 B / 4 asm lines, the imul operand order (ours mov esi,edx; imul esi,edi; the original mov esi,edi; imul esi,edx); grade.py diffclass verdict regs 6, spell.py finds no menu hunk; size*i, parentheses, casts, unsigned, an off temp, declaration orders and moving i = it->index all give the same code (notes/units/mem.md) */
void *pool_Next(MPOOLITER *it)
{
    int i, size;
    int *slot;

    if (it && it->pool) {
        i = it->index;
        do {
            size = it->pool->size;
            if (it->left == 0)
                it->left = it->pool->used;
            slot = (int *)((char *)(it->pool + 1) + i * size);
            if (it->left > 0) {
                while (1) {
                    if (*slot)
                        break;
                    i++;
                    slot = (int *)((char *)slot + size);
                }
                if (--it->left <= 0) {
                    it->pool = it->pool->next;
                    it->index = 0;
                    it->left = 0;
                } else
                    it->index = i + 1;
                return slot + 1;
            }
            it->pool = it->pool->next;
        } while (it->pool);
    }
    return NULL;
}
/* 0x422c30, initialise it for pool and return the first used slot (pool_Next), NULL without pool or it */

void *pool_First(MPOOL *pool, MPOOLITER *it)
{
    if (pool && it) {
        it->pool = pool;
        it->index = 0;
        it->left = 0;
        return pool_Next(it);
    }
    return NULL;
}
/* 0x422c60, free every block of the pool chain and clear *pool */

void pool_Destroy(MPOOL **pool)
{
    MPOOL *p, *n;

    if (pool) {
        for (p = *pool; p; p = n) {
            n = p->next;
            mem_Free(p);
        }
        *pool = NULL;
    }
}

extern MEMGROUP mem_Main;
int mem_Max = 0;  /* 0x632384, entries in mem_Table */
int mem_Bytes = 0;  /* 0x632388 */
int mem_Blocks = 0;  /* 0x63238c */
int mem_MaxBytes = 0;  /* 0x632390 */
int mem_MaxBlocks = 0;  /* 0x632394 */
int mem_Count = 0;  /* 0x632398, allocations minus frees */
int mem_Verbose = 0;  /* 0x63239c, dump the groups at exit */
MEMENTRY * mem_Table = 0;  /* 0x6323a0 */
MEMGROUP mem_Main = {0};  /* 0x1461400, "_main_:", head of the group list */

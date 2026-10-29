/* mem.c -- tracked memory allocation with per-group statistics ("m_alloc_debug") and fixed-size
   pools ("m:pool_..."), original file name unknown. Extent in GildeGold.exe (inferred): 0x4223c0..0x422c90.
   mem_Alloc / mem_Free are also declared in d3.h and the callers' headers (foreign declarations, mem.c's
   readability debt; this header is the canonical place). Names are ours. */
#ifndef UNK_4223C0_H
#define UNK_4223C0_H

/* The engine's inline-asm clear (see D3_ZERO in d3.h), here with the destination temp declared first:
   that decides which of the two temps is stored first. */
#define MEM_ZERO(dst, size) {                                       \
    void *_zp = (dst); int _zn = (size);                            \
    __asm mov edi, _zp                                              \
    __asm mov ebx, _zn                                              \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_ZERO_LABEL(__LINE__)                               \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm D3_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep stosb                                                 \
}

/* Same, naming two variables directly (no temps; both must be plain variables). */
#define MEM_ZERO_VAR(dst, size) {                                   \
    __asm mov edi, dst                                              \
    __asm mov ebx, size                                             \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_ZERO_LABEL(__LINE__)                               \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm D3_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep stosb                                                 \
}

typedef struct MEMGROUP {           /* 0x28 bytes, statistics of one tag prefix ("group:") */
    char name[0x10];                /* prefix including the ':' */
    int bytes;                      /* 0x10 */
    int blocks;                     /* 0x14 */
    int maxbytes;                   /* 0x18 */
    int maxblocks;                  /* 0x1c */
    int calls;                      /* 0x20 */
    struct MEMGROUP *next;          /* 0x24 */
} MEMGROUP;

typedef struct MEMENTRY {           /* 0x10 bytes, mem_Max of them at mem_Table */
    MEMGROUP *group;
    char *tag;                      /* 0x04 */
    void *ptr;                      /* 0x08, NULL = free entry */
    int size;                       /* 0x0c */
} MEMENTRY;

typedef struct MPOOL {              /* header of a pool block; count slots of size bytes follow */
    int used;
    int count;                      /* 0x04 */
    int size;                       /* 0x08, slot size: 4 byte owner pointer + data, 4-aligned */
    struct MPOOL *next;             /* 0x0c */
} MPOOL;

typedef struct MPOOLITER {
    MPOOL *pool;
    int index;                      /* 0x04 */
    int left;                       /* 0x08, used slots not visited yet in pool */
} MPOOLITER;

extern MEMGROUP mem_Main;                   /* 0x1461400, "_main_:", head of the group list */
extern MEMENTRY *mem_Table;                 /* 0x6323a0 */
extern int mem_Max;                         /* 0x632384, entries in mem_Table */
extern int mem_Bytes;                       /* 0x632388 */
extern int mem_Blocks;                      /* 0x63238c */
extern int mem_MaxBytes;                    /* 0x632390 */
extern int mem_MaxBlocks;                   /* 0x632394 */
extern int mem_Count;                       /* 0x632398, allocations minus frees */
extern int mem_Verbose;                     /* 0x63239c, dump the groups at exit */

void *mem_Alloc(int size, char *tag);                                                   /* 0x4223c0 */
int mem_IsValid(void *p);                                                               /* 0x4225e0 */
void mem_Free(void *p);                                                                 /* 0x422620 */
void mem_Init(int max);                                                                 /* 0x4226a0 */
void mem_Dump(char *filter);                                                            /* 0x422750 */
void mem_Exit(void);                                                                    /* 0x422910 */
void *pool_Alloc(MPOOL **pool, unsigned int size, int count);                           /* 0x422a20 */
void pool_Free(struct MPOOL **pool, void *ptr);                                                /* 0x422b30 */
void *pool_Next(MPOOLITER *it);                                                         /* 0x422ba0 */
void * pool_First(struct MPOOL *pool, struct MPOOLITER *it);                                           /* 0x422c30 */
void pool_Destroy(struct MPOOL **pool);                                                        /* 0x422c60 */

#endif

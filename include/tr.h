/* tr.c -- original file name unknown: a record table filled with two marker strings
   ("tr_startup") and the random number helpers. Names are ours except tr_startup. */
#ifndef UNK_42EA90_H
#define UNK_42EA90_H

typedef struct TRREC {                  /* 0x84 bytes */
    char key1[0x74];
    char key2[0x10];                    /* 0x74 */
} TRREC;

extern char *tr_Key1;                   /* 0x5f8fd0, "*bRaIn DeaD%&" */
extern char *tr_Key2;                   /* 0x5f8fd4, "%aRscH Ab :-)" */
extern TRREC *tr_First;                 /* 0x6333f0 */
extern TRREC *tr_End;                   /* 0x6333f4 */
extern TRREC *tr_Table;                 /* 0x6333f8 */

/* Inline-asm clear and copy as D3_ZERO / VFS_COPY: zero through a size and a pointer temp, copy with the
   destination named directly and src, size through temps. */
#define TR_CAT(a, b) a##b
#define TR_LABEL(n) TR_CAT(tr_tail, n)
#define TR_ZERO(dst, size) {                                        \
    int _zn = (size); void *_zp = (dst);                            \
    __asm mov edi, _zp                                              \
    __asm mov ebx, _zn                                              \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle TR_LABEL(__LINE__)                                    \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm TR_LABEL(__LINE__): add ecx, ebx                          \
    __asm rep stosb                                                 \
}
#define TR_COPY(dst, src, size) {                                   \
    int _cn = (size); void *_cs = (src);                            \
    __asm mov edi, dst                                              \
    __asm mov esi, _cs                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle TR_LABEL(__LINE__)                                    \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm TR_LABEL(__LINE__): add ecx, ebx                          \
    __asm rep movsb                                                 \
}


int tr_startup(unsigned short n);                                                       /* 0x42ea90 */
void tr_shutdown(void);                                                                 /* 0x42ebc0 */
/* 0x42ec00, rand() % n */
float util_RandomF(void);                                                               /* 0x42ec20, 0..1 */
float util_RandomSq(void);                                                              /* 0x42ec40, -1..1, squared with sign */
float util_RandomF2(void);                                                              /* 0x42ec70, 0..1 squared */
void util_Randomize(void);                                                              /* 0x42ec80 */
void util_Seed(unsigned int seed);                                                      /* 0x42ec90 */

#endif

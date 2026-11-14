/* qnt.c -- octree color quantizer (24-bit RGB to an 8-bit palette, optional error diffusion),
   original file name unknown. Extent in GildeGold.exe (inferred): 0x4206d0..0x4214a0. Names are ours. */
#ifndef UNK_4206D0_H
#define UNK_4206D0_H

typedef struct QNODE {              /* 0x18 bytes; level n has 8^n nodes, index = interleaved r/g/b bits */
    unsigned int r, g, b;           /* color sums */
    unsigned int count;             /* 0x0c, pixels (after reduction: of the merged subtree) */
    unsigned int total;             /* 0x10, pixels in the subtree, the heap key */
    unsigned char mask;             /* 0x14, children present */
    unsigned char index;            /* 0x15, palette index */
} QNODE;

typedef struct QHEAP {              /* heap entry: a leaf node */
    unsigned char level;
    unsigned char pad;
    unsigned short node;
} QHEAP;

extern QNODE *qnt_Level[6];                 /* 0x632340 */
extern QHEAP *qnt_Heap;                     /* 0x632360, 1-based min-heap on total */
extern void *qnt_UnusedBuffer;                 /* 0x632364 */
extern unsigned int qnt_HeapCount;          /* 0x632358 */
extern int qnt_Inited;                      /* 0x63235c */
extern int qnt_Colors;                      /* 0x631728 */
extern unsigned char qnt_Pal[3][256];       /* 0x631428, [0] red, [1] green, [2] blue */
extern unsigned short qnt_R[256];           /* 0x632140, r bits spread to octree index positions */
extern unsigned short qnt_G[256];           /* 0x631228 */
extern unsigned short qnt_B[256];           /* 0x631f38 */
extern int qnt_Sq[];                        /* 0x631b34, i * i for i = -255..255 (indexed around 0) */
extern int *qnt_SqPtr;                      /* 0x632138 */

int qnt_Quantize(unsigned char *rgb, unsigned char *out, int w, int h, int ncolors, int dither, unsigned char *pal); /* 0x4206d0 */
void qnt_GetPalette(unsigned char *pal);                                                /* 0x420780, 3 x 256 bytes */
void qnt_InitTables(void);                                                              /* 0x4207c0 */
int qnt_Alloc(void);                                                                    /* 0x420870 */
void qnt_Free(void);                                                                    /* 0x420900 */
int qnt_Build(unsigned char *rgb, unsigned int n);                                               /* 0x420990 */
void qnt_Sift(unsigned int k);                                                          /* 0x420b90 */
void qnt_Reduce(unsigned int ncolors);                                                  /* 0x420c90 */
void qnt_MakePalette(int idx, int lvl);                                                 /* 0x420db0 */
int qnt_Map(unsigned char *rgb, unsigned char *out, int w, int h, int dither);          /* 0x420e70 */
int qnt_Nearest(unsigned int r, unsigned int g, unsigned int b);                        /* 0x421410 */

#endif

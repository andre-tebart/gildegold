/* path.c -- A* path search on a square grid (8 neighbours, per-type terrain costs), original
   file name unknown. Extent in GildeGold.exe (inferred): 0x424f30..0x425bc0. Names are ours. */
#ifndef UNK_424F30_H
#define UNK_424F30_H

typedef struct PNODE {              /* 0x1c bytes */
    int type;                       /* terrain type, index into the cost table */
    short stamp;                    /* 0x04, path_Stamp of the search that touched the node */
    short pad06;
    float f;                        /* 0x08, g + h */
    float g;                        /* 0x0c */
    float h;                        /* 0x10 */
    unsigned char flags;            /* 0x14, 1 open, 2 closed, 4/8 reached from start/goal */
    unsigned char pad15;
    unsigned short next;            /* 0x16, open list, sorted by f */
    unsigned short prev;            /* 0x18 */
    unsigned short parent;          /* 0x1a */
} PNODE;

typedef struct PMAP {
    char pad00[0x20];
    int size;                       /* 0x20, grid is size x size, a power of two */
    PNODE *nodes;                   /* 0x24 */
} PMAP;

typedef struct PATHOUT {
    int count;
    int max;                        /* 0x04 */
    unsigned char *xy;              /* 0x08, max (x, y) byte pairs */
} PATHOUT;

extern int path_Mask;                       /* 0x5f7f60, size - 1 */
extern int path_Shift;                      /* 0x5f7f64, log2(size) */
extern short path_DX[8];                    /* 0x5f7f68 */
extern short path_DY[8];                    /* 0x5f7f78, set per search from the grid size */
extern float path_Step[8];                  /* 0x5f7f88, 1 or sqrt(2) */
extern float path_Terrain[][15];            /* 0x5f7fa8, cost per type (6 rows); < 0 = impassable */
extern float path_Tab1[6];                  /* 0x5f8110, per type: 0.25, 0.0 for one type */
extern float path_Tab2[6];                  /* 0x5f8128, same shape as path_Tab1 */
extern unsigned char path_SetFlag;          /* 0x1460aa0 */
extern float *path_Costs;                   /* 0x1460aa4, path_Terrain[type] of the running search */
extern unsigned char path_TestFlag;         /* 0x1460aa8 */
extern float path_Tab1Sel;                  /* 0x1460aac, path_Tab1[type], written per search, never read */
extern unsigned short path_Count;           /* 0x1460ab0 */
extern short path_Stamp;                    /* 0x1460ab2 */
extern unsigned short path_Goal;            /* 0x1460ab4 */
extern unsigned short path_Start;           /* 0x1460ab6 */
extern float path_Tab2Sel;                  /* 0x1460ab8, path_Tab2[type], written per search, never read */
extern unsigned short path_Target;          /* 0x1460abc */
extern unsigned short path_Meet;            /* 0x1460abe */
extern float path_Weight;                   /* 0x1460ac0 */
extern int path_Mode;                       /* 0x633184, 1 = one-directional search */

unsigned short path_Find(PATHOUT *out, PMAP *map, int sx, int sy, int ex, int ey, int type); /* 0x424f30 */
unsigned short path_Search2(PMAP *map, int sx, int sy, short ex, int ey, int type);       /* 0x4250a0, bidirectional */
unsigned short path_Search1(PMAP *map, int sx, int sy, short ex, int ey, int type);       /* 0x425470 */
unsigned short path_Expand(PMAP *map, unsigned short n);                                    /* 0x425780 */
void path_Reverse(PMAP *map, unsigned int n);                                           /* 0x425af0 */

#endif

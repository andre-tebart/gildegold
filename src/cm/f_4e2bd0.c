/* 0x4e2bd0, the window call 0x475590 of the scene helpers; in the replay mode (0x6f0d8c) the second argument is 0 */

/* the scene party records (0x925520, 96 of 0x114 bytes: +0 id (-1 free), +8 kind, +0x14 the thing, +0x26 flags, +0x30 used) */
typedef struct G28REC {
    int id;
    char pad04[4];
    unsigned char kind;                 /* 0x08 */
    char pad09[0xb];
    int thing;                          /* 0x14 */
    char pad18[0xe];
    unsigned char flags;                /* 0x26 */
    char pad27[9];
    unsigned char used;                 /* 0x30 */
    char pad31[0x114 - 0x31];
} G28REC;
extern G28REC ai_Unk925520[96];
/* ai_Unk925538: view of ai_Unk925520 (tools/data_views.py) */

/* the random seed of the scenes (0x92c4e0), a linear congruence; the number is the high word modulo 0x7fff */
#define G28_HI (*(unsigned short *)((char *)&ai_Unk92c4e0 + 2))  /* 0x92c4e2 */


int ai_Unk4e2bd0(int a, int b)
{
    if (ai_Unk6f0d8c != 0)
        b = 0;
    return ((int (*)(int, int))d2_SetViewEnabled)(a, b);
}

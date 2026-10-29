/* 0x500080, an object instance of the prototype kind 0x1d frees the record at +0x29 (0x4bd9e0) */

extern short ai_Unk61a674, ai_Unk61a676;/* 0x61a674 / 0x61a676, the query: the words +0x25 / +0x27 (0x7fff = any) */
extern char ai_Unk6f1bd4, ai_Unk6f1bd5; /* 0x6f1bd4, 0x6f1bd5, stop at the first, first call */
char * ai_Unk6f1bf0 = 0;  /* 0x6f1bf0, the current building */
extern unsigned ai_Unk889400[732];
extern unsigned short ai_Unk76fdc4;
void rt_FreePoints();                     /* 0x4bd9e0 */
/* 0x500190 */
int ai_Unk5a86c0();                     /* 0x5a86c0 */

void gm_FreeObjectExtra(unsigned short *o)
{
    if (ai_Goods[*o * 0x41] == 0x1d && *(int *)((char *)o + 0x29) != 0) {
        rt_FreePoints(*(int *)((char *)o + 0x29));
        *(int *)((char *)o + 0x29) = 0;
    }
}

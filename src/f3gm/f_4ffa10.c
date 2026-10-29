/* 0x4ffa10, the price of the goods record n of the player p (0x80 bytes each from 0x7d6a70 + p * 0x2a90): the base price 0x5087f0 (id, 100) when the player has no market object (0x500ec0, or for another player the object of 0x501000 with the group (2, 0, 0x1db, 6)); else the market's stock (the int at +0xe of the goods object of the market object (+0x14 list, id)) against the record's wanted amount (+0x20) and mean (+0x24) gives a factor limited to 0.25 .. 5.0, the price moves 0.4 of the way from the stored price (+0x38) to base * factor and is at least 512.0 (or the base when that is above 512). ROUGH */

int ai_Unk61a67c = 32767;  /* 0x61a67c, the query: object prototype ( */
int ai_Unk61a680 = -1;  /* 0x61a680, the query: owner id (-1 = any) */
short ai_Unk61a684 = -1;  /* 0x61a684, the query: */
#define ALIGN1 __declspec(align(1))
ALIGN1 char ai_Unk61a686[550] = { 255, 0, 0, 0, 0, 0, 0, 0, 105, 105, -25, -25, 105, 4, 63, 63, -67, -67, 63, 3, 63, 63, 126, 126, 42, 2, 63, 42, 84, 84, 21, 2, 63, 21, 84, 84, 21, 1, 42, 21, 84, 84, 21, 1, -109, 105, -109, 105, -67, 4, 105, 63, 105, 63, -109, 3, 105, 42, 84, 63, 105, 2, 105, 21, 63, 42, 63, 2, 105, 21, 63, 21, 63, 1, 84, 21, 63, 21, 63, 1, 105, 126, -109, 105, -46, 4, 63, 84, 105, 63, -88, 3, 63, 63, 84, 63, 126, 2, 63, 63, 63, 21, 84, 2, 42, 63, 63, 21, 84, 1, 21, 63, 63, 21, 84, 1, -46, -88, 105, 105, 105, 4, -88, 126, 63, 63, 63, 3, 126, 105, 63, 42, 63, 2, 84, 84, 63, 21, 42, 2, 84, 84, 63, 21, 21, 1, 84, 84, 42, 21, 21, 1, -25, 105, 105, 105, -109, 4, -67, 63, 63, 63, 105, 3, -109, 63, 63, 42, 84, 2, 126, 21, 42, 42, 63, 2, 126, 21, 42, 42, 42, 1, 105, 21, 42, 42, 42, 1, 84, 105, 63, 84, 42, 2, 84, 105, 63, 84, 42, 2, 84, 105, 63, 84, 42, 2, -88, -67, 105, 105, 126, 4, 126, -109, 63, 63, 84, 3, 105, 105, 63, 63, 63, 2, 84, 105, 42, 21, 42, 2, 84, 105, 21, 21, 42, 1, 84, 84, 21, 21, 42, 1, 105, 105, -88, -46, 105, 4, 63, 63, 126, -88, 63, 3, 63, 63, 105, 126, 42, 2, 42, 21, 84, 126, 21, 2, 42, 21, 84, 105, 21, 1, 42, 21, 84, 84, 21, 1, -88, -46, 105, 105, 105, 4, 126, -88, 63, 63, 63, 3, 105, 126, 63, 63, 42, 2, 84, 126, 42, 21, 21, 2, 84, 126, 21, 21, 21, 1, 84, 105, 21, 21, 21, 1, 105, 105, -88, -46, 105, 4, 63, 63, 126, -88, 63, 3, 63, 63, 105, 126, 42, 2, 63, 21, 105, 84, 21, 2, 42, 21, 105, 84, 21, 1, 42, 21, 84, 84, 21, 1, -88, -46, 105, 105, 105, 4, 126, -88, 63, 63, 63, 3, 105, 126, 63, 63, 42, 2, 84, 126, 42, 21, 21, 2, 84, 126, 21, 21, 21, 1, 84, 105, 21, 21, 21, 1, -88, -46, 105, 105, 105, 4, 126, -88, 63, 63, 63, 3, 105, 126, 63, 63, 42, 2, 84, 126, 42, 21, 21, 2, 84, 126, 21, 21, 21, 1, 84, 105, 21, 21, 21, 1, -67, -88, 105, 105, 126, 4, -109, 126, 63, 63, 84, 3, 105, 105, 63, 42, 84, 2, 84, 105, 42, 21, 42, 2, 84, 84, 42, 21, 42, 1, 84, 84, 21, 21, 42, 1, -109, -109, -67, 105, 105, 4, 105, 105, -109, 63, 63, 3, 84, 84, 105, 42, 63, 2, 63, 84, 84, 21, 21, 2, 63, 63, 84, 21, 21, 1, 63, 63, 84, 21, 21, 1, 0, 0, 0, 0, 0, 1, -67, -109, -109, 105, 105, 4, -109, 105, 105, 63, 63, 3, 105, 84, 84, 42, 63, 2, 84, 63, 63, 21, 21, 2, 84, 63, 63, 21, 21, 1, 84, 63, 63, 21, 21, 1, -88, -67, 105, 105, 126, 4, 126, -109, 63, 63, 84, 3, 105, 105 };  /* 0x61a686, the query: prototype kind (-1 = any); the
                                   table at +2 is ai_Unk61a688 (the same storage, one block as in the original) */
extern char ai_Unk6f1be0, ai_Unk6f1be8, ai_Unk6f1be9, ai_Unk6f1bea;  /* 0x6f1be0, 0x6f1be8, 0x6f1be9, 0x6f1bea, first-step flag, scan all, descend into children, stop at the first */
int ai_Unk6f1be4 = 0;  /* 0x6f1be4, the number of records scanned */
unsigned short * ai_Unk6f1bf4 = 0;  /* 0x6f1bf4, the current record */
int ai_Unk7e3f38;  /* 0x7e3f38, the depth of the child stack */
int ai_Unk8753e0[20488];  /* 0x8753e0, the child stack (the first word of */
int gm_FindMarketObject(int id);                     /* 0x500ec0 */
int gm_FindPlayerBuilding();                     /* 0x501000 */

float gm_Unk4ffa10(int p, int n)
{
    char *rec;
    int off;
    unsigned id;
    char *pr;
    int mk;
    int m2;
    float stock, cur;
    double f;
    float r;

    off = p * 0x2a90 + n * 0x80;
    id = *(unsigned short *)(init_StockBlocks + n * 0x80 + p * 0x2a90);
    pr = ai_Goods + id * 0x41;
    mk = gm_FindMarketObject(id);
    if (mk == 0 && p != ai_OwnPlayer && (m2 = gm_FindPlayerBuilding(p)) != 0)
        mk = (int)gm_Query(*(int *)(m2 + 0x5d), 2, 0, 0x1db, 6);
    stock = 0.0f;
    if (mk == 0) {
        r = gm_ItemPrice(id, 100);
    } else {
        m2 = (int)gm_Query(*(int *)(mk + 0x14), 1, 0, id);
        if (m2 != 0)
            stock = (float)*(int *)(m2 + 0xe);
        cur = *(float *)(ai_Unk7d6aa8 + off);
        if (stock <= *(float *)(ai_Unk7d6a90 + off) || *pr == 0x17)
            f = 1.0;
        else if (1.0 <= stock)
            f = (double)*(float *)(ai_Unk7d6a90 + off) / (double)stock;
        else
            f = (double)*(float *)(ai_Unk7d6a90 + off) / 1.0;
        f = f * ((double)*(float *)(ai_Unk7d6a94 + off) / (double)*(float *)(ai_Unk7d6a90 + off));
        if (f <= 0.25)
            f = 0.25;
        if (5.0 <= f)
            f = 5.0;
        r = (float)(((float)f * gm_ItemPrice(id, 100) - cur) * 0.4 + *(float *)(ai_Unk7d6aa8 + off));
        if (*(float *)(ai_Unk7d6aa8 + off) <= 0.0f) {
            r = gm_ItemPrice(id, 100);
            if (512.0 <= r)
                return gm_ItemPrice(id, 100);
            return 512.0f;
        }
    }
    return r;
}

/* 0x50c550, how much the person a likes the person b, 0 .. 100 (100 for the same person, 50 when one of them is unknown (0xffff)): the sum of the liking of a's two bound persons (0x5092e0 of the words +0x170 and, for a person with a kind 4 spouse at +0xce6, of 0x50b010(1), and of the guild head of the faith byte 0x1e .. 0x21 -> the persons 0x17 .. 0x1a), the stored mutual rating of the two (0x7e5360 matrix, scaled to 0 .. 100), the best building of the class 0x21 the person a lives in (the byte +0x247, up to 3), the rating of 0x4e5e50, the bonuses per kind of b's job group (0x5ba6b0 of the goods 0x155 .. 0x166: 10 / 7 / 8 / 15 times the count), the kinds 0x2b6 (+5 when b has a spouse, +5 when b has the flags 0x3e00), the owner link (0x16b, times 7), minus 3 per level of the bits 14..16 (a different faith byte) and bits 23..24 of a's flags +0x1ac. ROUGH: converted from the pseudocode, the long float expression is lost in part */

int gm_PlayerRecords[688] = {0};  /* 0x7d5f80, the 16 player records ( */
int ai_Unk7d6a40 = 0;  /* 0x7d6a40, the number of family records */
char ai_Unk7d6a60[0xd4d0] = {0}; /* 0x7d6a60, the five stock blocks (five rows of four header ints plus 0x55 records of 0x80 bytes); a tentative definition, so the COMMON linker reserves the whole span and no other global lands inside it. init_StockBlock is the same object (data.csv alias). */
char * ai_Objects = 0;  /* 0x7e3f34, the object instances ( */
int ai_Unk7e3f38 = 0;  /* 0x7e3f38, the depth of the child stack */
int gm_NOffers = 0;  /* 0x7e3f3c, the number of offers */
int gm_Offers[528] = {0};  /* 0x7e4b20, the offers (6 ints each: building, market, goods object, supply, price float, flag) */
int ai_Unk875360[32] = {0};  /* 0x875360 */
int ai_Unk8753e0[20488] = {0};  /* 0x8753e0, the child stack (the first word of */
unsigned ai_Unk889400[732] = {0};  /* 0x889400 */
int ai_Unk889f70 = 0;  /* 0x889f70 */
char * ai_Blds = 0;  /* 0x889f74, the buildings ( */
int ai_Unk919f80[732] = {0};  /* 0x919f80 */
char * ai_Goods = 0;  /* 0x91aaf0, the object prototypes ( */
unsigned ai_Unk91ac40[136] = {0};  /* 0x91ac40 */
char gm_CharacterStatistics[128] = {0};  /* 0x91b500, the character statistics ( */
char ai_Unkf8ef80[256] = {0}; /* 0xf8ef80, the game root path */
float gm_Liking(unsigned a, unsigned b, int c)
{
    float sum = 0.0f;
    int ra, rb, k;
    char m;
    unsigned char best = 0, kind;
    unsigned i;
    unsigned char *bp;
    char *cls;
    unsigned char sp[16];
    float f;
    unsigned fl;
    int w;
    unsigned char *ca = (unsigned char *)ai_Chars[a];
    unsigned char *cb = (unsigned char *)ai_Chars[b];

    if (a == b)
        return 100.0f;
    if (a == 0xffff || b == 0xffff)
        return 50.0f;
    if (*(int *)(ca + 0x170) != 0)
        sum = (float)gm_BuildingSkill(*(int *)(ca + 0x170), 4, 1);
    if (ca[0x166] != 0) {
        char rec[0x20];

        amt_GetKind(ca[0x166], rec);
        if (*(int *)(rec + 4) == 4) {
            ra = gm_FirstBuilding(1);
            if (ra != 0)
                sum = (float)gm_BuildingSkill(ra, 4, 1) + sum;
        }
    }
    kind = ca[0x169];
    if (kind > 0x1d && kind < 0x22) {
        int g = 0;

        switch (kind) {
        case 0x1e:
            g = 0x17;
            break;
        case 0x1f:
            g = 0x18;
            break;
        case 0x20:
            g = 0x19;
            break;
        case 0x21:
            g = 0x1a;
        }
        ra = gm_BuildQuery(1, 5, g);
        if (ra != 0 && (*(unsigned char *)(ra + 0x5a) & 1) == 0)
            sum = (float)gm_BuildingSkill(ra, 4, 1) + sum;
    }
    m = *((char *)(unsigned)&ai_Relations + b + a * 0x300);
    i = 0;
    bp = (unsigned char *)ai_Blds;
    do {
        if (*bp != 0 && *(short *)(bp + 0x27) == *(short *)ai_Chars[a]) {
            cls = ai_BTypes + (unsigned)*bp * 0x24d;
            if (*cls == '!' && best < (unsigned char)cls[0x247])
                best = cls[0x247];
            if (best > 2)
                break;
        }
        i++;
        bp += 0xa9;
    } while (i < 0x100);
    f = (float)amt_Rating(ai_Chars[a], ai_Chars[b], best, c);
    sum = f + ((float)(m + 0x7f) * 0.003921569f * 100.0f + sum);
    if (cb[0x166] != 0) {
        unsigned char jk;
        char rec[0x20];

        amt_GetKind(cb[0x166], rec);
        jk = *(unsigned char *)(rec + 8);
        if (jk == 0 || jk > 3) {
            if (jk < 4 || jk > 7) {
                if (jk >= 8 && jk <= 9) {
                    sum += (float)ai_InventoryHas(ai_Chars[a], 0x165) * 7.0f + (float)ai_InventoryHas(ai_Chars[a], 0x159) * 7.0f;
                    sum += (float)ai_InventoryHas(ai_Chars[a], 0x166) * 15.0f;
                }
            } else {
                sum += (float)ai_InventoryHas(ai_Chars[a], 0x157) * 10.0f;
                sum += (float)ai_InventoryHas(ai_Chars[a], 0x15f) * 8.0f;
            }
        } else {
            sum += (float)ai_InventoryHas(ai_Chars[a], 0x155) * 10.0f;
            sum += (float)ai_InventoryHas(ai_Chars[a], 0x15d) * 10.0f;
        }
    }
    w = ai_InventoryHas(ai_Chars[a], 0x2b6);
    if (w != 0) {
        if (cb[0x166] != 0)
            sum += 5.0f;
        if ((*(unsigned *)(cb + 0x1cc) & 0x3e00) != 0)
            sum += 5.0f;
    }
    if (*(int *)(cb + 0x16c) != 0 && *(unsigned short *)(*(int *)(cb + 0x16c) + 0x27) == a)
        sum += (float)ai_InventoryHas(ai_Chars[a], 0x16b) * 7.0f;
    fl = *(unsigned *)(ca + 0x2c) >> 0xe & 7;
    if (fl != 0 && ca[9] != cb[9])
        sum -= (float)(fl * 3);
    fl = *(unsigned *)(ca + 0x2c) >> 0x17 & 3;
    if (fl != 0)
        sum -= (float)(fl * 3);
    if (sum < 100.0f) {
        if (0.0f < sum)
            return sum;
        return 0.0f;
    }
    return 100.0f;
}

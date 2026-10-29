/* 0x509c80, the worth of the building b's owner: the value of the character's objects (0x509e60) for a person, plus (with flag) the prices of the building's first kind 0x2a object's goods (0x5089c0 for the player 0x6f1bc5) */

int gm_OwnerWorth(char *b, int flag)
{
    unsigned short id;
    int v;
    char *l;
    unsigned short *o;

    v = 0;
    id = *(unsigned short *)(b + 0x27);
    if (id != 0xffff && (unsigned char)ai_Chars[id][2] < 10)
        v = gm_GetGoodsValue(ai_Chars[id]);
    if (flag != 0 && (l = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x2a)) != 0) {
        o = (unsigned short *)gm_Query(*(void **)(l + 0x14), 1, 5);
        while (o) {
            v = (int)gm_GoodsPrice(*o, ai_OwnPlayer);
            o = (unsigned short *)gm_QueryNext();
        }
    }
    return v;
}

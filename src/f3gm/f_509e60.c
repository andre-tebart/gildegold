/* 0x509e60, the amount of the character c's objects of the prototype word of the player 0x6f1bc5 */

int gm_GetGoodsValue(char *c)
{
    int s;
    char *o;

    s = 0;
    o = gm_Query(*(void **)(c + 0x178), 1, 0, PLW54(ai_OwnPlayer));
    while (o) {
        s += *(int *)(o + 0xe);
        o = (char *)gm_QueryNext();
    }
    return s;
}

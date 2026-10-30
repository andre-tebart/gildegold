/* 0x4b48f0, mission condition: the share (in percent) of the characters (kind < 10) with c's byte 0x0c
   reaches t->goal */

typedef struct G13STAT {                /* 0x0c bytes, filled by 0x4b51f0 */
    int same;                           /* 0x00, the holders that match (the fillers' local same) */
    int n;                              /* 0x04 */
    float v;                            /* 0x08 */
} G13STAT;

void ai_CharBuildingShare(AICHAR *c, int k, G13STAT *s);                                 /* 0x4b51f0 */

int ai_CondCharShare(AICHAR *c, G13TYPE *t)
{
    int i, n, same;

    n = 0;
    same = 0;
    for (i = 0; i < 0x300; i++) {
        if (ai_CharsT[i].kind < 10) {
            n++;
            if (G13_B(&ai_CharsT[i], 0x0c) == G13_B(c, 0x0c))
                same++;
        }
    }
    if ((float)same / n * 100.0f >= t->goal)
        return 1;
    return 0;
}

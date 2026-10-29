/* 0x4b4af0, mission condition: values 0x15, 0x14, 0x12, 0x16, 8 and 9 of 0x4b5270 all reach 1 (where present)
   and their counts add up to t->goal */

void ai_PlayerShare(int k, G13STAT *s);                                            /* 0x4b5270 */

int ai_PlayerHeldBuildings(AICHAR *c, G13TYPE *t)
{
    G13STAT s;
    int n;

    ai_PlayerShare(0x15, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    n = s.same;
    ai_PlayerShare(0x14, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    n += s.same;
    ai_PlayerShare(0x12, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    n += s.same;
    ai_PlayerShare(0x16, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    n += s.same;
    ai_PlayerShare(8, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    n += s.same;
    ai_PlayerShare(9, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    return s.same + n >= t->goal;
}

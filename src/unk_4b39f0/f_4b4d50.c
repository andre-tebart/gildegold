/* 0x4b4d50, mission condition: value 0 of 0x4b5270 reaches t->goal percent */

int ai_CondPlayerShare(AICHAR *c, G13TYPE *t)
{
    G13STAT s;

    ai_PlayerShare(0, &s);
    if (t->goal * 0.01f <= s.v)
        return 1;
    return 0;
}

/* 0x4b4c50, mission condition: 0x4b5160 with "values 0x0b, 0x0c and 0x0d of 0x4b5270 reach 1 (where present)
   and t->goal <= 3" */

int ai_Unk4b4c50(AICHAR *c, int x, G13TYPE *t)
{
    G13STAT s;

    ai_PlayerShare(0x0b, &s);
    if (s.n > 0 && s.v < 1.0f)
        return ai_TimedCondition(x, 0, t->unk18);
    ai_PlayerShare(0x0c, &s);
    if (s.n > 0 && s.v < 1.0f)
        return ai_TimedCondition(x, 0, t->unk18);
    ai_PlayerShare(0x0d, &s);
    if (s.n > 0 && s.v < 1.0f)
        return ai_TimedCondition(x, 0, t->unk18);
    return ai_TimedCondition(x, t->goal <= 3, t->unk18);
}

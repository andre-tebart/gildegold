/* 0x4b45c0, mission condition: 0x4b5160 with c's byte 0x0d reaching t->goal */

int ai_Unk4b45c0(AICHAR *c, int x, G13TYPE *t)
{
    return ai_TimedCondition(x, G13_B(c, 0x0d) >= t->goal, t->unk18);
}

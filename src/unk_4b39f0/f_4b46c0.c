/* 0x4b46c0, mission condition: 0x4b5160 with c's byte 0x166 equal to t->goal */

int ai_Unk4b46c0(AICHAR *c, int x, G13TYPE *t)
{
    return ai_TimedCondition(x, G13_B(c, 0x166) == t->goal, t->unk18);
}

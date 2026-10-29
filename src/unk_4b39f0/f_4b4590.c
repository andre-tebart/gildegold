/* 0x4b4590, mission condition: 0x504ce0 of c's byte 0x164 reaches t->goal */

int ai_Unk4b4590(AICHAR *c, G13TYPE *t)
{
    return gm_TitlesLeft(G13_B(c, 0x164)) >= t->goal;
}

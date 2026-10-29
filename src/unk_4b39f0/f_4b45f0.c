/* 0x4b45f0, mission condition: 0x50a640 of c (as money, 0x5085b0) reaches t->goal */

int ai_Unk4b45f0(AICHAR *c, G13TYPE *t)
{
    return gm_ConvertMoney(gm_GetWorth(c->id), ai_OwnPlayer) >= t->goal;
}

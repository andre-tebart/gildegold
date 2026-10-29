/* 0x4b4630, mission condition: 0x509ee0 of c (as money, 0x5085b0) reaches t->goal */

int ai_Unk4b4630(AICHAR *c, G13TYPE *t)
{
    return gm_ConvertMoney(gm_Unk509ee0((void *)c, ai_OwnPlayer), ai_OwnPlayer) >= t->goal;
}

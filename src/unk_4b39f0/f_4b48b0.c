/* 0x4b48b0, the same with value 0x3c */

int ai_Unk4b48b0(AICHAR *c, G13TYPE *t)
{
    AIFIN *f = gm_PlayerRecord(c);

    if (f == 0)
        return 0;
    return gm_ConvertMoney(*(int *)((char *)f + 0x3c), ai_OwnPlayer) > t->goal;
}

/* 0x4b4870, the same with value 0x30 */

int ai_Unk4b4870(AICHAR *c, G13TYPE *t)
{
    AIFIN *f = gm_PlayerRecord(c);

    if (f == 0)
        return 0;
    return gm_ConvertMoney(*(int *)((char *)f + 0x30), ai_OwnPlayer) > t->goal;
}

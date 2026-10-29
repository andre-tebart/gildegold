/* 0x4b4550, mission condition: the value of what 0x506430 finds for c is above t->goal */

int ai_Unk4b4550(AICHAR *c, G13TYPE *t)
{
    AIFIN *f = gm_PlayerRecord(c);

    if (f == 0)
        return 0;
    return gm_ConvertMoney(*(int *)((char *)f + 0x38), ai_OwnPlayer) > t->goal;
}

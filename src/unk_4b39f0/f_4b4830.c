/* 0x4b4830, mission condition: values 0x2c + 0x34 of what 0x506430 finds for c are above t->goal */

int ai_Unk4b4830(AICHAR *c, G13TYPE *t)
{
    AIFIN *f = gm_PlayerRecord(c);

    if (f == 0)
        return 0;
    return gm_ConvertMoney(*(int *)((char *)f + 0x2c) + *(int *)((char *)f + 0x34), ai_OwnPlayer) > t->goal;
}

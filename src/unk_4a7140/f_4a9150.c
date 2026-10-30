/* 0x4a9150, eval callback: p (unk194 > 7) on its day (id & 3 = day % 4), one in four */

int ai_Unk4a9150(int a, AIPER *p)
{
    if (p->unk194 >= 8 && ai_TimeT.year % 4 == (p->netId & 3) && util_RandomU(4) == 0)
        return 1;
    return 0;
}

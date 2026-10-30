/* 0x4a8c20, eval callback: on p's day (id & 3 = day % 4, id & 7 = the hour & 7), one in four */

int ai_Unk4a8c20(int a, AIPER *p)
{
    if ((p->netId & 3) == ai_TimeT.year % 4 && (p->netId & 7) == (ai_TimeT.hour & 7))
        return util_RandomU(4) == 0;
    return 0;
}

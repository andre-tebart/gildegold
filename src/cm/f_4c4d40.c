/* 0x4c4d40, message 0x1e with the time t; -1: t is not valid (0x4fe210) */

int ai_Unk4c4d40(char *t)
{
    G17MSG m;

    if (gm_CompareTime(t, ai_Time) == 0)
        return -1;
    *(G17T14 *)m.data = *(G17T14 *)t;
    m.type = 0x1e;
    return ai_Unk4c39c0(&m);
}

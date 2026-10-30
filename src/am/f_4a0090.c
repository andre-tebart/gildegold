/* 0x4a0090, decide callback 0x37: when 0x4e94e0 allows it and the character can pay a price of
   max(0x50a640 * 0.01, 160) (at most 22 % of its money), a building (0x4a63f0 kind 6) whose object list some
   character points to */

int ai_Unk4a0090(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    LARGE_INTEGER m;
    int v, n, b, y, i;

    if (f != 0)
        return 0;
    if (a != 0)
        return 0;
    if (amt_CanUseMayorPower2(p) != 1)
        return 0;
    v = (int)max(gm_GetWorth(p->id) * 0.01f, 160.0f);
    if (v > (int)(gm_Unk509ee0((AIPER *)p, ai_OwnPlayer) * 0.22f))
        return 0;
    m.LowPart = 0x807d4190;
    m.HighPart = 1;
    if (!ai_Unk4a63f0((AIPER *)p, 6, m.QuadPart, 0, (float)(util_Random(0x20) + 0x20), &b))
        return 0;
    gm_FindById((int)&y, 0, 0, b);
    if (y == 0)
        return 0;
    n = 0;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].bld == (AIBLD *)y)
            n++;
    }
    if (n == 0)
        return 0;
    o1->unk10 = v;
    o1->kind = 4;
    o1->unk04 = b;
    return 0x37;
}

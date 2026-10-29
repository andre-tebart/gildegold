/* 0x490690, decide of "Graveyard Owner" (0x3d) */

int ai_Unk490690(int a, AIPX *p, int b, AIARG *o1, AIARG *o2)
{
    AIARG s1[4], s2[4];
    int r[4] = { 0, 0, 0, 0 };
    int v, x, i, n, fl = 0x20000000;

    if (b)
        return 0;
    if (a != 0x3d && a != 0)
        return 0;
    if (p->kind == 3)
        return 0;
    if (!AI_5044B0_US(p->id, 0xd))
        return 0;
    if (!AI_4A63F0_7(p, 4, fl, 0, 0, 100.0f, &x))
        return 0;
    AI_4FEA50_4(&v, 0, 0, x);
    if (!v)
        return 0;
    r[0] = ai_Unk48ff50(p, (AIBLD *)v, &s1[0], &s2[0]);
    r[1] = ai_Unk490170(p, (AIBLD *)v, &s1[1], &s2[1]);
    r[2] = ai_Unk490350(p, (AIBLD *)v, &s1[2], &s2[2]);
    r[3] = ai_Unk490570(p, (AIBLD *)v, &s1[3], &s2[3]);
    i = util_Random(4);
    for (n = 4; n; n--) {
        if (r[i]) {
            *o1 = s1[i];
            *o2 = s2[i];
            return 0x3d;
        }
        i = (i + 1) % 4;
    }
    return 0;
}

/* 0x49dd90, decide callback 0x15 (characters with +0x166 == 0xf): up to four tries to find a good to sell
   (as 0x499400 in am.c) */

int ai_Unk49dd90(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    AIBLD *y;
    LARGE_INTEGER m;
    int n, x, w, z, r;

    if (f != 0 || ((unsigned char *)p)[0x166] != 0xf || o1->kind != 0)
        return 0;
    if (a != 0 && ((unsigned char *)p)[0x130] != 6 && ((unsigned char *)p)[0x130] != 9)
        return 0;
    for (n = 4; n != 0; n--) {
        m.LowPart = 2;
        m.HighPart = 0;
        if (!ai_Unk4a63f0((AIPER *)p, 2, m.QuadPart, 0, 100.0f, &x))
            continue;
        gm_FindById((int)&y, 0, 0, x);
        if (y == 0)
            continue;
        if (!ai_Unk4a5540(&w, &z, y, 0))
            continue;
        s1.kind = 2;
        s1.unk04 = w;
        s1.unk08 = 1;
        if (z != -1) {
            s2.kind = 1;
            s2.unk04 = z;
        } else {
            s2.kind = 4;
            s2.unk04 = y->id;
        }
        s2.unk08 = 1;
        s2.unk14 = 0.44f;
        r = ai_Unk495a50(0x15, p->id, 1, &s1, &s2);
        if (r != 0) {
            *o1 = s1;
            *o2 = s2;
            return r;
        }
    }
    return 0;
}

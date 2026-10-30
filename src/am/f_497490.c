/* 0x497490, decide callback */

int ai_Unk497490(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int list[6];
    AIARG s2;
    AIARG s1;
    int r, k;

    if (f == 0) {
        if (p->unk00d < 2) {
            s1.kind = 13;
            s1.unk04 = 2;
            s1.unk08 = 1;
            s1.unk14 = 0.44f;
            memset(&s2, 0, sizeof(s2));
            r = ai_Unk495a50(10, p->id, 1, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
                return r;
            }
        } else if (ai_TimeHour > 12 && p->unk168 == 0 && unk_Unk4fc7a0(p) - p->unk00a > 2) {
            k = amt_GetOfficeCandidates(p, 2, list);
            if (k == 0) {
                if (util_RandomU(4) == 0) {
                    s2.kind = 0x10;
                    s2.unk04 = 0x12;
                    s2.unk08 = 0;
                    if (ai_Unk4a9a00(a, p, 0, &s2, &s1) != 0) {
                        *o1 = s2;
                        *o2 = s1;
                        return 0x27;
                    }
                }
            } else {
                r = list[util_RandomU(k) * 3];
                if (r != 0 && r < 0x1c) {
                    o1->unk04 = r;
                    o1->kind = 10;
                    return 10;
                }
            }
        }
    }
    return 0;
}

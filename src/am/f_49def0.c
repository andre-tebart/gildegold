/* 0x49def0, decide callback 0x31 (characters with +0x166 == 0xf): for a building found by 0x4a63f0 (mask
   0x8000): work on it (+0x5c low), sell its goods, or upgrade it (type 0x24 -> 0x25, 0x25 -> 0x26) */

int ai_Unk49def0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    AIBLD *y;
    AIOBJ *o;
    LARGE_INTEGER m;
    float d[10];
    int x, w, z, r, money, act, v;
    char lvl;

    if (f != 0 || ((unsigned char *)p)[0x166] != 0xf || o1->kind != 0)
        return 0;
    if (a != 0 && ((unsigned char *)p)[0x130] != 7 && ((unsigned char *)p)[0x130] != 6)
        return 0;
    m.LowPart = 0x8000;
    m.HighPart = 0;
    if (!ai_Unk4a63f0((AIPER *)p, 2, m.QuadPart, 0, 100.0f, &x))
        return 0;
    gm_FindById((int)&y, 0, 0, x);
    if (y == 0)
        return 0;
    o = gm_Query(y->objList, 2, 6, 0, 0x115);
    money = gm_Unk509e30(*(int *)((char *)o + 0x14), ai_OwnPlayer);
    if (!(y->flags5a & 4) && y->level < util_Random(0x40) + 0x25) {
        s1.kind = 4;
        s1.unk04 = y->id;
        s1.unk08 = 1;
        lvl = y->level;
        if (lvl < 0x21)
            s1.unk10 = 0x32;
        else
            s1.unk10 = lvl >= 0x42 ? 10 : 30;
        s2.kind = 1;
        s2.unk04 = *(int *)((char *)o + 2);
        s2.unk08 = money;
        s2.unk10 = 1;
        r = ai_Unk495a50(0xf, p->id, 0, &s1, &s2);
        if (r != 0)
            goto done;
    }
    if (ai_Unk4a5540(&w, &z, y, 0)) {
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
        if (r != 0)
            goto done;
    }
    if (y->flags5a & 0x41)
        return 0;
    unk_Unk4f9e00(d);
    if (y->type == 0x24) {
        act = 0x25;
        if (util_Random(0x200) + d[6] < 1250.0f)
            return 0;
    } else if (y->type == 0x25) {
        act = 0x26;
        if (util_Random(0x400) + d[6] < 5000.0f)
            return 0;
    } else
        return 0;
    v = gm_BuildingKindValue(act);
    if (money < v * 0.4f)
        return 0;
    o1->kind = 4;
    o1->unk04 = act;
    o1->unk08 = 1;
    o1->unk10 = 0;
    o2->kind = 4;
    o2->unk10 = 0;
    o2->unk04 = y->id;
    o2->unk08 = 1;
    return 0x31;
done:
    *o1 = s1;
    *o2 = s2;
    return r;
}

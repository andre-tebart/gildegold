/* 0x49d8d0, decide callback 0x2e (characters with +0x166 == 0xf): buy a building of kind 1 (type 2) or
   kind 1 (type 3) when 0x4f9e00 is above the value +0x100, else work on the weakest building (+0x5c < 0x50) */

int ai_Unk49d8d0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *best, *recA, *recB, *r, *b, *rec;
    AIOBJ *o;
    unsigned short fl;
    unsigned char k;
    char lvl;
    int cntA, cntB, n, money, v;
    AIARG s1;
    AIARG s2;
    float d[10];

    best = 0;
    recA = 0;
    recB = 0;
    cntA = 0;
    cntB = 0;
    if (a != 0 || f != 0 || ((unsigned char *)p)[0x166] != 0xf)
        return 0;
    b = gm_FirstBuilding(1);
    if (b == 0)
        return 0;
    o = gm_Query(b->objList, 2, 6, 0, 0x115);
    money = gm_Unk509e30(*(int *)((char *)o + 0x14), ai_OwnPlayer);
    unk_Unk4f9e00(d);
    rec = ai_Blds;
    for (n = 0x100; n != 0; n--, rec++) {
        k = rec->type;
        if (k == 0 || ai_BTypes[k].kind != 1)
            continue;
        fl = *(unsigned short *)&rec->flags5a;
        if (fl & 0x45)
            continue;
        lvl = rec->level;
        if (lvl < 0x50 && (best == 0 || best->level > lvl))
            best = rec;
        if (k == 3)
            continue;
        if (k == 2) {
            cntB++;
            if (!(fl & 0x45))
                recB = rec;
        } else {
            cntA++;
            if (!(fl & 0x45))
                recA = rec;
        }
    }
    if ((*(float *)((char *)p + 0x100) - 2.0) * 0.4f < d[3]) {
        if (cntA != 0 && recA != 0) {
            r = recA;
            o1->kind = 4;
            o1->unk04 = 2;
            v = gm_BuildingKindValue(2);
        } else if (cntB != 0 && recB != 0) {
            r = recB;
            o1->kind = 4;
            o1->unk04 = 3;
            v = gm_BuildingKindValue(3);
        } else {
            s1.kind = 5;
            s1.unk04 = 1;
            v = gm_BuildingKindValue(1);
            if (money < v * 1.2f)
                return 0;
            s1.unk08 = 1;
            s2.kind = 0;
            s2.unk04 = 0;
            s2.unk08 = 0;
            if (ai_Unk4a5140(0, (AIPER *)p, 1, &s1, &s2, 1) == 0)
                return 0;
            *o1 = s1;
            *o2 = s2;
            return 0x2e;
        }
        if (money < v * 0.4f)
            return 0;
        o1->unk08 = 1;
        o2->kind = 4;
        o2->unk04 = r->id;
        o2->unk08 = 1;
        return 0x2e;
    }
    if (best == 0)
        return 0;
    lvl = best->level;
    if (lvl - util_Random(0x20) >= 0x42)
        return 0;
    s1.unk04 = best->id;
    s1.kind = 4;
    s1.unk08 = 1;
    if (lvl < 0x21)
        s1.unk10 = 0x32;
    else
        s1.unk10 = lvl >= 0x42 ? 10 : 30;
    s2.unk04 = *(int *)((char *)o + 2);
    s2.unk08 = money;
    s2.unk10 = 1;
    s2.kind = 1;
    v = ai_Unk495a50(0xf, p->id, 0, &s1, &s2);
    if (v == 0)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return v;
}

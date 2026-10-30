/* 0x49e7a0, decide callback 0x2f (characters with +0x166 == 0xa, from day 4 on, rich enough): work
   on a building of kind 3, else build type 9 / 10 (1 in 8), else what 0x4a5140 finds */

int ai_Unk49e7a0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *recA, *recB, *b, *rec;
    unsigned char *o;
    unsigned char k;
    char lvl;
    int cntA, cntB, i, money, r;
    AIARG s1;
    AIARG s2;

    recA = 0;
    recB = 0;
    cntA = 0;
    cntB = 0;
    if (ai_TimeT.year < 4 || f != 0 || ((unsigned char *)p)[0x166] != 0xa || a != 0)
        return 0;
    b = gm_FirstBuilding(1);
    if (b == 0)
        return 0;
    o = (unsigned char *)gm_Query(b->objList, 2, 6, 0, 0x115);
    money = gm_Unk509e30(*(int *)(o + 0x14), ai_OwnPlayer);
    if (money < (util_RandomU(3000) + 1000) * 32)
        return 0;
    rec = ai_BldsT;
    for (i = 0; i < 0x100; i++, rec++) {
        k = rec->type;
        if (k == 0 || ai_BTypesT[k].kind != 3 || (rec->flags5a & 4))
            continue;
        lvl = rec->level;
        if (lvl < util_RandomU(0x40) + 0x25) {
            s1.kind = 4;
            s1.unk04 = rec->id;
            s1.unk08 = 1;
            if (lvl < 0x21)
                s1.unk10 = 0x32;
            else
                s1.unk10 = lvl >= 0x42 ? 10 : 30;
            s2.unk08 = money;
            s2.unk04 = *(int *)(o + 2);
            s2.kind = 1;
            s2.unk10 = 1;
            r = ai_Unk495a50(0xf, p->id, 0, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
                return r;
            }
        }
        if (rec->type == 0xa)
            continue;
        if (rec->type == 9) {
            cntB++;
            if (!(rec->flags5a & 0x41))
                recB = rec;
        } else {
            cntA++;
            if (!(rec->flags5a & 0x41))
                recA = rec;
        }
    }
    if (util_RandomU(8) != 0)
        return 0;
    if (cntA != 0 && recA != 0) {
        o1->kind = 4;
        o1->unk04 = 9;
        o1->unk08 = 1;
        o2->kind = 4;
        o2->unk04 = recA->id;
        o2->unk08 = 1;
        return 0x2f;
    }
    if (cntB != 0 && recB != 0) {
        o1->kind = 4;
        o1->unk04 = 0xa;
        o1->unk08 = 1;
        o2->kind = 4;
        o2->unk04 = recB->id;
        o2->unk08 = 1;
        return 0x2f;
    }
    s1.unk04 = 8;
    s1.unk08 = 1;
    s2.kind = 0;
    s2.unk04 = 0;
    s2.unk08 = 0;
    if (ai_Unk4a5140(0, (AIPER *)p, 1, &s1, &s2, 1) == 0)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return 0x2f;
}

/* 0x4a0a70, decide callback 0x39: the office building (kind 0x17..0x1a by the office 0x169): upgrade it (0x495a50
   kind 0xf), buy an item for it (0x4a5540) or build the next type; when the treasury (object 0x142) cannot pay,
   a credit (o1 kind 1, o2 kind 0x12) from the character's money over 80000 */

int ai_Unk4a0a70(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    AIBLD *bl;
    AIBTYPE *t;
    unsigned char *o;
    unsigned char k;
    char lvl;
    int money, need, amt, n, m, c;

    if (f != 0 || o1->kind != 0 || a != 0)
        return 0;
    k = ((unsigned char *)p)[0x169];
    if (k < 0x1e && k > 0x21)
        return 0;
    if (k == 0x1e)
        bl = gm_BuildQuery(1, 5, 0x17);
    else if (k == 0x1f)
        bl = gm_BuildQuery(1, 5, 0x18);
    else if (k == 0x20)
        bl = gm_BuildQuery(1, 5, 0x19);
    else
        bl = gm_BuildQuery(1, 5, 0x1a);
    if (bl == 0)
        return 0;
    o = (unsigned char *)gm_Query(bl->objList, 2, 6, 0, 0x142);
    money = gm_Unk509e30(*(int *)(o + 0x14), ai_OwnPlayer);
    lvl = bl->level;
    if (lvl < util_Random(0x10) + 0x32 && !(bl->flags5a & 4)) {
        s1.kind = 4;
        s1.unk04 = bl->id;
        s1.unk08 = 1;
        if (lvl < 0x21)
            need = 0x32;
        else
            need = lvl >= 0x42 ? 10 : 30;
        s2.unk04 = *(int *)(o + 2);
        s2.unk10 = 1;
        s1.unk10 = need;
        s2.kind = 1;
        s2.unk08 = money;
        n = ai_Unk495a50(0xf, p->id, 0, &s1, &s2);
        if (n != 0) {
            *o1 = s1;
            *o2 = s2;
            return n;
        }
        n = ai_Unk4a4e30((AIBLDX *)bl, need);
        if (n > money) {
            amt = n;
            c = 1;
            goto credit;
        }
    }
    if (ai_Unk4a5540(&f, &a, bl, 0) != 0) {
        s1.kind = 2;
        s1.unk04 = f;
        s1.unk08 = 1;
        s1.unk0c = *(int *)(o + 2);
        s1.unk10 = 0x39;
        s2.unk10 = money;
        if (a != -1) {
            s2.kind = 1;
            s2.unk04 = a;
        } else {
            s2.kind = 4;
            s2.unk04 = bl->id;
        }
        s2.unk08 = 1;
        *(int *)&s2.unk14 = 0;
        amt = (int)gm_GoodsPrice(f, ai_OwnPlayer);
        c = 1;
        if (amt < money) {
credit:
            m = gm_Unk509ee0((AIPER *)p, ai_OwnPlayer);
            if (m < 80000)
                return 0;
            amt += 0x20;
            if ((int)((m - 80000) * 0.25f) < amt)
                return 0;
            o1->kind = 1;
            o1->unk04 = *(int *)(o + 2);
            o1->unk10 = c;
            o2->unk08 = amt;
            o2->kind = 0x12;
            return 0x39;
        }
        n = ai_Unk495a50(0x39, p->id, 1, &s1, &s2);
        if (n == 0)
            return 0;
        *o1 = s1;
        *o2 = s2;
        return n;
    }
    t = &ai_BTypesT[bl->type];
    if (bl->flags5a & 0x45)
        return 0;
    if (t->cap247 >= ((unsigned char *)t)[0x248])
        return 0;
    n = bl->type + 1;
    amt = (int)(gm_BuildingKindValue(n) * 0.4f);
    if (amt < money) {
        o1->kind = 4;
        o1->unk04 = n;
        o1->unk08 = 1;
        o1->unk0c = bl->id;
        o2->kind = 1;
        o2->unk04 = *(int *)(o + 2);
        o2->unk08 = 1;
        return 0x39;
    }
    m = gm_Unk509ee0((AIPER *)p, ai_OwnPlayer);
    if (m < 80000)
        return 0;
    amt += 0x20;
    if ((int)((m - 80000) * 0.5f) < amt)
        return 0;
    o1->kind = 1;
    o1->unk04 = *(int *)(o + 2);
    o1->unk10 = 1;
    o2->unk08 = amt;
    o2->kind = 0x12;
    return 0x39;
}

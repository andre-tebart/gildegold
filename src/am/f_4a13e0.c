/* 0x4a13e0, decide callback 0x3b: the oldest married child (0x164, not 0x10000 in 0x1c8) gets one of three
   offers of 0x4948f0 (o2 kind 0xe), the best or the worst by the child's ratio 0x80.. to the family's 0x503ee0
   values; the character needs ten times the price */

int ai_Unk4a13e0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIX6 x;
    float s[3];
    G8OFFER rec[3];
    G5CH *c, *best;
    unsigned char k;
    int m, n;
    float va, vb;

    if (f != 0)
        return 0;
    if (a != 0)
        return 0;
    if (((unsigned char *)p)[2] != 5) {
        c = (G5CH *)gm_FindChar(*(int *)((unsigned char *)p + 0x5c));
        if (c != 0) {
            k = ((unsigned char *)c)[2];
            if (k == 5 || k == 6 || k == 7)
                return 0;
            if (((unsigned char *)c)[9] == 0 && ai_WorksForMaster((AICHAR *)c) != 0)
                return 0;
        }
    }
    best = 0;
    for (m = 0; m < 5; m++) {
        c = (G5CH *)gm_FindChar(((int *)((unsigned char *)p + 0x68))[m]);
        if (c != 0 && ((unsigned char *)c)[8] > 0 && ((unsigned char *)c)[0x164] != 0
            && !(*(int *)((unsigned char *)c + 0x1c8) & 0x10000)
            && (best == 0 || c->unk00a > best->unk00a))
            best = c;
    }
    if (best == 0)
        return 0;
    G8_4948F0(p, rec);
    gm_Unk503ee0(&x, ((unsigned char *)best)[0x164]);
    for (n = 0; n < 3; n++) {
        va = max((float)x.v[rec[n].b], 1.0f);
        vb = max((float)x.v[rec[n].c], 1.0f);
        s[n] = ((unsigned char *)best)[0x80 + rec[n].c] / vb + ((unsigned char *)best)[0x80 + rec[n].b] / va;
    }
    if (util_RandomU(2) != 0) {
        m = s[0] > s[1] ? 0 : 1;
        if (!(s[m] > s[2]))
            m = 2;
    } else {
        m = s[0] < s[1] ? 0 : 1;
        if (!(s[m] < s[2]))
            m = 2;
    }
    if (gm_Unk509ee0((AIPER *)p, ai_OwnPlayer) < rec[m].a * 10)
        return 0;
    o1->kind = 7;
    o1->unk04 = best->netId;
    o1->unk10 = rec[m].a;
    o2->kind = 0xe;
    o2->unk04 = rec[m].b;
    o2->unk10 = rec[m].c;
    return 0x3b;
}

/* 0x49eec0, decide callback 0x33 (also for 0x13 / 0x10 / 0x11): when fewer jobs (0x53e400 key 0x23) run than a
   random 0..9, picks one of the character's own buildings below its stock limit (hour-based) and a kind-5
   building of someone else, then the best of three offers of 0x50a250 */

int ai_Unk49eec0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int lim[4];
    G8OFFER rec[3];
    AIDEC d;
    AIBLD *own, *b;
    unsigned char k;
    int i, n, h, j, best;
    float m;

    lim[0] = ai_Unk5ed140[util_Random(0x10)];
    if (((unsigned char *)p)[2] == 3)
        return 0;
    if (a != 0x33 && a != 0x13 && a != 0x10 && a != 0x11)
        return 0;
    n = 0;
    if (he_FindEvent(2, 0, 0x23, 2, p->id) != 0) {
        do
            n++;
        while (he_NextEvent() != 0);
    }
    if (util_Random(10) < n + 1)
        return 0;
    if (a == 0x33) {
        h = ai_TimeT.hour - 8;
        if (h <= 0 || h >= 10)
            h = h > 0 ? 10 : 0;
        m = (10 - h) * 0.1f;
        m = m * m;
        lim[1] = (int)(m * 6400.0f);
        lim[2] = (int)(m * 12800.0f);
        lim[3] = (int)(m * 25600.0f);
        j = util_Random(0x100);
        for (i = 0x100;; ) {
            own = &ai_Blds[j];
            k = own->type;
            if (k != 0 && own->owner == p->id && ((1 << ai_BTypes[k].kind) & ((unsigned)&ai_Unk770d8c + 0x63604))
                && own->unk39 < lim[ai_BTypes[k].cap247])
                break;
            i--;
            j = (j + lim[0]) % 0x100;
            if (i == 0)
                return 0;
        }
        j = util_Random(0x100);
        for (i = 0x100;; ) {
            b = &ai_Blds[j];
            k = b->type;
            if (k != 0 && b->owner != p->id && ai_BTypes[k].kind == 5)
                break;
            i--;
            j = (j + lim[0]) % 0x100;
            if (i == 0)
                return 0;
        }
        n = gm_Unk50a250(b, p, 3, rec);
        if (n <= 0)
            return 0;
        gv_GetTableRecord(0xe, &d);
        for (i = 0; i < n; i++)
            ((float *)lim)[i + 1] = rec[i].b * 0.14285715f * ((float)(d.cur - 2) / rec[i].c) * rec[i].a;
        m = 0.0f;
        best = -1;
        for (i = 0; i < n; i++) {
            if (m < ((float *)lim)[i + 1]) {
                m = ((float *)lim)[i + 1];
                best = i;
            }
        }
        if (best < 0 && best > 2)
            return 0;
        o1->kind = 4;
        o1->unk04 = b->id;
        o1->unk08 = own->id;
        o2->kind = 0x14;
        o2->unk04 = rec[best].a;
        o2->unk08 = rec[best].b;
        o2->unk10 = rec[best].c;
        return 0x33;
    }
    if (o1->kind != 0x12)
        return 0;
    j = util_Random(0x100);
    for (i = 0x100;; ) {
        b = &ai_Blds[j];
        k = b->type;
        if (k != 0 && b->owner != p->id && ai_BTypes[k].kind == 5)
            break;
        i--;
        j = (j + lim[0]) % 0x100;
        if (i == 0)
            return 0;
    }
    n = gm_Unk50a250(b, p, 3, rec);
    if (n <= 0)
        return 0;
    gv_GetTableRecord(0xe, &d);
    for (i = 0; i < n; i++) {
        if (o1->unk08 < (unsigned int)rec[i].a)
            ((float *)lim)[i + 1] = (float)(d.cur - 2) / rec[i].c * (rec[i].b * 0.14285715f) * rec[i].a;
        else
            ((float *)lim)[i + 1] = -1.0f;
    }
    m = 0.0f;
    best = -1;
    for (i = 0; i < n; i++) {
        if (m < ((float *)lim)[i + 1]) {
            m = ((float *)lim)[i + 1];
            best = i;
        }
    }
    if (best < 0 || best > 2)
        return 0;
    o1->kind = 4;
    o1->unk04 = b->id;
    o1->unk08 = -1;
    o2->kind = 0x14;
    o2->unk04 = rec[best].a;
    o2->unk08 = rec[best].b;
    o2->unk10 = rec[best].c;
    return 0x33;
}

/* 0x498c70, decide: build up the character's workshops (action 0x13) */

int ai_Unk498c70(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIARG t1, t2, s1, s2;
    AIBTX *bt;
    AIBLDX *bl;
    AIJOBX *j;
    int i, tot, cnt, ones, sel, maxn, lim, k, n, v, r, d, h;

    tot = 0;
    ones = 0;
    cnt = 0;
    maxn = (p->kind == 5) + 2;
    sel = 0;
    if (p->kind == 3)
        return 0;
    if (a != 0)
        return 0;
    if (b == 0) {
        if (o1->kind != 0)
            return 0;
    } else if (b == 1 && o1->kind != 5)
        return 0;
    lim = (int)(gm_Unk509ee0(p, ai_OwnPlayer) * 0.66f);
    s2.kind = 0;
    s2.unk04 = 0;
    s2.unk08 = 0;
    for (i = 0; i < 0x100; i++) {
        if (AI_BLDX[i].type && AI_BLDX[i].owner == p->id) {
            bt = &AI_BTX[AI_BLDX[i].type];
            k = gm_BuildingJob(bt->kind);
            if (k > 0 && k < 0x10) {
                tot += bt->unk247;
                cnt++;
                if (bt->unk247 == 1)
                    ones++;
            }
        }
    }
    if (tot && (!p->bld2 || (p->bld2->type != 7
            && (float)(AI_BTX[p->bld2->type].unk247 - AI_BTX[4].unk247 + 1) / tot < 0.33f))) {
        AI_ZERO(&t1, sizeof(t1));
        AI_ZERO(&t1, sizeof(t1));
        r = ai_Unk4984d0(0x13, p, 0, &t1, &t2);
        if (r) {
            *o1 = t1;
            *o2 = t2;
            return r;
        }
    }
    if (o1->kind != 0)
        return 0;
    for (i = 0; i < 0x100; i++) {
        bl = &AI_BLDX[i];
        if (bl->type && bl->owner == p->id && !(bl->flags5a & 0x41)) {
            bt = &AI_BTX[bl->type];
            if (bt->unk247 < bt->unk248) {
                k = gm_BuildingJob(bt->kind);
                r = gm_Unk5044b0(p->id, k);
                if (r) {
                    n = gm_TitlesLeft(r);
                    n = n / 2 + n % 2;
                    if (n > bt->unk247 && k != 7) {
                        int v1, d1;

                        v1 = (int)(gm_BuildingKindValue(bl->type + 1) * 0.3f);
                        if (lim < v1) {
                            if ((float)(v1 - lim) / v1 < 0.25f) {
                                o1->kind = 0x12;
                                o1->unk08 = v1 - lim;
                                return ai_Unk49eec0(0x13, p, b, o1, o2);
                            }
                            return 0;
                        }
                        o1->kind = 5;
                        o1->unk04 = bl->type + 1;
                        o1->unk08 = 1;
                        o2->kind = 4;
                        o2->unk04 = bl->id;
                        o2->unk08 = 1;
                        return 0x13;
                    }
                    if (k == 7 && n > bt->unk247)
                        sel = bl->type + 1;
                }
            }
        }
    }
    if (p->flags1c8 & ((unsigned)&txt_Keys + 0x181c0))
        return 0;
    if (cnt > 3)
        return 0;
    if (ones > 0 && !sel) {
        if (ones > maxn)
            return 0;
        if (ai_TimeT.year % 8 != (unsigned int)p->netId % 8)
            return 0;
    }
    if (!sel) {
        sel = gm_FirstTitleNumber(p->jobId);
        if (!sel)
            return 0;
    }
    s1.kind = 5;
    s1.unk04 = sel;
    s1.unk08 = 1;
    s2.kind = 0;
    s2.unk04 = 0;
    s2.unk08 = 0;
    if (ai_Unk4a5140(0, p, b, &s1, &s2, 1)) {
        int v2, d2;

        v2 = gm_Unk509dc0(p, s1.unk04);
        if (lim < v2) {
            if ((float)(v2 - lim) / v2 < 0.25f) {
                o1->kind = 0x12;
                o1->unk08 = v2 - lim;
                return ai_Unk49eec0(0, p, b, o1, o2);
            }
            return 0;
        }
    } else {
        k = gm_JobBuildingType(gm_TitleGroup(p->jobId));
        n = gm_TitlesLeft(p->jobId);
        h = n % 2 + n / 2;
        if (k == 0x10)
            h = 3;
        s1.kind = 0;
        s2.kind = 0;
        for (bl = AI_501190_3(1, 5, k); s1.kind == 0 && bl; bl = AI_501030()) {
            if (AI_BTX[bl->type].unk247 <= h && (bl->flags5a & 2)) {
                for (j = AI_JOBS1B(); j; j = AI_JOBNEXT()) {
                    if (j->unkac == bl->id) {
                        v = j->unkb0;
                        if (lim < v) {
                            r = gm_EquipmentValue(bl, bl->level);
                            lim = v - lim;
                            if ((float)lim / v < (float)((float)(v - r) / r * -0.5f) + 0.25f) {
                                o1->kind = 0x12;
                                o1->unk08 = lim;
                                return ai_Unk49eec0(0x13, p, b, o1, o2);
                            }
                            return 0;
                        }
                        s1.kind = 4;
                        s1.unk04 = bl->id;
                        s1.unk08 = 1;
                        s2.kind = 8;
                        s2.unk04 = j->unk04;
                        break;
                    }
                }
            }
        }
        if (s1.kind != 4 || s2.kind != 8)
            return 0;
    }
    *o1 = s1;
    *o2 = s2;
    return 0x13;
}

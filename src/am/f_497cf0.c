/* 0x497cf0, decide: renovate the character's worst building (action 15) */

int ai_Unk497cf0(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIBLDX *bl, *best;
    float lim, f;
    int i, d, v;

    if (o1->kind == 0) {
        d = util_RandomU(0x24) + 0x23;
        best = 0;
        bl = AI_BLDX;
        for (i = 0; i < 0x100; i++, bl++) {
            if (bl->type && bl->masterId == p->id && !(bl->flags5a & 0x45)
                && ((1 << ai_BTypesT[bl->type].kind) & ((unsigned)&ai_Unk770d8c + 0x53628)) && bl->level < d) {
                d = bl->level;
                best = bl;
            }
        }
        if (!best)
            return 0;
        lim = gm_Unk509ee0(p, ai_OwnPlayer) * 0.95f;
        v = d < 0x32 ? 50 : 30;
        f = ai_Unk4a4e30(best, v);
        if (f == 0.0f || f > lim)
            return 0;
        o1->kind = 4;
        o1->unk04 = best->id;
        o1->unk08 = 1;
        o1->unk10 = v;
        o1->unk14 = f;
        o2->kind = 7;
        o2->unk04 = p->netId;
        o2->unk08 = (int)lim;
        o2->unk10 = 1;
        return 15;
    }
    if (o1->kind == 4) {
        bl = AI_BLDX;
        for (i = 0; i < 0x100; i++, bl++) {
            if (bl->type && bl->id == o1->unk04) {
                if (bl->flags5a & 5)
                    return 0;
                f = ai_Unk4a4e30(bl, o1->unk10);
                if (f == 0.0f || f > o2->unk08)
                    return 0;
                o1->unk14 = f;
                return 15;
            }
        }
    }
    return 0;
}

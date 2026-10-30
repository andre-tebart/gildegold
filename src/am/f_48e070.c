/* 0x48e070, the building of a master (kind != 3) in *out; 2: has one, 1: has a good one, 0: none */

int ai_Unk48e070(AIPX *p, int *out)
{
    int i, n, bld;

    if (ai_Unk5ffd1c < ai_TimeT.year) {
        ai_Unk6f0470 = 0;
        for (i = 0; i < 768; i++) {
            AIPX *c = &((AIPX *)ai_Chars)[i];

            if (c->kind == 6 || c->kind == 7 || c->kind == 5)
                ai_Unk6f0470++;
        }
        ai_Unk5ffd1c = ai_TimeT.year;
    }
    if (p->kind != 3) {
        bld = (int)gm_BuildQuery(2, 5, 0x1e, 4, p->id);
        if (out)
            *out = bld;
        if (bld)
            return 2;
        if (p->kind == 5)
            return 1;
        if ((p->kind == 2 || p->kind == 4) && p->unk00d > 6)
            return 1;
    }
    return 0;
}

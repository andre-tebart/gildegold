/* 0x490b90, the execute callback of "Handle Grave" */

int ai_Unk490b90(AGCH *p, AGARG *s1, AGARG *s2)
{
    AGARG *a = s1;

    gm_FindById((int)&s1, 0, 0, a->unk0c);
    if (s1 != 0) {
        if (a->kind == 4) {
            unk_Unk4f0430(p, s1, a->unk04, 10, a->unk08);
            return 0x3e;
        }
        if (a->kind == 5) {
            unk_Unk4f0e80I(p, a->unk04, s1, s2->unk04, s2->kind, a->unk08);
            return 0x3e;
        }
    }
    return 0;
}

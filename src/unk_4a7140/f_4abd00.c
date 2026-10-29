/* 0x4abd00, decide callback of action 0x47 "Ehrerbietung": p (unk194 >= 2, not flagged 0x20, not of kind 3) with a
   building of kind 0x1f holding object 0x276 picks a character within 25..57 units to honour, into m (kind 7, a
   cost 2..7); returns 0x45 (sic) */

int ai_Unk4abd00(int a, AIPER *p, int c, unsigned char *m)
{
    int v, i, n;
    unsigned int flags;
    unsigned short id;
    AIBLD *b;
    AIPER *o;

    v = gm_Unk5044b0(p->id, 0xe);
    flags = 0;
    if ((p->flags1cc & 0x20) != 0 || p->unk194 < 2)
        return 0;
    if ((a != 0x47 && a != 0) || c != 0 || v == 0 || p->kind == 3)
        return 0;
    b = ai_Blds;
    for (i = 0; i < 0x100; i++) {
        if (b->type != 0 && b->owner == p->id && ai_BTypes[b->type].kind == 0x1f
            && gm_Query(b->objList, 2, 6, 0, 0x276) != 0) {
            flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
            if (G10_4A58D0(p, 1, (int *)&flags, 0.0f, (float)util_Random(0x20) + 25.0f, &id) == 0)
                return 0;
            o = &((AIPER *)ai_Chars)[id];
            n = util_Random(p->unk194 < 8 ? p->unk194 : 8);
            if (n <= 2)
                n = 2;
            *(int *)(m + 0x10) = n;
            *(int *)(m + 4) = o->netId;
            *(int *)(m + 0xc) = 0;
            m[0] = 7;
            return 0x45;
        }
        b = (AIBLD *)((unsigned char *)b + 0xa9);
    }
    return 0;
}

/* 0x4f2b30, reads what 0x4f28d0 wrote from the file f (the tag must be 0x1d; saves after 0x10040 carry the counts 0x80 / 0x200 first, older ones assume them): the table ints, 0x6f1b88, the used 0x2d-byte records (the rest are set to -1 fields)
   and the pairs (the table 0x91b6e0 starts as all -1); 1 when all was read. ROUGH */

int unk_Unk4f2b30(void *f)
{
    unsigned n1, n2, i;
    int tag;
    char *p;
    int *q;

    n1 = 0x80;
    n2 = 0x200;
    if (f == 0)
        return 0;
    if (UNK_FR(&tag, 4, 1, f) == 0)
        return 0;
    if (tag != 0x1d)
        return 0;
    for (p = unk_Tab611258 + 0x18; (int)p < ((unsigned)&unk_Tab611258 + 0x42c); p += 0x24) {
        if (UNK_FR(p, 4, 1, f) == 0)
            return 0;
    }
    if (UNK_FR(unk_Var6f1b88, 4, 1, f) == 0)
        return 0;
    if (ls_Version > 0x10040) {
        if (UNK_FR(&n1, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(&n2, 4, 1, f) == 0)
            return 0;
    }
    i = 0;
    if (n1 != 0) {
        p = gv_Graveyard + 4;
        do {
            if (UNK_FR(p - 4, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(p, 0xe, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0xe, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x12, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x16, 2, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x18, 1, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x19, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x1d, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x21, 4, 1, f) == 0)
                return 0;
            i++;
            p += 0x2d;
        } while (i < n1);
        if ((int)i > 0x1ff)
            goto pairs;
    }
    for (q = (int *)(gv_Graveyard + 0x12 + i * 0x2d); (int)q < (int)(gv_Graveyard + 0x5a12); q = (int *)((char *)q + 0x2d)) {
        *(int *)((char *)q - 0x12) = -1;
        q[0] = -1;
        q[1] = -1;
        *(short *)(q + 2) = 0;
        *(int *)((char *)q + 0x13) = 0;
    }
pairs:
    memset(unk_Tab91b6e0, 0xff, 0x4000);
    i = 0;
    if (n2 != 0) {
        q = (int *)(unk_Tab91b6e0 + 4);
        do {
            if (UNK_FR(q - 1, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(q, 4, 1, f) == 0)
                return 0;
            i++;
            q += 2;
        } while (i < n2);
    }
    return 1;
}

/* 0x4ed910, reads what 0x4ed480 wrote from the file f (version v: the saves before 0x10055 have no fields +0xc / +0x10, they are 1.0); sets the slots' pointer fields (the callback 0x4ed3f0 at +0x168, the slot's record at +0x16c) and the float fields +0x78 / +0xe0 / +0x148 from 0x6333cc; 1 when all was read */


int unk_Unk4ed910(void *f, int v)
{
    char *e;
    char *q;

    if (UNK_FR(&ai_Unk6f0e38, 4, 1, f) == 0)
        return 0;
    if (UNK_FR(&ai_Unk6f0e3c, 2, 1, f) == 0)
        return 0;
    if (UNK_FR(&ai_Unk6f0e3e, 4, 1, f) == 0)
        return 0;
    if (UNK_FR(&ai_Unk6f0e42, 4, 1, f) == 0)
        return 0;
    if (UNK_FR(&ai_Unk925100, 4, 1, f) == 0)
        return 0;
    if (UNK_FR(&ai_Unk925104, 4, 1, f) == 0)
        return 0;
    if (UNK_FR(&ai_Unk925108, 12, 1, f) == 0)
        return 0;
    if (UNK_FR(&ai_Unk925114, 4, 1, f) == 0)
        return 0;
    if (UNK_FR(&ai_Unk925118, 4, 1, f) == 0)
        return 0;
    if (UNK_FR(&ai_Unk92511c, 4, 1, f) == 0)
        return 0;
    q = ai_Unk6f0e48;
    for (e = gv_Effects; e < gv_Effects + 8 * 0x170; e += 0x170) {
        if (UNK_FR(e + 0x0, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x8, 4, 1, f) == 0)
            return 0;
        if (v < 0x10055) {
            *(float *)(e + 0xc) = 1.0f;
            *(float *)(e + 0x10) = 1.0f;
        } else {
            if (UNK_FR(e + 0xc, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(e + 0x10, 4, 1, f) == 0)
                return 0;
        }
        if (UNK_FR(e + 0x14, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x18, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x28, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x38, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x58, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x68, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x6c, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x70, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x74, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x7c, 4, 1, f) == 0)
            return 0;
        *(int *)(e + 0x78) = ai_Unk6333cc;
        if (UNK_FR(e + 0x80, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x90, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0xa0, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0xc0, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0xd0, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0xd4, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0xd8, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0xdc, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0xe4, 4, 1, f) == 0)
            return 0;
        *(int *)(e + 0xe0) = ai_Unk6333cc;
        if (UNK_FR(e + 0xe8, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0xf8, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x108, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x128, 16, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x138, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x13c, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x140, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x144, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(e + 0x14c, 4, 1, f) == 0)
            return 0;
        *(int *)(e + 0x148) = ai_Unk6333cc;
        if (UNK_FR(e + 0x150, 12, 1, f) == 0)
            return 0;
        *(char **)(e + 0x16c) = q;
        *(void **)(e + 0x168) = unk_Unk4ed3f0;
        q += 0x38;
    }
    return 1;
}

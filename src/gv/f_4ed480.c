/* 0x4ed480, writes the game state of the effect slots to the file f: the globals 0x6f0e38 .. 0x92511c, then the 8 slots of 0x170 bytes at 0x6f1008 (fields by offset); 1 when all was written */

int unk_Unk4ed480(void *f)
{
    char *e;

    if (UNK_FW(&ai_Unk6f0e38, 4, 1, f) == 0)
        return 0;
    if (UNK_FW(&ai_Unk6f0e3c, 2, 1, f) == 0)
        return 0;
    if (UNK_FW(&ai_Unk6f0e3e, 4, 1, f) == 0)
        return 0;
    if (UNK_FW(&ai_Unk6f0e42, 4, 1, f) == 0)
        return 0;
    if (UNK_FW(&ai_Unk925100, 4, 1, f) == 0)
        return 0;
    if (UNK_FW(&ai_Unk925104, 4, 1, f) == 0)
        return 0;
    if (UNK_FW(&ai_Unk925108, 12, 1, f) == 0)
        return 0;
    if (UNK_FW(&ai_Unk925114, 4, 1, f) == 0)
        return 0;
    if (UNK_FW(&ai_Unk925118, 4, 1, f) == 0)
        return 0;
    if (UNK_FW(&ai_Unk92511c, 4, 1, f) == 0)
        return 0;
    for (e = gv_Effects; e < gv_Effects + 8 * 0x170; e += 0x170) {
        if (UNK_FW(e + 0x0, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x8, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0xc, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x10, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x14, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x18, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x28, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x38, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x58, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x68, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x6c, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x70, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x74, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x7c, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x80, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x90, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0xa0, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0xc0, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0xd0, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0xd4, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0xd8, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0xdc, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0xe4, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0xe8, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0xf8, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x108, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x128, 16, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x138, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x13c, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x140, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x144, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x14c, 4, 1, f) == 0)
            return 0;
        if (UNK_FW(e + 0x150, 12, 1, f) == 0)
            return 0;
    }
    return 1;
}

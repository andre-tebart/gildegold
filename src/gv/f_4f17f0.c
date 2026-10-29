/* 0x4f17f0, writes the graveyard record of the object o to the file f: +0, +4 (4 each), the four flag bytes +0x18 .. +0x1b, the zombie records (0x44 bytes: 4, 0x10, 0x10, 5, then six ints) of the
   kind's table 0x5ee074, then the graves (+0 +4 +8 short +10 short +0xc byte, and the index of the zombie record at +0x10) of the table 0x5ee064; 1 when all was written */

int unk_Unk4f17f0(void *f, unsigned char *o)
{
    char *g;
    char *z;
    int i, n, idx;

    g = *(char **)(o + 0x65);
    if (g == 0)
        return 0;
    if (UNK_FW(g, 4, 1, f) == 0)
        return 0;
    if (UNK_FW(g + 4, 4, 1, f) == 0)
        return 0;
    if (UNK_FW(g + 0x18, 1, 1, f) == 0)
        return 0;
    if (UNK_FW(g + 0x19, 1, 1, f) == 0)
        return 0;
    if (UNK_FW(g + 0x1a, 1, 1, f) == 0)
        return 0;
    if (UNK_FW(g + 0x1b, 1, 1, f) == 0)
        return 0;
    z = *(char **)(g + 0x20);
    i = 0;
    if (unk_Tab5ee074[*(int *)(g + 4)] > 0) {
        do {
            if (UNK_FW(z, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 4, 0x10, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x14, 0x10, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x24, 5, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x2c, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x30, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x34, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x38, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x3c, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x40, 4, 1, f) == 0)
                return 0;
            i++;
            z += 0x44;
        } while (i < unk_Tab5ee074[*(int *)(g + 4)]);
    }
    z = *(char **)(g + 0x1c);
    i = 0;
    if (unk_Tab5ee064[*(int *)(g + 4)] > 0) {
        do {
            if (UNK_FW(z, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 4, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 8, 2, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 10, 2, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0xc, 1, 1, f) == 0)
                return 0;
            idx = (*(int *)(z + 0x10) - *(int *)(g + 0x20)) / 0x44;
            if (UNK_FW(&idx, 4, 1, f) == 0)
                return 0;
            i++;
            z += 0x14;
        } while (i < unk_Tab5ee064[*(int *)(g + 4)]);
    }
    return 1;
}

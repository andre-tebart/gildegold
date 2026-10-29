/* 0x4f1aa0, reads the graveyard record of 0x4f17f0 from the file f for the object o (allocated by 0x4efc10): the same fields in the same order; the graves' zombie pointers are rebuilt from the saved indexes; 1 when all was read */

int gv_ReadGraveyard(void *f, unsigned char *o)
{
    char *g;
    char *z;
    int i, idx;

    g = (char *)gv_AllocGraveyard(o);
    if (g == 0)
        return 0;
    if (UNK_FR(g, 4, 1, f) == 0)
        return 0;
    if (UNK_FR(g + 4, 4, 1, f) == 0)
        return 0;
    if (UNK_FR(g + 0x18, 1, 1, f) == 0)
        return 0;
    if (UNK_FR(g + 0x19, 1, 1, f) == 0)
        return 0;
    if (UNK_FR(g + 0x1a, 1, 1, f) == 0)
        return 0;
    if (UNK_FR(g + 0x1b, 1, 1, f) == 0)
        return 0;
    z = *(char **)(g + 0x20);
    i = 0;
    if (unk_Tab5ee074[*(int *)(g + 4)] > 0) {
        do {
            if (UNK_FR(z, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 4, 0x10, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x14, 0x10, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x24, 5, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x2c, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x30, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x34, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x38, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x3c, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x40, 4, 1, f) == 0)
                return 0;
            i++;
            z += 0x44;
        } while (i < unk_Tab5ee074[*(int *)(g + 4)]);
    }
    i = 0;
    z = *(char **)(g + 0x1c);
    if (unk_Tab5ee064[*(int *)(g + 4)] > 0) {
        do {
            if (UNK_FR(z, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 4, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 8, 2, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 10, 2, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0xc, 1, 1, f) == 0)
                return 0;
            if (UNK_FR(&idx, 4, 1, f) == 0)
                return 0;
            *(int *)(z + 0x10) = *(int *)(g + 0x20) + idx * 0x44;
            z += 0x14;
            i++;
        } while (i < unk_Tab5ee064[*(int *)(g + 4)]);
    }
    return 1;
}

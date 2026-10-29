/* 0x4ef4f0, 1 unless the part i (0 .. 3) of the record r is missing (the pointer at +8 + 4 * i is 0 or the flag byte at +0x18 + i is 0) */

int unk_Unk4ef4f0(char *r, int i)
{
    if (i == 0) {
        if (*(int *)(r + 8) == 0 || r[0x18] == 0)
            return 0;
    } else if (i == 1) {
        if (*(int *)(r + 0xc) == 0 || r[0x19] == 0)
            return 0;
    } else if (i == 2) {
        if (*(int *)(r + 0x10) == 0 || r[0x1a] == 0)
            return 0;
    } else if (i == 3 && (*(int *)(r + 0x14) == 0 || r[0x1b] == 0))
        return 0;
    return 1;
}

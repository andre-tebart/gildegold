/* 0x4ef570, the entry n (0x14 bytes) of the list i (0 .. 3: the pointer at +8 + 4 * i of the record r) when the list exists (0x4ef4f0) and the level (+0x18 + i; here read from the flag byte, the original takes it from edx) allows
   that many entries; 0 when r[1] is not 1 .. 3 or n is out of range. ROUGH */

int unk_Unk4ef570(char *r, int i, unsigned char n)
{
    int lv;

    if (*(int *)(r + 4) > 0 && *(int *)(r + 4) < 4) {
        if (i == 0) {
            lv = r[0x18];
            if (unk_Unk4ef4f0(r, 0) != 0) {
                if ((lv != 1 || n < 8) && (lv != 2 || n < 0xc) && (lv != 3 || n < 0x10))
                    return *(int *)(r + 8) + n * 0x14;
            }
        } else if (i == 1) {
            lv = r[0x19];
            if (unk_Unk4ef4f0(r, 1) != 0) {
                if ((lv != 1 || n < 4) && (lv != 2 || n < 6) && (lv != 3 || n < 8))
                    return *(int *)(r + 0xc) + n * 0x14;
            }
        } else if (i == 2) {
            lv = r[0x1a];
            if (unk_Unk4ef4f0(r, 2) != 0 && lv != 1) {
                if ((lv != 2 || n < 2) && (lv != 3 || n < 4))
                    return *(int *)(r + 0x10) + n * 0x14;
            }
        } else if (i == 3) {
            lv = r[0x1b];
            if (unk_Unk4ef4f0(r, 3) != 0 && lv != 1 && lv != 2 && (lv != 3 || n < 2))
                return *(int *)(r + 0x14) + n * 0x14;
        }
    }
    return 0;
}

/* 0x4fabe0, the end of the day for the incomes: shifts the 17-entry history of each of the five sums (0x91b580 / 0x91b5c4 / 0x91b608 / 0x91b64c / 0x91b690, 0x40 bytes) one place and clears the sums 0x91b5c0 / 0x91b604 / 0x91b648 / 0x91b68c / 0x91b6d0; runs 0x4fa4b0 (i, out, flags) for the 0x300 characters (clearing +0x198 / +0x19c); with flag 1 also 0x4fb0e0 and a list of up to 8 objects of the group (1, 5, 7) (0x501190 / 0x501030); then clears the ints +0x4d and (for kinds 11 .. 13) +0x69 in the 256 building records. ROUGH */

void unk_Unk4fabe0(unsigned flags)
{
    unsigned char *o;
    int *c;
    int i, k, j;
    unsigned v[8];
    int ent[14];

    memmove(gm_IncomeHistory, gm_IncomeHistory + 4, 0x40);
    memmove(gm_IncomeHistory + 0x44, gm_IncomeHistory + 0x48, 0x40);
    memmove(gm_IncomeHistory + 0x88, gm_IncomeHistory + 0x8c, 0x40);
    memmove(gm_IncomeHistory + 0xcc, gm_IncomeHistory + 0xd0, 0x40);
    memmove(gm_IncomeHistory + 0x110, gm_IncomeHistory + 0x114, 0x40);
    *(int *)ai_Unk91b5c0 = 0;
    *(int *)ai_Unk91b604 = 0;
    *(int *)ai_Unk91b648 = 0;
    *(int *)ai_Unk91b68c = 0;
    *(int *)ai_Unk91b6d0 = 0;
    ai_Unk6f1b98 = 0;
    for (i = 0; i < 0x300; i++) {
        unk_Unk4fa4b0(i, ent, flags);
        c = (int *)(ai_Chars[i] + 0x198);
        c[1] = 0;
        c[0] = 0;
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
    }
    if (flags & 1) {
        unk_Unk4fb0e0();
        o = (unsigned char *)gm_BuildQuery(1, 5, 7);
        if (o != 0) {
            k = 0;
            do {
                if (k > 0x1f)
                    break;
                j = k;
                if (*(short *)(o + 0x27) != -1) {
                    *(unsigned char **)((char *)ent + k) = o;
                    j = k + 4;
                    *(unsigned *)((char *)v + k) = *(unsigned char *)(ai_BTypes + *o * 0x24d + 0x247);
                }
                o = (unsigned char *)gm_BuildNext();
                k = j;
            } while (o != 0);
        }
    }
    for (k = 0; k < 0xa900; k += 0xa9) {
        *(int *)(ai_Blds + k + 0x4d) = 0;
        if ((unsigned char)ai_Blds[k] > 10 && (unsigned char)ai_Blds[k] < 0xe)
            *(int *)(ai_Blds + k + 0x69) = 0;
    }
}

/* 0x4fda70, finds a free grown character (not flagged 0x2000 at +0x1cc, kind < 2, not married (+0x5e..), by job (+0x164 = param_3) and, with param_2 = 0, the group of 0x503f30 (param_3); with param_2: the title byte +0x165 = param_2 or 0, the age float (+0x1c) not above the age, 0x503f50) whom the master (the record at +0x27 of o = param_1, or none) is not in debt with (0x50c520 > -0x1e), flags it 0x2000 and returns it. ROUGH */

short *unk_Unk4fda70(char *o, unsigned a, unsigned b)
{
    int i, g;
    char *c;

    if (a == 0) {
        if (b != 0) {
            g = gm_Unk503f30(b & 0xff);
            for (i = 0; i < 0x300; i++) {
                c = ai_Chars[i];
                if (*(short *)c != -1 && (*(unsigned *)(c + 0x1c8) & 0x2000) == 0 && (unsigned char)c[2] < 2) {
                    if ((unsigned char)c[0x164] == b && *(int *)(c + 0x16c) == 0 && (g == 2 || (unsigned char)c[9] == g)) {
                        if (*(short *)(o + 0x27) == -1 || gm_Opinion(*(short *)(o + 0x27), i) > -0x1e) {
                            *(unsigned *)(c + 0x1c8) |= 0x2000;
                            return (short *)c;
                        }
                    }
                }
            }
            return 0;
        }
    } else {
        for (i = 0; i < 0x300; i++) {
            c = ai_Chars[i];
            if (*(short *)c != -1 && (*(unsigned *)(c + 0x1c8) & 0x2000) == 0 && (unsigned char)c[2] < 2) {
                if (((unsigned char)c[0x165] == a || c[0x165] == 0) && *(int *)(c + 0x16c) == 0) {
                    if (*(float *)(c + 0x20) <= (float)*(unsigned short *)(c + 10) && (unsigned char)c[0x164] == b) {
                        g = gm_Unk503f50(a);
                        if (g == 2 || (unsigned char)c[9] == g) {
                            if (*(short *)(o + 0x27) == -1 || gm_Opinion(*(short *)(o + 0x27), i) > -0x1e) {
                                *(unsigned *)(c + 0x1c8) |= 0x2000;
                                return (short *)c;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

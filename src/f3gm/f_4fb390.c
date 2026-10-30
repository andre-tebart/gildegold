/* 0x4fb390, the name record (head table entry) of the character id: a character with the job (+0x164) 0 and +0x20 above +10 gets the default record (0x6118a8 / 0x6118f0 by +9); a character with its own name (+0x1f4) and the int +0x18c gets a copy of it in the history 0x91ac40 (8 records, 0x6f1bb0 the next one; the four head numbers = the int - 0x639); kind 0x11: one of the three records 0x617398; else the record of its job (+0x164) or of its title (+0x165) in the tables by +9 (0x612bd8 / 0x614518, 0x611938 / 0x612288). ROUGH */

unsigned *unk_Unk4fb390(int id)
{
    unsigned *p, *q;
    unsigned char *c;
    unsigned char k;
    char *s;
    int i, n, v;
    unsigned rec;

    p = 0;
    i = 0;
    c = (unsigned char *)ai_Chars[id];
    k = c[0x164];
    if (k != 0 || !((float)*(unsigned short *)(c + 10) < *(float *)(c + 0x20))) {
        s = (char *)c + 0x1f4;
        if (*s != 0 && (v = *(int *)(c + 0x18c)) != -1) {
            ai_Unk6f1bb0 = (ai_Unk6f1bb0 + 1) % 8;
            n = ai_Unk6f1bb0 * 0x44;
            do {
                *((char *)ai_Unk91ac40 + n + 4 + (s - ((char *)c + 0x1f4))) = *s;
            } while (*s++ != 0);
            v -= 0x639;
            *(int *)((char *)ai_Unk91ac40 + n + 0x34) = v;
            *(int *)((char *)ai_Unk91ac40 + n + 0x38) = v;
            *(int *)((char *)ai_Unk91ac40 + n + 0x3c) = v;
            *(int *)((char *)ai_Unk91ac40 + n + 0x40) = v;
            return (unsigned *)((char *)ai_Unk91ac40 + n);
        }
        if (c[2] == 0x11)
            return ai_Unk617398 + (util_RandomU(3) & 0xffff) * 0x11;
        if (k != 0) {
            if (c[9] == 0) {
                p = ai_Unk612bd8;
                q = p;
            } else {
                q = p;
                if (c[9] == 1) {
                    p = ai_Unk614518;
                    q = p;
                }
            }
            do {
                if (*p == 0)
                    break;
                if (*p == k) {
                    unk_Unk4fb620(q + i * 0x11);
                    return q + i * 0x11;
                }
                i++;
                p += 0x11;
            } while (i < 0x5f);
            return q + i * 0x11;
        }
        if (c[0x165] != 0) {
            if (c[9] == 0) {
                q = ai_Unk611938;
                p = q;
            } else {
                q = p;
                if (c[9] == 1) {
                    q = ai_Unk612288;
                    p = q;
                }
            }
            do {
                if (*q == 0) {
                    if (i < 0x23)
                        return p + i * 0x11;
                    break;
                }
                if (*q == c[0x165]) {
                    unk_Unk4fb620(p + i * 0x11);
                    return p + i * 0x11;
                }
                i++;
                q += 0x11;
            } while (i < 0x23);
        }
        if (c[9] == 0)
            return ai_Unk611938;
        if (c[9] == 1)
            p = ai_Unk612288;
    } else {
        p = ai_Unk6118a8;
        if (c[9] != 0)
            return ai_Unk6118f0;
    }
    return p;
}

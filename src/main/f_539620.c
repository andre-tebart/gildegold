/* 0x539620, a player (the character slot p, kind 6 = prisoner / dead, flag +6 clear) is replaced by a new figure: every record of the table 0x6fab40 (0x14c bytes) of that character whose kind is in the first list is
   closed (0x53e1e0), in the second list its +0x70 is set to -1; the kind byte becomes 5, a name is copied from the name table 0x143ac5c (random 0xbf) or 0x143af70 (0x70, flag +7 of the slot) and the
   record counters are reset. Sets 0x6f259c when no slot of kind 6 is left; ROUGH */

void main_Unk539620(unsigned p)
{
    int kinds[17];
    char *s;
    unsigned char *e;
    unsigned u;
    int i, j, d;
    int *pi;

    kinds[0] = 0x47; kinds[1] = 0; kinds[2] = 0x5d; kinds[3] = 0x11; kinds[4] = 0x30; kinds[5] = 0x35; kinds[6] = 0x42; kinds[7] = 0x60; kinds[8] = 0x4b;
    kinds[9] = 0x5b; kinds[10] = 0x5c; kinds[11] = 0x5a; kinds[12] = 0x46; kinds[13] = 0x2e; kinds[14] = 0x2d; kinds[15] = 0x41; kinds[16] = 0;
    i = 0;
    s = (char *)((unsigned)&ai_CharInts + 0x4);
    while (s[-6] != 6 || *s == 0) {
        s += 0x21c;
        i++;
        if ((int)s > ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x5)) {
            U_I((unsigned)&ai_GameOver) = 1;
            return;
        }
    }
    if (i == 0xffff) {
        U_I((unsigned)&ai_GameOver) = 1;
        return;
    }
    d = p * 0x21c;
    if (*(char *)(((unsigned)&ai_Chars + 0x2) + d) == 6 && *(char *)(((unsigned)&ai_CharInts + 0x4) + d) == 0) {
        e = (unsigned char *)(unsigned)&he_Events;
        do {
            if (*e != 0 && *(unsigned short *)(e + 8) == p) {
                u = 0x5d;
                pi = kinds + 2;
                do {
                    if (u == *e) {
                        he_Unk53e1f0(e);
                        break;
                    }
                    u = pi[1];
                    pi++;
                } while (u != 0);
                j = 0;
                do {
                    if ((unsigned)*(int *)((char *)kinds + j + 8) == *e) {
                        e[0x70] = 0xff;
                        e[0x71] = 0xff;
                        e[0x72] = 0xff;
                        e[0x73] = 0xff;
                        break;
                    }
                    pi = (int *)((char *)kinds + j);
                    j += 4;
                } while (*pi != 0);
            }
            e += 0x14c;
        } while ((int)e < (unsigned)((unsigned)he_Events + 0x53000));
        *(char *)(((unsigned)&ai_Chars + 0x2) + d) = 5;
        if (*(char *)((unsigned)&ai_Unk770b89 + d) == 0) {
            u = util_RandomInt(0xbf);
            s = *(char **)((unsigned)&ai_Unk143ac5c + (u & 0xffff) * 4);
        } else {
            u = util_RandomInt(0x70);
            s = *(char **)((unsigned)&ai_Unk143af70 + (u & 0xffff) * 4);
        }
        strcpy((char *)((unsigned)&ai_Chars + d + 0x30), s);
        *(short *)(((unsigned)&ai_Unk770b89 + 0x47) + p * 0x21c) = (short)U_I((unsigned)&ai_Unk6f1bb8);
        U_I((unsigned)&ai_Unk6f1bb8) = U_I((unsigned)&ai_Unk6f1bb8) + 1;
        pi = (int *)(((unsigned)&ai_Unk770b89 + 0x53) + p * 0x21c);
        for (i = 8; i != 0; i--)
            *pi++ = -1;
        s = (char *)((unsigned)&ai_Unk925520 + 0x8);
        do {
            if (s[0x28] != 0) {
                if (ai_Unk4e4870(U_I((unsigned)&ai_CharInts + p * 0x21c), s - 8) != 0 && (*s == 3 || *s == 5 || *s == 4 || *s == 6 || *s == 9 || *s == 7))
                    ai_Unk4e5180(*(int *)(s - 8));
            }
            s += 0x114;
        } while ((int)s < (unsigned)((unsigned)ai_Unk925520 + 0x6788));
    }
}

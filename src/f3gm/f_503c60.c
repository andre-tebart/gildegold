/* 0x503c60, "gm_KillSpieler": removes the character c: with mode 0 only its data is held (kind 0xf = dead; the clock stamped at +0x28 / +0x48; the party records / pairs of the player cleaned up), else it is removed for good (the slot freed, the number of living (0x6f1bc0) decremented, the references in the other characters' relative lists (+0x5c ..) cleared). ROUGH */

void unk_Unk503c60(int c, int mode)
{
    char buf[0x200];
    char *p;
    int id, i, cnt;
    char *peer;
    int *q, *r;
    unsigned char *b;
    int k;

    id = *(int *)(ai_Chars[c] + 4);
    p = ai_Chars[c];
    if (p[2] != 0xf) {
        *(int *)(p + 8) = 0;
        *(unsigned short *)(p + 0x28) = *(unsigned short *)ai_Time;
        gm_Unk503c20(p);
        p[2] = 0xf;
        peer = 0;
        cnt = 0;
        for (i = 0; i < 0x300; i++) {
            if (ai_Chars[i][2] == 6) {
                peer = ai_Chars[i];
                break;
            }
        }
        q = (int *)(ai_Unk91f716);
        do {
            if (*q == *(int *)(ai_Chars[c] + 4)) {
                k = *(int *)((char *)q - 0x16);
                r = ai_Unk91b6e0V;
                do {
                    if (r[1] == k) {
                        if (peer != 0 && *(int *)(peer + 4) == *r)
                            cnt++;
                        r[1] = -1;
                        *r = -1;
                    }
                    r += 2;
                } while ((int)r < ((unsigned)ai_Unk91b6e0V + 0x4000));
                q[-1] = -1;
                *q = -1;
                *(int *)((char *)q - 0x16) = -1;
            }
            q = (int *)((char *)q + 0x2d);
        } while ((int)q < ((unsigned)((unsigned)ai_Unk91f716 + 0x59fe) + 0x2));
        if (cnt != 0 && peer != 0 && peer != p)
            ai_Unk5a9430(peer, p);
    }
    gm_FreeList(ai_Chars[c] + 0x178);
    if (*(int *)(ai_Chars[c] + 0x184) != 0) {
        ch_Delete(*(int *)(ai_Chars[c] + 0x184));
        *(int *)(ai_Chars[c] + 0x184) = 0;
    }
    b = *(unsigned char **)(ai_Chars[c] + 0x170);
    if (b != 0) {
        if (ai_BTypes[*b * 0x24d] == 1 && b[0x65] != 0)
            b[0x65]--;
        *(int *)(ai_Chars[c] + 0x170) = 0;
    }
    *(int *)(ai_Chars[c] + 0x16c) = 0;
    if (mode == 0) {
        sprintf(buf, "gm_KillSpieler: Killing Player <id = %i>, holding data", id);
    } else {
        if (*(short *)p != -1)
            ai_CharCount--;
        q = (int *)(ai_Chars[0] + 0x5c);
        do {
            i = 8;
            r = q;
            do {
                if (*r == *(int *)(ai_Chars[c] + 4))
                    *r = -1;
                r++;
                i--;
            } while (i != 0);
            q += 0x87;
        } while ((int)q < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x5a));
        memset(p, 0, 0x21c);
        *(short *)p = -1;
        *(int *)(ai_Chars[c] + 4) = -1;
        r = (int *)(ai_Chars[c] + 0x5c);
        for (i = 8; i != 0; i--)
            *r++ = -1;
        if (mode == 1)
            sprintf(buf, "gm_KillSpieler: Killing Player <id = %i> permanently", id);
    }
}
